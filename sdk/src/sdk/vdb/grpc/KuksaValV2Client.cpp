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

#include "sdk/vdb/grpc/KuksaValV2Client.h"

#include "sdk/DataPointValue.h"
#include "sdk/Exceptions.h"
#include "sdk/Job.h"
#include "sdk/Logger.h"

#include "sdk/middleware/Middleware.h"
#include "sdk/vdb/grpc/KuksaValV2AsyncGrpcFacade.h"

#include <fmt/core.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <limits>
#include <stdexcept>
#include <utility>

namespace velocitas {

namespace {

int assertProtobufArrayLimits(size_t numElements) {
    if (numElements > std::numeric_limits<int>::max()) {
        throw std::runtime_error("# requested datapoints exceeds gRPC limits");
    }
    return static_cast<int>(numElements);
}

kuksa::val::v2::Value convertToGrpcValue(const DataPointValue& dataPoint) {
    kuksa::val::v2::Value grpcValue;

    switch (dataPoint.getType()) {
    case DataPointValue::Type::BOOL: {
        grpcValue.set_bool_(dynamic_cast<const TypedDataPointValue<bool>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::BOOL_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<bool>>*>(&dataPoint)->value();
        grpcValue.mutable_bool_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT32: {
        grpcValue.set_int32(dynamic_cast<const TypedDataPointValue<int32_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT32_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int32_t>>*>(&dataPoint)->value();
        grpcValue.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT64: {
        grpcValue.set_int64(dynamic_cast<const TypedDataPointValue<int64_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT64_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int64_t>>*>(&dataPoint)->value();
        grpcValue.mutable_int64_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::UINT32: {
        grpcValue.set_uint32(
            dynamic_cast<const TypedDataPointValue<uint32_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT32_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint32_t>>*>(&dataPoint)->value();
        grpcValue.mutable_uint32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::UINT64: {
        grpcValue.set_uint64(
            dynamic_cast<const TypedDataPointValue<uint64_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT64_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint64_t>>*>(&dataPoint)->value();
        grpcValue.mutable_uint64_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::FLOAT: {
        grpcValue.set_float_(dynamic_cast<const TypedDataPointValue<float>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::FLOAT_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<float>>*>(&dataPoint)->value();
        grpcValue.mutable_float_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::DOUBLE: {
        grpcValue.set_double_(
            dynamic_cast<const TypedDataPointValue<double>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::DOUBLE_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<double>>*>(&dataPoint)->value();
        grpcValue.mutable_double_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::STRING: {
        grpcValue.set_string(
            dynamic_cast<const TypedDataPointValue<std::string>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::STRING_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<std::string>>*>(&dataPoint)->value();
        grpcValue.mutable_string_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    default:
        throw InvalidTypeException("");
    }

    return grpcValue;
}

template <typename DATA_TYPE, typename ARRAY_CLASS>
std::vector<DATA_TYPE> convertValueArray(const ARRAY_CLASS& arrayObject) {
    const auto&            valueArray = arrayObject.values();
    std::vector<DATA_TYPE> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

Timestamp convertFromGrpcTimestamp(const google::protobuf::Timestamp& grpcTimestamp) {
    return {grpcTimestamp.seconds(), grpcTimestamp.nanos()};
}

DataPointValue::Failure
convertFromGrpcValueFailure(const kuksa::val::v2::ValueFailure grpcValueFailure) {
    switch (grpcValueFailure) {
    case kuksa::val::v2::ValueFailure::INVALID_VALUE:
        return DataPointValue::Failure::INVALID_VALUE;
    case kuksa::val::v2::ValueFailure::NOT_PROVIDED:
        return DataPointValue::Failure::NOT_AVAILABLE;
    case kuksa::val::v2::ValueFailure::UNKNOWN_SIGNAL:
        return DataPointValue::Failure::UNKNOWN_DATAPOINT;
    case kuksa::val::v2::ValueFailure::ACCESS_DENIED:
        return DataPointValue::Failure::ACCESS_DENIED;
    case kuksa::val::v2::ValueFailure::INTERNAL_ERROR:
        return DataPointValue::Failure::INTERNAL_ERROR;
    default:
        logger().error("Unknown 'kuksa::val::v2::ValueFailure': {}",
                       kuksa::val::v2::ValueFailure_Name(grpcValueFailure));
        assert(false);
        return DataPointValue::Failure::INTERNAL_ERROR;
    }
}

std::shared_ptr<DataPointValue>
convertFromGrpcDataPoint(const std::string& path, const kuksa::val::v2::Datapoint& grpcDataPoint) {
    auto timestamp = convertFromGrpcTimestamp(grpcDataPoint.timestamp());
    if (grpcDataPoint.has_value()) {
        const auto& value = grpcDataPoint.value();
        switch (value.typed_value_case()) {
        case kuksa::val::v2::Value::TypedValueCase::kString:
            return std::make_shared<TypedDataPointValue<std::string>>(path, value.string(),
                                                                      timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kBool:
            return std::make_shared<TypedDataPointValue<bool>>(path, value.bool_(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kInt32:
            return std::make_shared<TypedDataPointValue<int32_t>>(path, value.int32(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kInt64:
            return std::make_shared<TypedDataPointValue<int64_t>>(path, value.int64(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kUint32:
            return std::make_shared<TypedDataPointValue<uint32_t>>(path, value.uint32(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kUint64:
            return std::make_shared<TypedDataPointValue<uint64_t>>(path, value.uint64(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kFloat:
            return std::make_shared<TypedDataPointValue<float>>(path, value.float_(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kDouble:
            return std::make_shared<TypedDataPointValue<double>>(path, value.double_(), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kStringArray:
            return std::make_shared<TypedDataPointValue<std::vector<std::string>>>(
                path, convertValueArray<std::string>(value.string_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kBoolArray:
            return std::make_shared<TypedDataPointValue<std::vector<bool>>>(
                path, convertValueArray<bool>(value.bool_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kInt32Array:
            return std::make_shared<TypedDataPointValue<std::vector<int32_t>>>(
                path, convertValueArray<int32_t>(value.int32_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kInt64Array:
            return std::make_shared<TypedDataPointValue<std::vector<int64_t>>>(
                path, convertValueArray<int64_t>(value.int64_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kUint32Array:
            return std::make_shared<TypedDataPointValue<std::vector<uint32_t>>>(
                path, convertValueArray<uint32_t>(value.uint32_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kUint64Array:
            return std::make_shared<TypedDataPointValue<std::vector<uint64_t>>>(
                path, convertValueArray<uint64_t>(value.uint64_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kFloatArray:
            return std::make_shared<TypedDataPointValue<std::vector<float>>>(
                path, convertValueArray<float>(value.float_array()), timestamp);
        case kuksa::val::v2::Value::TypedValueCase::kDoubleArray:
            return std::make_shared<TypedDataPointValue<std::vector<double>>>(
                path, convertValueArray<double>(value.double_array()), timestamp);
        default:
            throw RpcException("Unknown value case!");
        }
    } else {
        DataPointValue::Failure failure = DataPointValue::Failure::INTERNAL_ERROR;
        if (grpcDataPoint.has_failure()) {
            failure = convertFromGrpcValueFailure(grpcDataPoint.failure());
        }
        return std::make_shared<DataPointValue>(DataPointValue::Type::INVALID, path, timestamp,
                                                failure);
    }
}

} // namespace

KuksaValV2Client::KuksaValV2Client(const std::string& vdbAddress, std::string vdbServiceName) {
    logger().info("Connecting to data broker service '{}' via '{}'", vdbServiceName, vdbAddress);
    m_asyncBrokerFacade = std::make_shared<KuksaValV2AsyncGrpcFacade>(
        grpc::CreateChannel(vdbAddress, grpc::InsecureChannelCredentials()));
    Middleware::Metadata metadata = Middleware::getInstance().getMetadata(vdbServiceName);
    m_asyncBrokerFacade->setContextModifier([metadata](auto& context) {
        for (auto metadatum : metadata) {
            context.AddMetadata(metadatum.first, metadatum.second);
        }
    });
}

KuksaValV2Client::KuksaValV2Client(const std::string& vdbServiceName)
    : KuksaValV2Client(Middleware::getInstance().getServiceLocation(vdbServiceName),
                       vdbServiceName) {}

KuksaValV2Client::~KuksaValV2Client() {}

AsyncResultPtr_t<DataPointReply>
KuksaValV2Client::getDatapoints(const std::vector<std::string>& paths) {
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
            const auto& dataPoints = reply.datapoints();
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
                Status(fmt::format("GetDatapoints failed:", status.error_message())));
        });
    return result;
}

AsyncResultPtr_t<IVehicleDataBrokerClient::SetErrorMap_t>
KuksaValV2Client::setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) {
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
                Status(fmt::format("SetDatapoints failed:", status.error_message())));
        });
    return result;
}

namespace {

const std::string SELECT_STATEMENT{"SELECT "};
const std::string WHERE_STATEMENT{" WHERE "};

void parseQueryIntoRequest(kuksa::val::v2::SubscribeRequest& request, const std::string& query) {
    if (query.find(SELECT_STATEMENT) != 0) {
        throw std::runtime_error("Mallformed query not starting with \"SELECT \"!");
    }
    if (query.find(WHERE_STATEMENT) != std::string::npos) {
        throw std::runtime_error(
            "Queries (containing WHERE clauses) not allowd with kuksa.val.v2 API!");
    }

    for (std::string::size_type first{SELECT_STATEMENT.size()}, last{};
         (first = query.find_first_not_of(' ', first)) != std::string::npos; first = last + 1) {
        last = query.find_first_of(", ", first + 1);
        if (last == std::string::npos) {
            last = query.length();
        }
        std::string path = query.substr(first, last - first);
        request.add_signal_paths(std::move(path));
    }

    if (request.signal_paths().empty()) {
        throw std::runtime_error("Mallformed query selecting no signals!");
    }
}

void clearUpdateStatus(DataPointMap_t& datapointMap) {
    for (const auto& [key, value] : datapointMap) {
        value->clearUpdateStatus();
    }
}

} // namespace

AsyncSubscriptionPtr_t<DataPointReply> KuksaValV2Client::subscribe(const std::string& query) {
    auto subscription = std::make_shared<AsyncSubscription<DataPointReply>>();

    kuksa::val::v2::SubscribeRequest request;
    parseQueryIntoRequest(request, query);

    auto lastUpdates = std::make_shared<DataPointMap_t>();
    m_asyncBrokerFacade->Subscribe(
        std::move(request),
        [subscription, lastUpdates](const auto& item) {
            clearUpdateStatus(*lastUpdates);
            const auto fieldsMap = item.entries();
            for (const auto& [key, value] : fieldsMap) {
                (*lastUpdates)[key] = convertFromGrpcDataPoint(key, value);
            }
            subscription->insertNewItem(DataPointReply(DataPointMap_t(*lastUpdates)));
        },
        [subscription](const auto& status) {
            subscription->insertError(
                Status(fmt::format("Subscribe failed: {}", status.error_message())));
        });

    return subscription;
}

} // namespace velocitas
