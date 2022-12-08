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

#ifndef VEHICLE_APP_SDK_DATAPOINTSRESULT_H
#define VEHICLE_APP_SDK_DATAPOINTSRESULT_H

#include "sdk/Exceptions.h"

#include <fmt/core.h>

#include <map>
#include <memory>
#include <string>
#include <variant>
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
    return lhs.seconds == rhs.seconds && lhs.seconds == rhs.seconds;
}

class DataPoint;

class DataPointResult {
public:
    enum class Type {
        INVALID = -1,
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

    DataPointResult(Type type, std::string path, Timestamp timestamp)
        : m_type{type}
        , m_path(std::move(path))
        , m_timestamp(std::move(timestamp)) {}

    virtual ~DataPointResult()                         = default;
    DataPointResult(const DataPointResult&)            = default;
    DataPointResult(DataPointResult&&)                 = default;
    DataPointResult& operator=(const DataPointResult&) = default;
    DataPointResult& operator=(DataPointResult&&)      = default;

    [[nodiscard]] const std::string& getPath() const { return m_path; }
    [[nodiscard]] const Timestamp&   getTimestamp() const { return m_timestamp; }
    [[nodiscard]] Type               getType() const { return m_type; }

    bool operator==(const DataPointResult& other) const {
        return std::tie(m_path, m_timestamp, m_type) ==
               std::tie(other.m_path, other.m_timestamp, other.m_type);
    }

private:
    std::string m_path;
    Timestamp   m_timestamp{};
    Type        m_type{Type::INVALID};
};

template <typename T> DataPointResult::Type getValueType() {
    static_assert(std::is_same<T, std::false_type>::value, "Value type not supported!");
    return DataPointResult::Type::INVALID;
}

template <> inline DataPointResult::Type getValueType<bool>() {
    return DataPointResult::Type::BOOL;
}
template <> inline DataPointResult::Type getValueType<std::vector<bool>>() {
    return DataPointResult::Type::BOOL_ARRAY;
}
template <> inline DataPointResult::Type getValueType<int32_t>() {
    return DataPointResult::Type::INT32;
}
template <> inline DataPointResult::Type getValueType<std::vector<int32_t>>() {
    return DataPointResult::Type::INT32_ARRAY;
}
template <> inline DataPointResult::Type getValueType<int64_t>() {
    return DataPointResult::Type::INT64;
}
template <> inline DataPointResult::Type getValueType<std::vector<int64_t>>() {
    return DataPointResult::Type::INT64_ARRAY;
}
template <> inline DataPointResult::Type getValueType<uint32_t>() {
    return DataPointResult::Type::UINT32;
}
template <> inline DataPointResult::Type getValueType<std::vector<uint32_t>>() {
    return DataPointResult::Type::UINT32_ARRAY;
}
template <> inline DataPointResult::Type getValueType<uint64_t>() {
    return DataPointResult::Type::UINT64;
}
template <> inline DataPointResult::Type getValueType<std::vector<uint64_t>>() {
    return DataPointResult::Type::UINT64_ARRAY;
}
template <> inline DataPointResult::Type getValueType<float>() {
    return DataPointResult::Type::FLOAT;
}
template <> inline DataPointResult::Type getValueType<std::vector<float>>() {
    return DataPointResult::Type::FLOAT_ARRAY;
}
template <> inline DataPointResult::Type getValueType<double>() {
    return DataPointResult::Type::DOUBLE;
}
template <> inline DataPointResult::Type getValueType<std::vector<double>>() {
    return DataPointResult::Type::DOUBLE_ARRAY;
}
template <> inline DataPointResult::Type getValueType<std::string>() {
    return DataPointResult::Type::STRING;
}
template <> inline DataPointResult::Type getValueType<std::vector<std::string>>() {
    return DataPointResult::Type::STRING_ARRAY;
}

template <typename T> class TypedDataPointResult : public DataPointResult {
public:
    TypedDataPointResult()
        : DataPointResult(Type::INVALID, "", Timestamp{}){};

    TypedDataPointResult(std::string path, T value)
        : DataPointResult(getValueType<T>(), std::forward<decltype(path)>(path), Timestamp{})
        , m_value(std::move(value)) {}

    TypedDataPointResult(std::string path, T value, Timestamp timestamp)
        : DataPointResult(getValueType<T>(), std::forward<decltype(path)>(path),
                          std::forward<decltype(timestamp)>(timestamp))
        , m_value(std::move(value)) {}

    [[nodiscard]] const T& value() const { return m_value; }

    bool operator==(const TypedDataPointResult& other) const {
        return DataPointResult::operator==(other) && m_value == other.m_value;
    }

private:
    T m_value;
};

using DataPointMap_t = std::map<std::string, std::shared_ptr<DataPointResult>>;

/**
 * @brief Result of an operation which returns multiple data points.
 *        Provides typed access to obtained data points.
 *
 */
class DataPointsResult final {
public:
    DataPointsResult() = default;

    DataPointsResult(DataPointMap_t&& dataPointsMap)
        : m_dataPointsMap(std::move(dataPointsMap)){};

    /**
     * @brief Get the desired data point from the result.
     *
     * @tparam TDataPointType   The type of the data point to return.
     * @param dataPoint         The data point to query from the result.
     * @return std::shared_ptr<TDataPointType>  The data point contained in the result.
     */
    template <class TDataPointType>
    [[nodiscard]] std::shared_ptr<TypedDataPointResult<typename TDataPointType::value_type>>
    get(const TDataPointType& dataPoint) const {
        static_assert(std::is_base_of_v<DataPoint, TDataPointType>);

        if (m_dataPointsMap.find(dataPoint.getPath()) == m_dataPointsMap.end()) {
            throw InvalidValueException(
                fmt::format("{} is not contained in result!", dataPoint.getPath()));
        }

        std::shared_ptr<DataPointResult> result = m_dataPointsMap.at(dataPoint.getPath());
        /*
        if (!result->isValid()) {
            throw InvalidValueException(fmt::format("{} is invalid: {}!", result->getPath(),
                                                    result->asFailure().getReason()));
        }*/

        return std::dynamic_pointer_cast<TypedDataPointResult<typename TDataPointType::value_type>>(
            result);
        ;
    }

    /**
     * @brief Check if the result is empty.
     *
     * @return true   Result is empty.
     * @return false  Result is not empty.
     */
    bool empty() { return m_dataPointsMap.empty(); }

private:
    DataPointMap_t m_dataPointsMap;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_DATAPOINTSRESULT_H
