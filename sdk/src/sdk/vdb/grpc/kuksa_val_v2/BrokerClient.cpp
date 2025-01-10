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
#include "sdk/vdb/grpc/kuksa_val_v2/TypeConversions.h"

#include <fmt/core.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <deque>
#include <limits>
#include <shared_mutex>
#include <stdexcept>
#include <utility>

namespace velocitas::kuksa_val_v2 {

namespace {

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

BrokerClient::BrokerClient(const std::string& vdbAddress, const std::string& vdbServiceName) {
    logger().info("Connecting to data broker service '{}' via '{}'", vdbServiceName, vdbAddress);
    m_asyncBrokerFacade = std::make_shared<BrokerAsyncGrpcFacade>(grpc::CreateCustomChannel(
        vdbAddress, grpc::InsecureChannelCredentials(), getChannelArguments()));
    m_metadataStore     = std::make_shared<MetadataStore>();
    m_activeCalls       = std::make_unique<GrpcClient>();
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
BrokerClient::getDatapoints(const std::vector<std::string>& paths) {
    auto result = std::make_shared<AsyncResult<DataPointReply>>();

    kuksa::val::v2::GetValuesRequest request;

    auto& signalIds = *request.mutable_signal_ids();

    signalIds.Reserve(assertProtobufArrayLimits(paths.size()));
    for (const auto& path : paths) {
        signalIds.Add()->set_path(path);
    }

    m_asyncBrokerFacade->GetValues(
        std::move(request),
        [result, paths](auto reply) {
            const auto& dataPoints = reply.data_points();
            if (dataPoints.size() == paths.size()) {
                DataPointMap_t resultMap;
                auto           path      = paths.cbegin();
                auto           dataPoint = dataPoints.cbegin();
                for (; path != paths.cend(); ++path, ++dataPoint) {
                    resultMap[*path] = convertFromGrpcDataPoint(*path, *dataPoint);
                }
                result->insertResult(DataPointReply(std::move(resultMap)));
            } else {
                result->insertError(Status(fmt::format(
                    "GetDatapoints: Mismatch in # returned data points (#req={}, #ret={})",
                    paths.size(), dataPoints.size())));
            }
        },
        [result](auto status) {
            result->insertError(
                Status(fmt::format("GetDatapoints failed: {}", status.error_message())));
        });
    return result;
}

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
            // Everything went fine, return empty map
        },
        [result](auto status) {
            result->insertError(
                Status(fmt::format("SetDatapoints failed: {}", status.error_message())));
        });
    return result;
}

struct Metadata {
    std::string m_path;
    int32_t     m_id;
    bool        m_isKnown{false};
};

class MetadataStore {
public:
    std::shared_ptr<Metadata> getById(int32_t id) const {
        std::shared_lock lock(m_mutex);
        if (auto metadata = m_idMap.find(id); metadata != m_idMap.end()) {
            return metadata->second;
        }
        return {};
    }

    std::shared_ptr<Metadata> getByPath(const std::string& path) const {
        std::shared_lock lock(m_mutex);
        if (auto metadata = m_pathMap.find(path); metadata != m_pathMap.end()) {
            return metadata->second;
        }
        return {};
    }

    void addMetadata(const std::shared_ptr<Metadata>& metadata) {
        assert(metadata);
        std::unique_lock lock(m_mutex);
        m_pathMap[metadata->m_path] = metadata;
        if (metadata->m_isKnown) {
            m_idMap[metadata->m_id] = metadata;
        }
    }

    void invalidate() {
        std::unique_lock lock(m_mutex);
        m_idMap.clear();
        m_pathMap.clear();
    }

private:
    mutable std::shared_mutex                        m_mutex;
    std::map<int32_t, std::shared_ptr<Metadata>>     m_idMap;
    std::map<std::string, std::shared_ptr<Metadata>> m_pathMap;
};

