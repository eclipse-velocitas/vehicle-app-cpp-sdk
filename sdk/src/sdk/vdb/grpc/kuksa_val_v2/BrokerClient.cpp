/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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
#include "sdk/middleware/Middleware.h"
#include "sdk/vdb/grpc/kuksa_val_v2/BrokerAsyncGrpcFacade.h"
#include "sdk/vdb/grpc/kuksa_val_v2/TypeConversions.h"

#include <fmt/core.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <limits>
#include <stdexcept>
#include <utility>

namespace velocitas::kuksa_val_v2 {

namespace {

int assertProtobufArrayLimits(size_t numElements) {
    if (numElements > std::numeric_limits<int>::max()) {
        throw std::runtime_error("# requested datapoints exceeds gRPC limits");
    }
    return static_cast<int>(numElements);
}

} // namespace

BrokerClient::BrokerClient(const std::string& vdbAddress, const std::string& vdbServiceName) {
    logger().info("Connecting to data broker service '{}' via '{}'", vdbServiceName, vdbAddress);
    m_asyncBrokerFacade = std::make_shared<BrokerAsyncGrpcFacade>(
        grpc::CreateChannel(vdbAddress, grpc::InsecureChannelCredentials()));
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

namespace {

void clearUpdateStatus(DataPointMap_t& datapointMap) {
    for (const auto& [key, value] : datapointMap) {
        value->clearUpdateStatus();
    }
}

} // namespace

AsyncSubscriptionPtr_t<DataPointReply> BrokerClient::subscribe(const std::string& query) {
    auto subscription = std::make_shared<AsyncSubscription<DataPointReply>>();

    kuksa::val::v2::SubscribeRequest request;
    parseQueryIntoRequest(request, query);

    auto datapointUpdates = std::make_shared<DataPointMap_t>();
    m_asyncBrokerFacade->Subscribe(
        std::move(request),
        [subscription, datapointUpdates](const auto& item) {
            clearUpdateStatus(*datapointUpdates);
            const auto fieldsMap = item.entries();
            for (const auto& [key, value] : fieldsMap) {
                (*datapointUpdates)[key] = convertFromGrpcDataPoint(key, value);
            }
            subscription->insertNewItem(DataPointReply(DataPointMap_t(*datapointUpdates)));
        },
        [subscription](const auto& status) {
            subscription->insertError(
                Status(fmt::format("Subscribe failed: {}", status.error_message())));
        });

    return subscription;
}

} // namespace velocitas::kuksa_val_v2
