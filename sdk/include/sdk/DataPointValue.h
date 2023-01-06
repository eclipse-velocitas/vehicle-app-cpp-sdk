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

#ifndef VEHICLE_APP_SDK_DATAPOINTVALUE_H
#define VEHICLE_APP_SDK_DATAPOINTVALUE_H

#include "sdk/Exceptions.h"

#include <fmt/core.h>

#include <cassert>
#include <cstdint>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace velocitas {

/**
 * @brief See
 * https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/timestamp.proto
 */
struct Timestamp {
    int64_t seconds{0};
    int32_t nanos{0};
};

inline bool operator==(const Timestamp& lhs, const Timestamp& rhs) {
    return lhs.seconds == rhs.seconds && lhs.nanos == rhs.nanos;
}

class DataPointValue {
public:
    enum class Type {
        INVALID,
        BOOL,
        BOOL_ARRAY,
        INT32,
        INT32_ARRAY,
        INT64,
        INT64_ARRAY,
        UINT32,
        UINT32_ARRAY,
        UINT64,
        UINT64_ARRAY,
        FLOAT,
        FLOAT_ARRAY,
        DOUBLE,
        DOUBLE_ARRAY,
        STRING,
        STRING_ARRAY
    };

    enum class Failure {
        // No failure, i.e. is a valid data point
        NONE,
        // The data point is known, but doesn't have a valid value
        INVALID_VALUE,
        // The data point is known, but no value is available
        NOT_AVAILABLE,
        // Unknown datapoint
        UNKNOWN_DATAPOINT,
        // Access denied
        ACCESS_DENIED,
        // Unexpected internal error
        INTERNAL_ERROR,
    };

    DataPointValue(Type type, std::string path, Timestamp timestamp,
                   Failure failure = Failure::NONE)
        : m_path(std::move(path))
        , m_type{type}
        , m_timestamp(std::move(timestamp))
        , m_failure{failure} {}

    virtual ~DataPointValue()                        = default;
    DataPointValue(const DataPointValue&)            = default;
    DataPointValue(DataPointValue&&)                 = default;
    DataPointValue& operator=(const DataPointValue&) = default;
    DataPointValue& operator=(DataPointValue&&)      = default;

    [[nodiscard]] const std::string& getPath() const { return m_path; }
    [[nodiscard]] Type               getType() const { return m_type; }
    [[nodiscard]] const Timestamp&   getTimestamp() const { return m_timestamp; }
    [[nodiscard]] bool               isValid() const { return m_failure == Failure::NONE; }
    [[nodiscard]] Failure            getFailure() const { return m_failure; }

    bool operator==(const DataPointValue& other) const {
        return std::tie(m_path, m_type, m_timestamp, m_failure) ==
               std::tie(other.m_path, other.m_type, other.m_timestamp, other.m_failure);
    }

private:
    std::string m_path;
    Type        m_type{Type::INVALID};
    Timestamp   m_timestamp{};
    Failure     m_failure{Failure::NONE};
};

std::string toString(DataPointValue::Failure);

template <typename T> DataPointValue::Type getValueType() {
    static_assert(std::is_same<T, std::false_type>::value, "Value type not supported!");
    return DataPointValue::Type::INVALID;
}

template <> inline DataPointValue::Type getValueType<bool>() { return DataPointValue::Type::BOOL; }
template <> inline DataPointValue::Type getValueType<std::vector<bool>>() {
    return DataPointValue::Type::BOOL_ARRAY;
}
template <> inline DataPointValue::Type getValueType<int32_t>() {
    return DataPointValue::Type::INT32;
}
template <> inline DataPointValue::Type getValueType<std::vector<int32_t>>() {
    return DataPointValue::Type::INT32_ARRAY;
}
template <> inline DataPointValue::Type getValueType<int64_t>() {
    return DataPointValue::Type::INT64;
}
template <> inline DataPointValue::Type getValueType<std::vector<int64_t>>() {
    return DataPointValue::Type::INT64_ARRAY;
}
template <> inline DataPointValue::Type getValueType<uint32_t>() {
    return DataPointValue::Type::UINT32;
}
template <> inline DataPointValue::Type getValueType<std::vector<uint32_t>>() {
    return DataPointValue::Type::UINT32_ARRAY;
}
template <> inline DataPointValue::Type getValueType<uint64_t>() {
    return DataPointValue::Type::UINT64;
}
template <> inline DataPointValue::Type getValueType<std::vector<uint64_t>>() {
    return DataPointValue::Type::UINT64_ARRAY;
}
template <> inline DataPointValue::Type getValueType<float>() {
    return DataPointValue::Type::FLOAT;
}
template <> inline DataPointValue::Type getValueType<std::vector<float>>() {
    return DataPointValue::Type::FLOAT_ARRAY;
}
template <> inline DataPointValue::Type getValueType<double>() {
    return DataPointValue::Type::DOUBLE;
}
template <> inline DataPointValue::Type getValueType<std::vector<double>>() {
    return DataPointValue::Type::DOUBLE_ARRAY;
}
template <> inline DataPointValue::Type getValueType<std::string>() {
    return DataPointValue::Type::STRING;
}
template <> inline DataPointValue::Type getValueType<std::vector<std::string>>() {
    return DataPointValue::Type::STRING_ARRAY;
}

template <typename T> class TypedDataPointValue : public DataPointValue {
public:
    TypedDataPointValue()
        : DataPointValue(Type::INVALID, "", Timestamp{}){};

    TypedDataPointValue(const std::string& path, T value, Timestamp timestamp = Timestamp{})
        : DataPointValue(getValueType<T>(), path, std::forward<decltype(timestamp)>(timestamp))
        , m_value(std::move(value)) {}

    TypedDataPointValue(const std::string& path, DataPointValue::Failure failure,
                        Timestamp timestamp = Timestamp{})
        : DataPointValue(getValueType<T>(), path, std::forward<decltype(timestamp)>(timestamp),
                         failure)
        , m_value{} {
        assert(failure != Failure::NONE);
    }

    [[nodiscard]] const T& value() const {
        if (!isValid()) {
            throw InvalidValueException(
                fmt::format("'{}' has no valid value: {}!", getPath(), toString(getFailure())));
        }
        return m_value;
    }

    bool operator==(const TypedDataPointValue& other) const {
        return DataPointValue::operator==(other) && m_value == other.m_value;
    }

private:
    T m_value;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_DATAPOINTVALUE_H
