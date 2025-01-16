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
#include "sdk/Exceptions.h"
#include "sdk/Logger.h"

#include "sdk/middleware/Middleware.h"
#include "sdk/vdb/grpc/common/ChannelConfiguration.h"
#include "sdk/vdb/grpc/sdv_databroker_v1/BrokerAsyncGrpcFacade.h"
#include "sdk/vdb/grpc/sdv_databroker_v1/GrpcDataPointValueProvider.h"

#include <fmt/core.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <thread>
#include <utility>

namespace velocitas::sdv_databroker_v1 {

BrokerClient::BrokerClient(const std::string& vdbAddress, const std::string& vdbServiceName) {
    logger().info("Connecting to data broker service '{}' via '{}'", vdbServiceName, vdbAddress);
    m_asyncBrokerFacade = std::make_shared<BrokerAsyncGrpcFacade>(grpc::CreateCustomChannel(
        vdbAddress, grpc::InsecureChannelCredentials(), getChannelArguments()));
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

static sdv::databroker::v1::Datapoint_Failure mapToGrpcType(DataPointValue::Failure failure) {
    switch (failure) {
    case DataPointValue::Failure::INVALID_VALUE:
        return sdv::databroker::v1::Datapoint_Failure_INVALID_VALUE;
    case DataPointValue::Failure::NOT_AVAILABLE:
        return sdv::databroker::v1::Datapoint_Failure_NOT_AVAILABLE;
    case DataPointValue::Failure::UNKNOWN_DATAPOINT:
        return sdv::databroker::v1::Datapoint_Failure_UNKNOWN_DATAPOINT;
    case DataPointValue::Failure::ACCESS_DENIED:
        return sdv::databroker::v1::Datapoint_Failure_ACCESS_DENIED;
    case DataPointValue::Failure::INTERNAL_ERROR:
        return sdv::databroker::v1::Datapoint_Failure_INTERNAL_ERROR;
    default:
        logger().error("Unknown 'DataPointValue::Failure': {}", static_cast<unsigned int>(failure));
        assert(false);
        return sdv::databroker::v1::Datapoint_Failure_INTERNAL_ERROR;
    }
}

sdv::databroker::v1::Datapoint convertToGrpcDataPoint(const DataPointValue& dataPoint) {
    sdv::databroker::v1::Datapoint grpcDataPoint{};

    switch (dataPoint.getType()) {
    case DataPointValue::Type::BOOL: {
        grpcDataPoint.set_bool_value(
            dynamic_cast<const TypedDataPointValue<bool>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::BOOL_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<bool>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_bool_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::DOUBLE: {
        grpcDataPoint.set_double_value(
            dynamic_cast<const TypedDataPointValue<double>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::DOUBLE_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<double>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_double_array()->mutable_values()->Assign(array.cbegin(),
                                                                       array.cend());
        break;
    }
    case DataPointValue::Type::FLOAT: {
        grpcDataPoint.set_float_value(
            dynamic_cast<const TypedDataPointValue<float>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::FLOAT_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<float>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_float_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT8: {
        grpcDataPoint.set_int32_value(
            dynamic_cast<const TypedDataPointValue<int8_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT8_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int8_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT16: {
        grpcDataPoint.set_int32_value(
            dynamic_cast<const TypedDataPointValue<int16_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT16_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int16_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT32: {
        grpcDataPoint.set_int32_value(
            dynamic_cast<const TypedDataPointValue<int32_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT32_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int32_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT64: {
        grpcDataPoint.set_int64_value(
            dynamic_cast<const TypedDataPointValue<int64_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT64_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int64_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_int64_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::STRING: {
        grpcDataPoint.set_string_value(
            dynamic_cast<const TypedDataPointValue<std::string>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::STRING_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<std::string>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_string_array()->mutable_values()->Assign(array.cbegin(),
                                                                       array.cend());
        break;
    }
    case DataPointValue::Type::UINT8: {
        grpcDataPoint.set_int32_value(
            dynamic_cast<const TypedDataPointValue<uint8_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT8_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint8_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::UINT16: {
        grpcDataPoint.set_int32_value(
            dynamic_cast<const TypedDataPointValue<uint16_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT16_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint16_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::UINT32: {
        grpcDataPoint.set_uint32_value(
            dynamic_cast<const TypedDataPointValue<uint32_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT32_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint32_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_uint32_array()->mutable_values()->Assign(array.cbegin(),
                                                                       array.cend());
        break;
    }
    case DataPointValue::Type::UINT64: {
        grpcDataPoint.set_uint64_value(
            dynamic_cast<const TypedDataPointValue<uint64_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT64_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint64_t>>*>(&dataPoint)->value();
        grpcDataPoint.mutable_uint64_array()->mutable_values()->Assign(array.cbegin(),
                                                                       array.cend());
        break;
    }
    default:
        throw InvalidTypeException("");
    }

    return grpcDataPoint;
}

std::shared_ptr<DataPointValue>
convertDataPointToInternal(const std::string&                    name,
                           const sdv::databroker::v1::Datapoint& grpcDataPoint) {
    GrpcDataPointValueProvider valueProvider{grpcDataPoint};

    switch (grpcDataPoint.value_case()) {
    case sdv::databroker::v1::Datapoint::ValueCase::kFailureValue:
        return std::make_shared<DataPointValue>(DataPointValue::Type::INVALID, name,
                                                valueProvider.getTimestamp(),
                                                valueProvider.getFailure());
    case sdv::databroker::v1::Datapoint::ValueCase::kStringValue:
        return std::make_shared<TypedDataPointValue<std::string>>(
            name, valueProvider.getStringValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kBoolValue:
        return std::make_shared<TypedDataPointValue<bool>>(name, valueProvider.getBoolValue(),
                                                           valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt32Value:
        return std::make_shared<TypedDataPointValue<int32_t>>(name, valueProvider.getInt32Value(),
                                                              valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt64Value:
        return std::make_shared<TypedDataPointValue<int64_t>>(name, valueProvider.getInt64Value(),
                                                              valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint32Value:
        return std::make_shared<TypedDataPointValue<uint32_t>>(name, valueProvider.getUint32Value(),
                                                               valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint64Value:
        return std::make_shared<TypedDataPointValue<uint64_t>>(name, valueProvider.getUint64Value(),
                                                               valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kFloatValue:
        return std::make_shared<TypedDataPointValue<float>>(name, valueProvider.getFloatValue(),
                                                            valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kDoubleValue:
        return std::make_shared<TypedDataPointValue<double>>(name, valueProvider.getDoubleValue(),
                                                             valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kStringArray:
        return std::make_shared<TypedDataPointValue<std::vector<std::string>>>(
            name, valueProvider.getStringArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kBoolArray:
        return std::make_shared<TypedDataPointValue<std::vector<bool>>>(
            name, valueProvider.getBoolArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt32Array:
        return std::make_shared<TypedDataPointValue<std::vector<int32_t>>>(
            name, valueProvider.getInt32ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kInt64Array:
        return std::make_shared<TypedDataPointValue<std::vector<int64_t>>>(
            name, valueProvider.getInt64ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint32Array:
        return std::make_shared<TypedDataPointValue<std::vector<uint32_t>>>(
            name, valueProvider.getUint32ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kUint64Array:
        return std::make_shared<TypedDataPointValue<std::vector<uint64_t>>>(
            name, valueProvider.getUint64ArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kFloatArray:
        return std::make_shared<TypedDataPointValue<std::vector<float>>>(
            name, valueProvider.getFloatArrayValue(), valueProvider.getTimestamp());
    case sdv::databroker::v1::Datapoint::ValueCase::kDoubleArray:
        return std::make_shared<TypedDataPointValue<std::vector<double>>>(
            name, valueProvider.getDoubleArrayValue(), valueProvider.getTimestamp());
    default:
        throw RpcException("Unknown value case!");
    }

    return nullptr;
}

AsyncResultPtr_t<DataPointReply>
BrokerClient::getDatapoints(const std::vector<std::string>& datapoints) {
    auto result = std::make_shared<AsyncResult<DataPointReply>>();
    m_asyncBrokerFacade->GetDatapoints(
        datapoints,
        [result](auto reply) {
            DataPointMap_t resultMap;
            for (const auto [key, value] : reply.datapoints()) {
                resultMap[key] = convertDataPointToInternal(key, value);
            }

            result->insertResult(DataPointReply(std::move(resultMap)));
        },
        [result](auto status) {
            result->insertError(
                Status(fmt::format("RPC 'GetDatapoints' failed: {}", status.error_message())));
        });
    return result;
}

AsyncResultPtr_t<IVehicleDataBrokerClient::SetErrorMap_t>
BrokerClient::setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) {
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
                Status(fmt::format("RPC 'SetDatapoints' failed: {}", status.error_message())));
        });
    return result;
}

AsyncSubscriptionPtr_t<DataPointReply> BrokerClient::subscribe(const std::string& query) {
    auto subscription = std::make_shared<AsyncSubscription<DataPointReply>>();
    m_asyncBrokerFacade->Subscribe(
        query,
        [subscription](const auto& item) {
            DataPointMap_t resultFields;
            const auto     fieldsMap = item.fields();
            for (const auto& [key, value] : fieldsMap) {
                resultFields[key] = convertDataPointToInternal(key, value);
            }
            subscription->insertNewItem(DataPointReply(std::move(resultFields)));
        },
        [subscription](const auto& status) {
            subscription->insertError(
                Status(fmt::format("RPC 'Subscribe' failed: {}", status.error_message())));
        });

    return subscription;
}

} // namespace velocitas::sdv_databroker_v1