namespace {

void clearUpdateStatus(DataPointMap_t& datapointMap) {
    for (const auto& [key, value] : datapointMap) {
        value->clearUpdateStatus();
    }
}

class MetadataRequester {
public:
    MetadataRequester(std::shared_ptr<BrokerAsyncGrpcFacade> asyncBrokerFacade,
                      std::shared_ptr<MetadataStore>         metadataStore,
                      std::vector<std::string> signalPaths, std::function<void()> onDone,
                      std::function<void(grpc::Status)> onError)
        : m_asyncBrokerFacade(std::move(asyncBrokerFacade))
        , m_metadataStore(std::move(metadataStore))
        , m_signalPaths(std::move(signalPaths))
        , m_currentPath(m_signalPaths.cend())
        , m_onDone(std::move(onDone))
        , m_onError(std::move(onError)) {}

    void getMetadata() {
        assert(m_currentPath == m_signalPaths.cend());
        m_currentPath = m_signalPaths.cbegin();
        ensureMetadata();
    }

    void ensureMetadata() {
        while (m_currentPath != m_signalPaths.cend() &&
               m_metadataStore->getByPath(*m_currentPath)) {
            ++m_currentPath;
        }
        if (m_currentPath != m_signalPaths.cend()) {
            requestMetadata(*m_currentPath);
        } else {
            m_onDone();
        }
    }

    void requestMetadata(const std::string& path) {
        kuksa::val::v2::ListMetadataRequest request;
        request.set_root(path);
        m_asyncBrokerFacade->ListMetadata(
            request,
            [this](auto&& resp) { onMetadataResponse(std::forward<decltype(resp)>(resp)); },
            [this](auto&& status) { onMetadataError(std::forward<decltype(status)>(status)); });
    }

    void onMetadataResponse(const kuksa::val::v2::ListMetadataResponse& response) {
        assert(response.metadata_size() == 1);
        if (response.metadata_size() != 0) {
            m_metadataStore->addMetadata(std::make_shared<Metadata>(
                Metadata{*m_currentPath, response.metadata(0).id(), true}));
            getNextMetadata();
        } else {
            onMetadataError(grpc::Status(grpc::StatusCode::NOT_FOUND, ""));
        }
    }

    void onMetadataError(grpc::Status status) {
        if (status.error_code() == grpc::StatusCode::NOT_FOUND ||
            status.error_code() == grpc::StatusCode::PERMISSION_DENIED) {
            m_metadataStore->addMetadata(
                std::make_shared<Metadata>(Metadata{*m_currentPath, 0, false}));
            getNextMetadata();
        } else {
            m_currentPath = m_signalPaths.cend();
            m_onError(std::move(status));
        }
    }

