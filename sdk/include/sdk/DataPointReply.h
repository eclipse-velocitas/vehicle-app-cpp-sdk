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

#ifndef VEHICLE_APP_SDK_DATAPOINTREPLY_H
#define VEHICLE_APP_SDK_DATAPOINTREPLY_H

#include "sdk/DataPointValue.h"
#include "sdk/Exceptions.h"

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace velocitas {

using DataPointMap_t = std::map<std::string, std::shared_ptr<DataPointValue>>;

class DataPoint;

/**
 * @brief Result of an operation which returns multiple data points.
 *        Provides typed access to obtained data points.
 *
 */
class DataPointReply final {
public:
    DataPointReply() = default;

    DataPointReply(DataPointMap_t&& dataPointsMap)
        : m_dataPointsMap(std::move(dataPointsMap)) {}

    /**
     * @brief Get the desired data point from the reply as an untyped DataPointValue.
     *
     * @param path The path ("name") of the data point to query from the reply.
     * @return std::shared_ptr<DataPointValue>  The genric data point value contained in the reply.
     */
    [[nodiscard]] std::shared_ptr<DataPointValue> getUntyped(const std::string& path) const {
        auto mapEntry = m_dataPointsMap.find(path);
        if (mapEntry == m_dataPointsMap.end()) {
            throw InvalidValueException(path + " is not contained in reply!");
        }
        return mapEntry->second;
    }

    /**
     * @brief Get the desired data point from the reply.
     *
     * @tparam TDataPointType   The type of the data point to return.
     * @param dataPoint         The data point to query from the reply.
     * @return std::shared_ptr<TDataPointType>  The data point value contained in the reply.
     */
    template <class TDataPointType>
    [[nodiscard]] std::shared_ptr<TypedDataPointValue<typename TDataPointType::value_type>>
    get(const TDataPointType& dataPoint) const {
        static_assert(std::is_base_of_v<DataPoint, TDataPointType>);

        auto value = getUntyped(dataPoint.getPath());
        if (value->isValid()) {
            return std::dynamic_pointer_cast<
                TypedDataPointValue<typename TDataPointType::value_type>>(value);
        }

        return std::make_shared<TypedDataPointValue<typename TDataPointType::value_type>>(
            value->getPath(), value->getFailure(), value->getTimestamp());
    }

    /**
     * @brief Check if the reply is empty.
     *
     * @return true   Reply is empty.
     * @return false  Reply is not empty.
     */
    [[nodiscard]] bool empty() const { return m_dataPointsMap.empty(); }

private:
    DataPointMap_t m_dataPointsMap;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_DATAPOINTREPLY_H
