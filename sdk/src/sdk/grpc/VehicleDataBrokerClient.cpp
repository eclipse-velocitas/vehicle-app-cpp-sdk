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

#include "sdk/grpc/AsyncGrpcFacade.h"
#include "sdk/grpc/BrokerAsyncGrpcFacade.h"
#include "sdk/grpc/GrpcDataPointValueProvider.h"
#include "sdk/vdb/DataPointsResult.h"

#include "sdk/Exceptions.h"
#include "sdk/Job.h"
#include "sdk/Logger.h"

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

sdv::databroker::v1::Datapoint convertToGrpcDataPoint(const DataPointResult& dataPoint) {
    sdv::databroker::v1::Datapoint grpcDataPoint{};

    switch (dataPoint.getType()) {
    case DataPointResult::Type::BOOL: {
        grpcDataPoint.set_bool_value(
            dynamic_cast<const TypedDataPointResult<bool>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::BOOL_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<bool>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_bool_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::DOUBLE: {
        grpcDataPoint.set_double_value(
            dynamic_cast<const TypedDataPointResult<double>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::DOUBLE_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<double>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_double_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::FLOAT: {
        grpcDataPoint.set_float_value(
            dynamic_cast<const TypedDataPointResult<float>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::FLOAT_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<float>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_float_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::INT32: {
        grpcDataPoint.set_int32_value(
            dynamic_cast<const TypedDataPointResult<int32_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::INT32_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<int32_t>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_int32_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::INT64: {
        grpcDataPoint.set_int64_value(
            dynamic_cast<const TypedDataPointResult<int64_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::INT64_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<int64_t>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_int64_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::STRING: {
        grpcDataPoint.set_string_value(
            dynamic_cast<const TypedDataPointResult<std::string>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::STRING_ARRAY: {
        auto array = dynamic_cast<const TypedDataPointResult<std::vector<std::string>>*>(&dataPoint)
                         ->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_string_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::UINT32: {
        grpcDataPoint.set_uint32_value(
            dynamic_cast<const TypedDataPointResult<uint32_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::UINT32_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<uint32_t>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_uint32_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::UINT64: {
        grpcDataPoint.set_uint64_value(
            dynamic_cast<const TypedDataPointResult<uint64_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointResult::Type::UINT64_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointResult<std::vector<uint64_t>>*>(&dataPoint)->value();
        for (const auto& entry : array) {
            grpcDataPoint.mutable_uint64_array()->add_values(entry);
        }
        break;
    }
    case DataPointResult::Type::INVALID:
    default:
        throw InvalidTypeException("");
    }

    return grpcDataPoint;
}

std::shared_ptr<DataPointResult>
convertDataPointToInternal(const std::string&                    name,
                           const sdv::databroker::v1::Datapoint& grpcDataPoint) {
    GrpcDataPointValueProvider valueProvider{grpcDataPoint};

    switch (grpcDataPoint.value_case()) {
    case sdv::databroker::v1::Datapoint::ValueCase::kStringValue:
        return std::make_shared<TypedDataPointResult<std::string>>(
            name, valueProvider.getStringValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt32Value:
        return std::make_shared<TypedDataPointResult<int32_t>>(name, grpcDataPoint.int32_value(),
                                                               valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt64Value:
        return std::make_shared<TypedDataPointResult<int64_t>>(name, grpcDataPoint.int64_value(),
                                                               valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint32Value:
        return std::make_shared<TypedDataPointResult<uint32_t>>(name, grpcDataPoint.uint32_value(),
                                                                valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint64Value:
        return std::make_shared<TypedDataPointResult<uint64_t>>(name, grpcDataPoint.uint64_value(),
                                                                valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kFloatValue:
        return std::make_shared<TypedDataPointResult<float>>(name, grpcDataPoint.float_value(),
                                                             valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kDoubleValue:
        return std::make_shared<TypedDataPointResult<double>>(name, grpcDataPoint.double_value(),
                                                              valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kStringArray:
        return std::make_shared<TypedDataPointResult<std::vector<std::string>>>(
            name, valueProvider.getStringArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt32Array:
        return std::make_shared<TypedDataPointResult<std::vector<int32_t>>>(
            name, valueProvider.getInt32ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt64Array:
        return std::make_shared<TypedDataPointResult<std::vector<int64_t>>>(
            name, valueProvider.getInt64ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint32Array:
        return std::make_shared<TypedDataPointResult<std::vector<uint32_t>>>(
            name, valueProvider.getUint32ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint64Array:
        return std::make_shared<TypedDataPointResult<std::vector<uint64_t>>>(
            name, valueProvider.getUint64ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kFloatArray:
        return std::make_shared<TypedDataPointResult<std::vector<float>>>(
            name, valueProvider.getFloatArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kDoubleArray:
        return std::make_shared<TypedDataPointResult<std::vector<double>>>(
            name, valueProvider.getDoubleArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kBoolValue:
        return std::make_shared<TypedDataPointResult<bool>>(name, valueProvider.getBoolValue(),
                                                            valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kBoolArray:
        return std::make_shared<TypedDataPointResult<std::vector<bool>>>(
            name, valueProvider.getBoolArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kFailureValue:
        throw RpcException(fmt::format(
            "Datapoint failure: {}",
            sdv::databroker::v1::Datapoint_Failure_Name(grpcDataPoint.failure_value())));
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

AsyncResultPtr_t<IVehicleDataBrokerClient::SetErrorMap_t> VehicleDataBrokerClient::setDatapoints(
    const std::vector<std::unique_ptr<DataPointResult>>& datapoints) {
    auto result = std::make_shared<AsyncResult<SetErrorMap_t>>();

    std::map<std::string, sdv::databroker::v1::Datapoint> grpcDataPoints{};
    for (const auto& dataPoint : datapoints) {
        grpcDataPoints[dataPoint->getPath()] = convertToGrpcDataPoint(*dataPoint);
    }

    m_asyncBrokerFacade->SetDatapoints(
        grpcDataPoints,
        [result](const sdv::databroker::v1::SetDatapointsReply& reply) {
            SetErrorMap_t errorMap;
            for (const auto& [key, error] : reply.errors()) {
                errorMap[key] = sdv::databroker::v1::DatapointError_Name(error);
            }
            result->insertResult(std::move(errorMap));
        },
        [result](auto status) {
            result->insertError(
                Status(fmt::format("RPC 'SetDatapoints' failed:", status.error_message())));
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
