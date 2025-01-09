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

#include "GrpcDataPointValueProvider.h"

#include "sdk/Logger.h"

#include <limits>
#include <utility>

namespace velocitas::sdv_databroker_v1 {

GrpcDataPointValueProvider::GrpcDataPointValueProvider(sdv::databroker::v1::Datapoint datapoint)
    : m_datapoint(std::move(datapoint)) {}

const sdv::databroker::v1::Datapoint& GrpcDataPointValueProvider::getDataPoint() const {
    return m_datapoint;
}

DataPointValue::Failure GrpcDataPointValueProvider::getFailure() const {
    if (!m_datapoint.has_failure_value()) {
        return DataPointValue::Failure::NONE;
    }

    switch (m_datapoint.failure_value()) {
    case sdv::databroker::v1::Datapoint_Failure_INVALID_VALUE:
        return DataPointValue::Failure::INVALID_VALUE;
    case sdv::databroker::v1::Datapoint_Failure_NOT_AVAILABLE:
        return DataPointValue::Failure::NOT_AVAILABLE;
    case sdv::databroker::v1::Datapoint_Failure_UNKNOWN_DATAPOINT:
        return DataPointValue::Failure::UNKNOWN_DATAPOINT;
    case sdv::databroker::v1::Datapoint_Failure_ACCESS_DENIED:
        return DataPointValue::Failure::ACCESS_DENIED;
    case sdv::databroker::v1::Datapoint_Failure_INTERNAL_ERROR:
        return DataPointValue::Failure::INTERNAL_ERROR;
    default:
        logger().error("Unknown 'DataPointValue::Failure': {}",
                       static_cast<int>(m_datapoint.failure_value()));
        assert(false);
        return DataPointValue::Failure::INTERNAL_ERROR;
    }
}

bool GrpcDataPointValueProvider::getBoolValue() const { return m_datapoint.bool_value(); }

std::vector<bool> GrpcDataPointValueProvider::getBoolArrayValue() const {
    auto              valueArray = getDataPoint().bool_array().values();
    std::vector<bool> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

float GrpcDataPointValueProvider::getFloatValue() const { return getDataPoint().float_value(); }

std::vector<float> GrpcDataPointValueProvider::getFloatArrayValue() const {
    auto               valueArray = getDataPoint().float_array().values();
    std::vector<float> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

double GrpcDataPointValueProvider::getDoubleValue() const { return getDataPoint().double_value(); }

std::vector<double> GrpcDataPointValueProvider::getDoubleArrayValue() const {
    auto                valueArray = getDataPoint().double_array().values();
    std::vector<double> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

int8_t GrpcDataPointValueProvider::getInt8Value() const {
    const auto value = getDataPoint().int32_value();
    assert(std::numeric_limits<int8_t>::min() <= value &&
           value <= std::numeric_limits<int8_t>::max());
    return static_cast<int8_t>(value);
}

std::vector<int8_t> GrpcDataPointValueProvider::getInt8ArrayValue() const {
    auto                valueArray = getDataPoint().int32_array().values();
    std::vector<int8_t> result;
    result.reserve(valueArray.size());
    for (const auto value : valueArray) {
        assert(std::numeric_limits<int8_t>::min() <= value &&
               value <= std::numeric_limits<int8_t>::max());
        result.push_back(static_cast<int8_t>(value));
    }
    return result;
}

int16_t GrpcDataPointValueProvider::getInt16Value() const {
    const auto value = getDataPoint().int32_value();
    assert(std::numeric_limits<int16_t>::min() <= value &&
           value <= std::numeric_limits<int16_t>::max());
    return static_cast<int16_t>(value);
}

std::vector<int16_t> GrpcDataPointValueProvider::getInt16ArrayValue() const {
    auto                 valueArray = getDataPoint().int32_array().values();
    std::vector<int16_t> result;
    result.reserve(valueArray.size());
    for (const auto value : valueArray) {
        assert(std::numeric_limits<int16_t>::min() <= value &&
               value <= std::numeric_limits<int16_t>::max());
        result.push_back(static_cast<int16_t>(value));
    }
    return result;
}

int32_t GrpcDataPointValueProvider::getInt32Value() const { return getDataPoint().int32_value(); }

std::vector<int32_t> GrpcDataPointValueProvider::getInt32ArrayValue() const {
    auto                 valueArray = getDataPoint().int32_array().values();
    std::vector<int32_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

int64_t GrpcDataPointValueProvider::getInt64Value() const { return getDataPoint().int64_value(); }

std::vector<int64_t> GrpcDataPointValueProvider::getInt64ArrayValue() const {
    auto                 valueArray = getDataPoint().int64_array().values();
    std::vector<int64_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

uint8_t GrpcDataPointValueProvider::getUint8Value() const {
    const auto value = getDataPoint().uint32_value();
    assert(std::numeric_limits<uint8_t>::min() <= value &&
           value <= std::numeric_limits<uint8_t>::max());
    return static_cast<uint8_t>(value);
}

std::vector<uint8_t> GrpcDataPointValueProvider::getUint8ArrayValue() const {
    auto                 valueArray = getDataPoint().uint32_array().values();
    std::vector<uint8_t> result;
    result.reserve(valueArray.size());
    for (const auto value : valueArray) {
        assert(std::numeric_limits<uint8_t>::min() <= value &&
               value <= std::numeric_limits<uint8_t>::max());
        result.push_back(static_cast<uint8_t>(value));
    }
    return result;
}

uint16_t GrpcDataPointValueProvider::getUint16Value() const {
    const auto value = getDataPoint().uint32_value();
    assert(std::numeric_limits<uint16_t>::min() <= value &&
           value <= std::numeric_limits<uint16_t>::max());
    return static_cast<uint16_t>(value);
}

std::vector<uint16_t> GrpcDataPointValueProvider::getUint16ArrayValue() const {
    auto                  valueArray = getDataPoint().uint32_array().values();
    std::vector<uint16_t> result;
    result.reserve(valueArray.size());
    for (const auto value : valueArray) {
        assert(std::numeric_limits<uint16_t>::min() <= value &&
               value <= std::numeric_limits<uint16_t>::max());
        result.push_back(static_cast<uint16_t>(value));
    }
    return result;
}

uint32_t GrpcDataPointValueProvider::getUint32Value() const {
    return getDataPoint().uint32_value();
}

std::vector<uint32_t> GrpcDataPointValueProvider::getUint32ArrayValue() const {
    auto                  valueArray = getDataPoint().uint32_array().values();
    std::vector<uint32_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

uint64_t GrpcDataPointValueProvider::getUint64Value() const {
    return getDataPoint().uint64_value();
}

std::vector<uint64_t> GrpcDataPointValueProvider::getUint64ArrayValue() const {
    auto                  valueArray = getDataPoint().uint64_array().values();
    std::vector<uint64_t> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

std::string GrpcDataPointValueProvider::getStringValue() const {
    return getDataPoint().string_value();
}

std::vector<std::string> GrpcDataPointValueProvider::getStringArrayValue() const {
    auto                     valueArray = getDataPoint().string_array().values();
    std::vector<std::string> result{valueArray.cbegin(), valueArray.cend()};
    return result;
}

Timestamp GrpcDataPointValueProvider::getTimestamp() const {
    return {m_datapoint.timestamp().seconds(), m_datapoint.timestamp().nanos()};
}

} // namespace velocitas::sdv_databroker_v1
