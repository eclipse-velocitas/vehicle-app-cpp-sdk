/**
 * Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BrokerClient.h"

#include "sdk/DataPointValue.h"
#include "sdk/Logger.h"
#include "sdk/ThreadPool.h"
#include "sdk/Utils.h"
#include "sdk/grpc/GrpcCall.h"
#include "sdk/grpc/GrpcClient.h"
#include "sdk/middleware/Middleware.h"
#include "sdk/vdb/grpc/common/ChannelConfiguration.h"
#include "sdk/vdb/grpc/kuksa_val_v2/BrokerAsyncGrpcFacade.h"
#include "sdk/vdb/grpc/kuksa_val_v2/Metadata.h"
#include "sdk/vdb/grpc/kuksa_val_v2/TypeConversions.h"

#include <fmt/core.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <limits>
#include <shared_mutex>
#include <stdexcept>
#include <utility>

namespace velocitas::kuksa_val_v2 {

namespace {

const unsigned int DEFAULT_SUBSCRIBE_BUFFER_SIZE = 0;

const std::chrono::milliseconds RESUBSCRIBE_DELAY_INITIAL{100};
const std::chrono::milliseconds RESUBSCRIBE_DELAY_MAX{2000};
const unsigned int              RESUBSCRIBE_DELAY_FACTOR{2};

int assertProtobufArrayLimits(size_t numElements) {
    if (numElements > std::numeric_limits<int>::max()) {
        throw std::runtime_error("# requested datapoints exceeds gRPC limits");
    }
    return static_cast<int>(numElements);
}

} // namespace

BrokerClient::BrokerClient(const std::string& vdbAddress, const std::string& vdbServiceName)
    : m_asyncBrokerFacade(std::make_shared<BrokerAsyncGrpcFacade>(grpc::CreateCustomChannel(
          vdbAddress, grpc::InsecureChannelCredentials(), getChannelArguments())))
    , m_metadataAgent(MetadataAgent::create(m_asyncBrokerFacade))
    , m_activeCalls(std::make_unique<GrpcClient>()) {
    logger().info("Connecting to data broker service '{}' via '{}'", vdbServiceName, vdbAddress);
    Middleware::Metadata metadata = Middleware::getInstance().getMetadata(vdbServiceName);
    m_asyncBrokerFacade->setContextModifier([metadata](auto& context) {
        for (auto metadatum : metadata) {
            context.AddMetadata(metadatum.first, metadatum.second);
        }
    });
}

BrokerClient::BrokerClient(const std::string& vdbServiceName)
    : BrokerClient(Middleware::getInstance().getServiceLocation(vdbServiceName), vdbServiceName) {}

BrokerClient::~BrokerClient() {}

AsyncResultPtr_t<DataPointReply>
BrokerClient::getDatapoints(const std::vector<std::string>& signalPaths) {
    auto result = std::make_shared<AsyncResult<DataPointReply>>();
    m_metadataAgent->query(
        signalPaths,
        [this, result](MetadataList_t&& metadataList) {
            kuksa::val::v2::GetValuesRequest request;
            auto&                            signalIds = *request.mutable_signal_ids();
            signalIds.Reserve(assertProtobufArrayLimits(metadataList.size()));
            size_t numRequestedSignals = 0;
            for (const auto& metadata : metadataList) {
                if (metadata->m_isKnown) {
                    signalIds.Add()->set_id(metadata->m_id);
                    ++numRequestedSignals;
                }
            }
            m_asyncBrokerFacade->GetValues(
                std::move(request),
                [this, result, metadataList, numRequestedSignals](auto response) {
                    onGetValuesResponse(response, metadataList, numRequestedSignals, result);
                },
                [this, result, metadataList](auto status) {
                    onGetValuesError(status, metadataList, result);
                });
        },
        [this, result](const auto& status) {
            if (status.error_code() == grpc::StatusCode::UNAVAILABLE) {
                m_metadataAgent->invalidate(status.error_code());
            }
            result->insertError(
                Status(fmt::format("GetDatapoints failed: {}", status.error_message())));
        });
    return result;
}

void BrokerClient::onGetValuesResponse(const kuksa::val::v2::GetValuesResponse& response,
                                       const MetadataList_t&                    metadataList,
                                       const size_t                             numRequestedSignals,
                                       const AsyncResultPtr_t<DataPointReply>&  result) {
    DataPointMap_t resultMap;
    const auto&    dataPoints = response.data_points();
    if (dataPoints.size() == numRequestedSignals) {
        auto dataPointIter = dataPoints.cbegin();
        for (const auto& metadata : metadataList) {
            if (metadata->m_isKnown) {
                assert(dataPointIter != dataPoints.cend());
                resultMap[metadata->m_signalPath] =
                    convertFromGrpcDataPoint(metadata->m_signalPath, *dataPointIter);
                ++dataPointIter;
            } else {
                resultMap[metadata->m_signalPath] = std::make_shared<DataPointValue>(
                    DataPointValue::Type::INVALID, metadata->m_signalPath, Timestamp{},
                    DataPointValue::Failure::UNKNOWN_DATAPOINT);
            }
        }
        result->insertResult(DataPointReply(std::move(resultMap)));
    } else {
        result->insertError(Status(fmt::format("GetDatapoints: Mismatch in # returned data "
                                               "points (#req={}, #ret={})",
                                               numRequestedSignals, dataPoints.size())));
    }
}

void BrokerClient::onGetValuesError(const grpc::Status& status, const MetadataList_t& metadataList,
                                    const AsyncResultPtr_t<DataPointReply>& result) {
    if (status.error_code() == grpc::StatusCode::UNAVAILABLE) {
        m_metadataAgent->invalidate(status.error_code());
        DataPointMap_t resultMap;
        for (const auto& metadata : metadataList) {
            resultMap[metadata->m_signalPath] = std::make_shared<DataPointValue>(
                DataPointValue::Type::INVALID, metadata->m_signalPath, Timestamp{},
                (metadata->m_isKnown ? DataPointValue::Failure::NOT_AVAILABLE
                                     : DataPointValue::Failure::UNKNOWN_DATAPOINT));
        }
        result->insertResult(DataPointReply(std::move(resultMap)));
    } else {
        result->insertError(
            Status(fmt::format("GetDatapoints failed: {}", status.error_message())));
    }
}

namespace {
const std::string errorMessagePart1 = "Provider for vss_id "; // NOLINT(runtime/string)
const std::string errorMessagePart2 = " not available";       // NOLINT(runtime/string)

// The databroker indicates a missing provider of at least one of the addressed signals with the
// error message "Provider for vss_id <some id> not available".
bool isSignalProviderUnavailable(const grpc::Status& status) {
    auto part1Pos = status.error_message().find(errorMessagePart1);
    return (part1Pos != std::string::npos) &&
           (status.error_message().find(errorMessagePart2, part1Pos + errorMessagePart1.length()) !=
            std::string::npos);
}
} // namespace

AsyncResultPtr_t<IVehicleDataBrokerClient::SetErrorMap_t>
BrokerClient::setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) {
    auto result = std::make_shared<AsyncResult<SetErrorMap_t>>();

    kuksa::val::v2::BatchActuateRequest batchRequest;

    auto& requests = *batchRequest.mutable_actuate_requests();
    requests.Reserve(assertProtobufArrayLimits(datapoints.size()));

    for (const auto& dataPoint : datapoints) {
        kuksa::val::v2::ActuateRequest& request = *requests.Add();
        request.mutable_signal_id()->set_path(dataPoint->getPath());
        *request.mutable_value() = convertToGrpcValue(*dataPoint);
    }

    m_asyncBrokerFacade->BatchActuate(
        std::move(batchRequest),
        [result](const kuksa::val::v2::BatchActuateResponse& reply) {
            std::ignore = reply;
            // Everything went fine, return empty map
            result->insertResult(SetErrorMap_t());
        },
        [this, result](auto status) {
            if (status.error_code() == grpc::StatusCode::UNAVAILABLE) {
                // The error code UNAVAILABLE is also used by the databroker to indicate that the
                // provider of at least one of the addressed signals is missing. This situation
                // shall not lead to cache invalidation.
                if (!isSignalProviderUnavailable(status)) {
                    m_metadataAgent->invalidate(status.error_code());
                }
            }
            result->insertError(
                Status(fmt::format("SetDatapoints failed: {} --- Error details: {}",
                                   status.error_message(), status.error_details())));
        });
    return result;
}

namespace {
void clearUpdateStatus(DataPointMap_t& datapointMap) {
    for (auto& entry : datapointMap) {
        entry.second->clearUpdateStatus();
    }
}

uint32_t determineSubscribeBufferSize() {
    uint32_t bufferSize = DEFAULT_SUBSCRIBE_BUFFER_SIZE;
    try {
        auto bufferSizeStr = getEnvVar("SDV_SUBSCRIBE_BUFFER_SIZE");
        if (!bufferSizeStr.empty()) {
            bufferSize = std::stoi(bufferSizeStr);
        }
    } catch (...) {
        logger().error("Invalid Subscribe BufferSize specified via env var! Using default ({}).",
                       bufferSize);
    }
    return bufferSize;
}

uint32_t getSubscribeBufferSize() {
    static uint32_t bufferSize = determineSubscribeBufferSize();
    return bufferSize;
}

std::string getSignalPathAbstract(const std::vector<std::string>& signalPaths) {
    auto abstract{signalPaths.front()};
    if (signalPaths.size() > 1) {
        abstract.append(", etc");
    }
    return abstract;
}

// ToDo: Making this class a GrpcCall to store active subscriptions is a bit "quick &
// dirty". Please check for a better solution!
class SubscriptionHandler : public GrpcCall {
public:
    SubscriptionHandler(std::shared_ptr<BrokerAsyncGrpcFacade> asyncBrokerFacade,
                        std::shared_ptr<MetadataAgent>         metadataAgent,
                        std::vector<std::string>               signalPaths)
        : m_asyncBrokerFacade(std::move(asyncBrokerFacade))
        , m_metadataAgent(std::move(metadataAgent))
        , m_signalPaths(std::move(signalPaths))
        , m_subscription(std::make_shared<AsyncSubscription<DataPointReply>>())
        , m_datapointUpdates(std::make_shared<DataPointMap_t>()) {}

    void subscribe() {
        m_metadataAgent->query(
            m_signalPaths,
            [this](auto&& metadataList) {
                onMetadataPresent(std::forward<decltype(metadataList)>(metadataList));
            },
            [this](const auto& status) { onError(std::forward<decltype(status)>(status)); });
    }

    void onMetadataPresent(MetadataList_t&& metadataList) {
        kuksa::val::v2::SubscribeByIdRequest request;
        if (getSubscribeBufferSize() != DEFAULT_SUBSCRIBE_BUFFER_SIZE) {
            request.set_buffer_size(getSubscribeBufferSize());
        }
        for (const auto& metadata : metadataList) {
            if (metadata->m_isKnown) {
                request.add_signal_ids(metadata->m_id);
            } else {
                (*m_datapointUpdates)[metadata->m_signalPath] = std::make_shared<DataPointValue>(
                    DataPointValue::Type::INVALID, metadata->m_signalPath, Timestamp{},
                    DataPointValue::Failure::UNKNOWN_DATAPOINT);
            }
        }

        assert(!m_grpcSubscriptionCall || m_grpcSubscriptionCall->m_isComplete);
        m_grpcSubscriptionCall = m_asyncBrokerFacade->SubscribeById(
            std::move(request),
            [this](auto&& update) { onUpdate(std::forward<decltype(update)>(update)); },
            [this](auto&& status) { onError(std::forward<decltype(status)>(status)); });
    }

    void onUpdate(const kuksa::val::v2::SubscribeByIdResponse& update) {
        resetResubscribeDelay();
        const auto fieldsMap = update.entries();
        for (const auto& [id, dataPoint] : fieldsMap) {
            auto metadata = m_metadataAgent->getByNumericId(id);
            if (metadata) {
                const auto& path            = metadata->m_signalPath;
                (*m_datapointUpdates)[path] = convertFromGrpcDataPoint(path, dataPoint);
            } else {
                logger().error("onSubscriptionUpdate: Unexpected signal id={} received.", id);
            }
        }
        m_subscription->insertNewItem(DataPointReply(DataPointMap_t(*m_datapointUpdates)));
        clearUpdateStatus(*m_datapointUpdates);
    }

    void onError(const grpc::Status& status) {
        switch (status.error_code()) {
        case grpc::StatusCode::OK:
        case grpc::StatusCode::UNAVAILABLE:
            // The databroker ended the connection or became unavailable. This is most
            // probably a temporary error, so we try to subscribe again
            logger().warn("Connection to databroker lost or failed");
            m_metadataAgent->invalidate();
            if (invalidateDataPointValues()) {
                m_subscription->insertNewItem(DataPointReply(DataPointMap_t(*m_datapointUpdates)));
                clearUpdateStatus(*m_datapointUpdates);
            }
            resubscribe();
            break;
        default:
            // all other errors are rated unrecoverable, therefore retry does not make sense
            m_subscription->insertError(Status(fmt::format(
                "Subscribe failed: code={}, {}", static_cast<unsigned int>(status.error_code()),
                status.error_message())));
            m_isComplete = true;
            break;
        }
    }

    bool invalidateDataPointValues() {
        bool anyValueInvalidated = false;
        for (const auto& path : m_signalPaths) {
            auto dpValueIter = m_datapointUpdates->find(path);
            if (dpValueIter == m_datapointUpdates->end()) {
                m_datapointUpdates->emplace(std::make_pair(
                    std::string{path}, std::make_shared<DataPointValue>(
                                           DataPointValue::Type::INVALID, path, Timestamp{},
                                           DataPointValue::Failure::NOT_AVAILABLE)));
                anyValueInvalidated = true;
            } else {
                auto& dpValue = dpValueIter->second;
                switch (dpValue->getFailure()) {
                case DataPointValue::Failure::NOT_AVAILABLE:
                case DataPointValue::Failure::UNKNOWN_DATAPOINT:
                case DataPointValue::Failure::ACCESS_DENIED:
                    // nothing to do
                    break;
                default:
                    dpValue =
                        std::make_shared<DataPointValue>(dpValue->getType(), path, Timestamp{},
                                                         DataPointValue::Failure::NOT_AVAILABLE);
                    anyValueInvalidated = true;
                    break;
                }
            }
        }
        return anyValueInvalidated;
    }

    void resubscribe() {
        logger().debug("Initiating re-subscribe of {} after {}ms",
                       getSignalPathAbstract(m_signalPaths), m_resubscribeDelay.count());
        ThreadPool::getInstance()->enqueue(Job::create(
            [this, start = std::chrono::steady_clock::now()]() {
                const std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
                logger().debug("Try re-subscribing {} after {}s of waiting",
                               getSignalPathAbstract(m_signalPaths), diff.count());
                increaseResubscribeDelay();
                subscribe();
            },
            m_resubscribeDelay));
    }

    void resetResubscribeDelay() { m_resubscribeDelay = RESUBSCRIBE_DELAY_INITIAL; }

    void increaseResubscribeDelay() {
        m_resubscribeDelay *= RESUBSCRIBE_DELAY_FACTOR;
        if (m_resubscribeDelay > RESUBSCRIBE_DELAY_MAX) {
            m_resubscribeDelay = RESUBSCRIBE_DELAY_MAX;
        }
    }

    [[nodiscard]] AsyncSubscriptionPtr_t<DataPointReply> getSubscription() const {
        return m_subscription;
    }

private:
    std::shared_ptr<BrokerAsyncGrpcFacade>             m_asyncBrokerFacade;
    std::shared_ptr<MetadataAgent>                     m_metadataAgent;
    std::vector<std::string>                           m_signalPaths;
    std::shared_ptr<AsyncSubscription<DataPointReply>> m_subscription;
    std::shared_ptr<DataPointMap_t>                    m_datapointUpdates;
    std::shared_ptr<GrpcCall>                          m_grpcSubscriptionCall;
    std::chrono::milliseconds m_resubscribeDelay{RESUBSCRIBE_DELAY_INITIAL};
};

} // namespace

AsyncSubscriptionPtr_t<DataPointReply> BrokerClient::subscribe(const std::string& query) {
    auto signalPaths         = parseQuery(query);
    auto subscriptionHandler = std::make_shared<SubscriptionHandler>(
        m_asyncBrokerFacade, m_metadataAgent, std::move(signalPaths));
    m_activeCalls->addActiveCall(subscriptionHandler);
    subscriptionHandler->subscribe();
    return subscriptionHandler->getSubscription();
}

} // namespace velocitas::kuksa_val_v2
