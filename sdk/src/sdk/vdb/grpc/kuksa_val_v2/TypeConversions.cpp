/**
 * Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
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

#include "TypeConversions.h"

#include "sdk/DataPointValue.h"
#include "sdk/Logger.h"
#include "sdk/vdb/grpc/common/TypeConversions.h"

#include <stdexcept>

namespace velocitas::kuksa_val_v2 {

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
    case DataPointValue::Type::INT8: {
        grpcValue.set_int32(dynamic_cast<const TypedDataPointValue<int8_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT8_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int8_t>>*>(&dataPoint)->value();
        grpcValue.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::INT16: {
        grpcValue.set_int32(dynamic_cast<const TypedDataPointValue<int16_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::INT16_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<int16_t>>*>(&dataPoint)->value();
        grpcValue.mutable_int32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
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
    case DataPointValue::Type::UINT8: {
        grpcValue.set_uint32(
            dynamic_cast<const TypedDataPointValue<uint8_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT8_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint8_t>>*>(&dataPoint)->value();
        grpcValue.mutable_uint32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
        break;
    }
    case DataPointValue::Type::UINT16: {
        grpcValue.set_uint32(
            dynamic_cast<const TypedDataPointValue<uint16_t>*>(&dataPoint)->value());
        break;
    }
    case DataPointValue::Type::UINT16_ARRAY: {
        auto array =
            dynamic_cast<const TypedDataPointValue<std::vector<uint16_t>>*>(&dataPoint)->value();
        grpcValue.mutable_uint32_array()->mutable_values()->Assign(array.cbegin(), array.cend());
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

std::shared_ptr<DataPointValue> convertFromGrpcValue(const std::string&           path,
                                                     const kuksa::val::v2::Value& value,
                                                     const Timestamp&             timestamp) {
    switch (value.typed_value_case()) {
    case kuksa::val::v2::Value::TypedValueCase::kString:
        return std::make_shared<TypedDataPointValue<std::string>>(path, value.string(), timestamp);
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
}

std::shared_ptr<DataPointValue>
convertFromGrpcDataPoint(const std::string& path, const kuksa::val::v2::Datapoint& grpcDataPoint) {
    auto timestamp = convertFromGrpcTimestamp(grpcDataPoint.timestamp());
    if (grpcDataPoint.has_value()) {
        return convertFromGrpcValue(path, grpcDataPoint.value(), timestamp);
    }

    return std::make_shared<DataPointValue>(DataPointValue::Type::INVALID, path, timestamp,
                                            DataPointValue::Failure::NOT_AVAILABLE);
}

static const std::string SELECT_STATEMENT{"SELECT "}; // NOLINT(runtime/string)
static const std::string WHERE_STATEMENT{" WHERE "};  // NOLINT(runtime/string)

std::vector<std::string> parseQuery(const std::string& query) {
    if (query.find(SELECT_STATEMENT) != 0) {
        throw std::runtime_error("Mallformed query not starting with \"SELECT \"!");
    }
    if (query.find(WHERE_STATEMENT) != std::string::npos) {
        throw std::runtime_error(
            "Queries (containing WHERE clauses) not allowd with kuksa.val.v2 API!");
    }

    std::vector<std::string> signalPaths;
    for (std::string::size_type first{SELECT_STATEMENT.size()}, last{};
         (first = query.find_first_not_of(' ', first)) != std::string::npos; first = last + 1) {
        last = query.find_first_of(", ", first + 1);
        if (last == std::string::npos) {
            last = query.length();
        }
        std::string path = query.substr(first, last - first);
        signalPaths.emplace_back(std::move(path));
    }

    if (signalPaths.empty()) {
        throw std::runtime_error("Mallformed query selecting no signals!");
    }
    return signalPaths;
}

} // namespace velocitas::kuksa_val_v2
