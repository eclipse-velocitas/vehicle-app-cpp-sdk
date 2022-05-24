/**
 * Copyright (c) 2022 Robert Bosch GmbH
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

#include "sdk/grpc/VehicleDataBrokerClient.h"

#include "sdk/dapr/DaprSupport.h"
#include "sdk/grpc/AsyncGrpcFacade.h"
#include "sdk/grpc/BrokerAsyncGrpcFacade.h"
#include "sdk/grpc/GrpcDataPointValueProvider.h"

#include "sdk/Exceptions.h"
#include "sdk/Job.h"
#include "sdk/Logger.h"
#include "sdk/ThreadPool.h"

#include <fmt/core.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <thread>
#include <utility>

namespace velocitas {

VehicleDataBrokerClient::VehicleDataBrokerClient(const std::string& vdbAddress,
                                                 std::string        vdbAppId)
    : m_vdbAppId(std::move(vdbAppId)) {
    dapr::waitForSidecar();

    m_asyncBrokerFacade = std::make_shared<BrokerAsyncGrpcFacade>(
        grpc::CreateChannel(vdbAddress, grpc::InsecureChannelCredentials()));
    m_asyncBrokerFacade->setContextModifier(
        [this](auto& context) { context.AddMetadata("dapr-app-id", m_vdbAppId); });
}

VehicleDataBrokerClient::VehicleDataBrokerClient(const std::string& vdbAppId)
    : VehicleDataBrokerClient(getVdbEndpointAddress(), vdbAppId) {}

VehicleDataBrokerClient::~VehicleDataBrokerClient() {}

std::string VehicleDataBrokerClient::getVdbEndpointAddress() {
    constexpr auto DEFAULT_DAPR_GRPC_PORT{52001};

    auto        daprGrpcPort = DEFAULT_DAPR_GRPC_PORT;
    auto* const envPort      = std::getenv("DAPR_GRPC_PORT");
    if (envPort != nullptr) {
        daprGrpcPort = atoi(envPort);
    }

    return fmt::format("localhost:{}", daprGrpcPort);
}

std::shared_ptr<DataPoint>
convertDataPointToInternal(const std::string&                    name,
                           const sdv::databroker::v1::Datapoint& grpcDataPoint) {
    auto valueProvider = std::make_shared<GrpcDataPointValueProvider>(grpcDataPoint);
    switch (grpcDataPoint.value_case()) {
    case sdv::databroker::v1::Datapoint::ValueCase::kStringValue:
        return std::make_shared<DataPointString>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kInt32Value:
        return std::make_shared<DataPointInt32>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kInt64Value:
        return std::make_shared<DataPointInt64>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kUint32Value:
        return std::make_shared<DataPointUint32>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kUint64Value:
        return std::make_shared<DataPointUint64>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kFloatValue:
        return std::make_shared<DataPointFloat>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kDoubleValue:
        return std::make_shared<DataPointDouble>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kStringArray:
        return std::make_shared<DataPointStringArray>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kInt32Array:
        return std::make_shared<DataPointInt32Array>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kInt64Array:
        return std::make_shared<DataPointInt64Array>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kUint32Array:
        return std::make_shared<DataPointUint32Array>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kUint64Array:
        return std::make_shared<DataPointUint64Array>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kFloatArray:
        return std::make_shared<DataPointFloatArray>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kDoubleArray:
        return std::make_shared<DataPointDoubleArray>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kBoolValue:
        return std::make_shared<DataPointBoolean>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kBoolArray:
        return std::make_shared<DataPointBooleanArray>(name, valueProvider);
    case sdv::databroker::v1::Datapoint::ValueCase::kFailureValue:
        return std::make_shared<DataPointFailure>(
            name, sdv::databroker::v1::Datapoint_Failure_Name(grpcDataPoint.failure_value()));
    default:
        throw RpcException("Unknown value case!");
    }

    return nullptr;
}

AsyncResultPtr_t<DataPointsResult>
VehicleDataBrokerClient::getDatapoints(const std::vector<std::string>& datapoints) {
    auto result = std::make_shared<AsyncResult<DataPointsResult>>();
    m_asyncBrokerFacade->GetDatapoints(
        datapoints,
        [result](auto reply) {
            DataPointMap_t resultMap;
            for (const auto [key, value] : reply.datapoints()) {
                resultMap[key] = convertDataPointToInternal(key, value);
            }

            result->insertResult(DataPointsResult(std::move(resultMap)));
        },
        [result](auto status) {
            result->insertError(
                Status(fmt::format("RPC 'GetDatapoints' failed:", status.error_message())));
        });
    return result;
}

AsyncSubscriptionPtr_t<DataPointsResult>
VehicleDataBrokerClient::subscribe(const std::string& query) {
    auto subscription = std::make_shared<AsyncSubscription<DataPointsResult>>();
    m_asyncBrokerFacade->Subscribe(
        query,
        [subscription](const auto& item) {
            DataPointMap_t resultFields;
            const auto     fieldsMap = item.fields();
            for (const auto& [key, value] : fieldsMap) {
                resultFields[key] = convertDataPointToInternal(key, value);
            }
            subscription->insertNewItem(DataPointsResult(std::move(resultFields)));
        },
        [subscription](const auto& status) {
            subscription->insertError(
                Status(fmt::format("RPC 'Subscribe' failed: {}", status.error_message())));
        });

    return subscription;
}

std::shared_ptr<IVehicleDataBrokerClient>
IVehicleDataBrokerClient::createInstance(const std::string& vdbAppId) {
    return std::make_shared<VehicleDataBrokerClient>(vdbAppId);
}

} // namespace velocitas