    void getNextMetadata() {
        ++m_currentPath;
        ensureMetadata();
    }

private:
    std::shared_ptr<BrokerAsyncGrpcFacade>   m_asyncBrokerFacade;
    std::shared_ptr<MetadataStore>           m_metadataStore;
    std::vector<std::string>                 m_signalPaths;
    std::vector<std::string>::const_iterator m_currentPath;
    std::function<void()>                    m_onDone;
    std::function<void(grpc::Status)>        m_onError;
};

uint32_t determineSubscribeBufferSize() {
    uint32_t bufferSize = 0;
    try {
        auto bufferSizeStr = getEnvVar("SDV_SUBSCRIBE_BUFFER_SIZE", "0");
        bufferSize         = std::stoi(bufferSizeStr);
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

// ToDo: Making this class a GrpcCall to store active subscriptions is a bit "quick & dirty".
// Please check for better solution before merging to main!
class SubscriptionHandler : public GrpcCall {
public:
    SubscriptionHandler(std::shared_ptr<BrokerAsyncGrpcFacade> asyncBrokerFacade,
                        std::shared_ptr<MetadataStore>         metadataStore,
                        std::vector<std::string>               signalPaths)
        : m_asyncBrokerFacade(std::move(asyncBrokerFacade))
        , m_metadataStore(std::move(metadataStore))
        , m_signalPaths(std::move(signalPaths))
        , m_subscription(std::make_shared<AsyncSubscription<DataPointReply>>())
        , m_datapointUpdates(std::make_shared<DataPointMap_t>()) {
        m_metadataRequester = std::make_unique<MetadataRequester>(
            m_asyncBrokerFacade, m_metadataStore, m_signalPaths, [this] { onMetadataPresent(); },
            [this](auto&& status) { onError(std::forward<decltype(status)>(status)); });
    }

    void subscribe() { m_metadataRequester->getMetadata(); }

    void onMetadataPresent() {
        kuksa::val::v2::SubscribeByIdRequest request;
        request.set_buffer_size(getSubscribeBufferSize());
        for (const auto& path : m_signalPaths) {
            auto metadata = m_metadataStore->getByPath(path);
            if (metadata->m_isKnown) {
                request.add_signal_ids(metadata->m_id);
            } else {
                (*m_datapointUpdates)[path] = std::make_shared<DataPointValue>(
                    DataPointValue::Type::INVALID, path, Timestamp{},
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
            auto metadata = m_metadataStore->getById(id);
            if (metadata) {
                const auto& path            = metadata->m_path;
                (*m_datapointUpdates)[path] = convertFromGrpcDataPoint(path, dataPoint);
                logger().debug("onSubscriptionUpdate: signal id={}, path={} received.", id, path);
            } else {
                logger().error("onSubscriptionUpdate: Unknown signal id={} received.", id);
            }
        }
        m_subscription->insertNewItem(DataPointReply(DataPointMap_t(*m_datapointUpdates)));
        clearUpdateStatus(*m_datapointUpdates);
    }

    void onError(const grpc::Status& status) {
        switch (status.error_code()) {
        case grpc::StatusCode::OK:
        case grpc::StatusCode::UNAVAILABLE:
            // The databroker ended the connection or became unavailable. This is most probably a
            // temporary error, so we try to subscribe again
            logger().warn("Connection to databroker lost or failed");
            m_metadataStore->invalidate();
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
        bool anyValueUpdated = false;
        for (const auto& path : m_signalPaths) {
            auto dpValueIter = m_datapointUpdates->find(path);
            if (dpValueIter == m_datapointUpdates->end()) {
                m_datapointUpdates->emplace(std::make_pair(
                    std::string{path}, std::make_shared<DataPointValue>(
                                           DataPointValue::Type::INVALID, path, Timestamp{},
                                           DataPointValue::Failure::NOT_AVAILABLE)));
                anyValueUpdated = true;
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
                    anyValueUpdated = true;
                    break;
                }
            }
        }
        return anyValueUpdated;
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
    std::shared_ptr<MetadataStore>                     m_metadataStore;
    std::vector<std::string>                           m_signalPaths;
    std::unique_ptr<MetadataRequester>                 m_metadataRequester;
    std::shared_ptr<AsyncSubscription<DataPointReply>> m_subscription;
    std::shared_ptr<DataPointMap_t>                    m_datapointUpdates;
    std::shared_ptr<GrpcCall>                          m_grpcSubscriptionCall;
    std::chrono::milliseconds m_resubscribeDelay{RESUBSCRIBE_DELAY_INITIAL};
};

} // namespace

AsyncSubscriptionPtr_t<DataPointReply> BrokerClient::subscribe(const std::string& query) {
    auto signalPaths         = parseQuery(query);
    auto subscriptionHandler = std::make_shared<SubscriptionHandler>(
        m_asyncBrokerFacade, m_metadataStore, std::move(signalPaths));
    m_activeCalls->addActiveCall(subscriptionHandler);
    subscriptionHandler->subscribe();
    return subscriptionHandler->getSubscription();
}

} // namespace velocitas::kuksa_val_v2
