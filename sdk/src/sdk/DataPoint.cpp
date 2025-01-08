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

#include "sdk/DataPoint.h"

#include "sdk/VehicleModelContext.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <fmt/core.h>

#include <memory>
#include <utility>

namespace velocitas {

template <typename T> AsyncResultPtr_t<TypedDataPointValue<T>> TypedDataPoint<T>::get() const {
    return VehicleModelContext::getInstance()
        .getVdbc()
        ->getDatapoints({getPath()})
        ->map<TypedDataPointValue<T>>(
            [this](const DataPointReply& dataPointValues) { return *dataPointValues.get(*this); });
}

template <typename T> AsyncResultPtr_t<Status> TypedDataPoint<T>::set(T value) const {
    std::vector<std::unique_ptr<DataPointValue>> vec;
    vec.reserve(1);
    vec.emplace_back(std::make_unique<TypedDataPointValue<T>>(getPath(), value));
    return VehicleModelContext::getInstance().getVdbc()->setDatapoints(vec)->map<Status>(
        [this](auto errorMap) {
            const auto iter = errorMap.find(this->getPath());
            if (iter == errorMap.end()) {
                return Status();
            }
            return Status(iter->second);
        });
}

template <> std::string TypedDataPoint<bool>::toString() const {
    return fmt::format("DataPointBoolean: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<bool>>::toString() const {
    return fmt::format("DataPointBooleanArray: ('{}')", getName());
}

template <> std::string TypedDataPoint<int8_t>::toString() const {
    return fmt::format("DataPointInt8: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<int8_t>>::toString() const {
    return fmt::format("DataPointInt8Array: ('{}')", getName());
}

template <> std::string TypedDataPoint<int16_t>::toString() const {
    return fmt::format("DataPointInt16: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<int16_t>>::toString() const {
    return fmt::format("DataPointInt16Array: ('{}' : '{}')", getName());
}

template <> std::string TypedDataPoint<int32_t>::toString() const {
    return fmt::format("DataPointInt32: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<int32_t>>::toString() const {
    return fmt::format("DataPointInt32Array: ('{}')", getName());
}

template <> std::string TypedDataPoint<int64_t>::toString() const {
    return fmt::format("DataPointInt64: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<int64_t>>::toString() const {
    return fmt::format("DataPointInt64Array: ('{}' : '{}')", getName());
}

template <> std::string TypedDataPoint<uint8_t>::toString() const {
    return fmt::format("DataPointUint8: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<uint8_t>>::toString() const {
    return fmt::format("DataPointUint8Array: ('{}')", getName());
}

template <> std::string TypedDataPoint<uint16_t>::toString() const {
    return fmt::format("DataPointUint16: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<uint16_t>>::toString() const {
    return fmt::format("DataPointUint16Array: ('{}' : '{}')", getName());
}

template <> std::string TypedDataPoint<uint32_t>::toString() const {
    return fmt::format("DataPointUint32: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<uint32_t>>::toString() const {
    return fmt::format("DataPointUint32Array: ('{}')", getName());
}

template <> std::string TypedDataPoint<uint64_t>::toString() const {
    return fmt::format("DataPointUint64: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<uint64_t>>::toString() const {
    return fmt::format("DataPointUint64Array: ('{}')", getName());
}

template <> std::string TypedDataPoint<float>::toString() const {
    return fmt::format("DataPointFloat: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<float>>::toString() const {
    return fmt::format("DataPointFloatArray: ('{}')", getName());
}

template <> std::string TypedDataPoint<double>::toString() const {
    return fmt::format("DataPointDouble: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<double>>::toString() const {
    return fmt::format("DataPointDoubleArray: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::string>::toString() const {
    return fmt::format("DataPointString: ('{}')", getName());
}

template <> std::string TypedDataPoint<std::vector<std::string>>::toString() const {
    return fmt::format("DataPointStringArray: ('{}')", getName());
}

template class TypedDataPoint<bool>;
template class TypedDataPoint<std::vector<bool>>;
template class TypedDataPoint<int8_t>;
template class TypedDataPoint<std::vector<int8_t>>;
template class TypedDataPoint<int16_t>;
template class TypedDataPoint<std::vector<int16_t>>;
template class TypedDataPoint<int32_t>;
template class TypedDataPoint<std::vector<int32_t>>;
template class TypedDataPoint<int64_t>;
template class TypedDataPoint<std::vector<int64_t>>;
template class TypedDataPoint<uint8_t>;
template class TypedDataPoint<std::vector<uint8_t>>;
template class TypedDataPoint<uint16_t>;
template class TypedDataPoint<std::vector<uint16_t>>;
template class TypedDataPoint<uint32_t>;
template class TypedDataPoint<std::vector<uint32_t>>;
template class TypedDataPoint<uint64_t>;
template class TypedDataPoint<std::vector<uint64_t>>;
template class TypedDataPoint<float>;
template class TypedDataPoint<std::vector<float>>;
template class TypedDataPoint<double>;
template class TypedDataPoint<std::vector<double>>;
template class TypedDataPoint<std::string>;
template class TypedDataPoint<std::vector<std::string>>;

} // namespace velocitas
