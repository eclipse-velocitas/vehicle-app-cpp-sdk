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

#ifndef VEHICLE_APP_SDK_DATAPOINT_H
#define VEHICLE_APP_SDK_DATAPOINT_H

#include "sdk/AsyncResult.h"
#include "sdk/DataPointValue.h"
#include "sdk/Node.h"

#include <cstdint>
#include <string>
#include <vector>

namespace velocitas {

/**
 * @brief Base class for data points.
 *        Do not use for modelling directly.
 *
 * @details Provides as*Type*() methods to convert the base data point into any of its derived
 * types. If the type is not the one you are treating it as (i.e. using asInt32() on a bool type) an
 * InvalidTypeException is thrown at runtime.
 */
class DataPoint : public Node {
public:
    using Node::Node;

    explicit DataPoint(const std::string& name);
    ~DataPoint() override = default;

    DataPoint(const DataPoint&)            = delete;
    DataPoint(DataPoint&&)                 = delete;
    DataPoint& operator=(const DataPoint&) = delete;
    DataPoint& operator=(DataPoint&&)      = delete;

    [[nodiscard]] virtual std::string toString() const = 0;

    bool operator<(const DataPoint& rhs) const { return getPath() < rhs.getPath(); }
};

inline bool operator<(const std::reference_wrapper<DataPoint>& lhs,
                      const std::reference_wrapper<DataPoint>& rhs) {
    return lhs.get().getPath() < rhs.get().getPath();
}

/**
 * @brief A typed data point template.
 *
 */
template <typename T> class TypedDataPoint : public DataPoint {
public:
    using DataPoint::DataPoint;

    using value_type = T;

    ~TypedDataPoint() override = default;

    TypedDataPoint(const TypedDataPoint&)            = delete;
    TypedDataPoint(TypedDataPoint&&)                 = delete;
    TypedDataPoint& operator=(const TypedDataPoint&) = delete;
    TypedDataPoint& operator=(TypedDataPoint&&)      = delete;

    [[nodiscard]] AsyncResultPtr_t<TypedDataPointValue<T>> get() const;
    [[nodiscard]] AsyncResultPtr_t<Status>                 set(T value);

    [[nodiscard]] std::string toString() const override;
};

using DataPointBoolean      = TypedDataPoint<bool>;
using DataPointBooleanArray = TypedDataPoint<std::vector<bool>>;
using DataPointInt32        = TypedDataPoint<int32_t>;
using DataPointInt32Array   = TypedDataPoint<std::vector<int32_t>>;
using DataPointInt64        = TypedDataPoint<int64_t>;
using DataPointInt64Array   = TypedDataPoint<std::vector<int64_t>>;
using DataPointUint32       = TypedDataPoint<uint32_t>;
using DataPointUint32Array  = TypedDataPoint<std::vector<uint32_t>>;
using DataPointUint64       = TypedDataPoint<uint64_t>;
using DataPointUint64Array  = TypedDataPoint<std::vector<uint64_t>>;
using DataPointFloat        = TypedDataPoint<float>;
using DataPointFloatArray   = TypedDataPoint<std::vector<float>>;
using DataPointDouble       = TypedDataPoint<double>;
using DataPointDoubleArray  = TypedDataPoint<std::vector<double>>;
using DataPointString       = TypedDataPoint<std::string>;
using DataPointStringArray  = TypedDataPoint<std::vector<std::string>>;

} // namespace velocitas

#endif // VEHICLE_APP_SDK_DATAPOINT_H
