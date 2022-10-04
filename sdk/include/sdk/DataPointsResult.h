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
#include <string>

namespace velocitas {

class DataPoint;

using DataPointMap_t = std::map<std::string, std::shared_ptr<DataPoint>>;

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
    [[nodiscard]] std::shared_ptr<TDataPointType> get(const TDataPointType& dataPoint) const {
        static_assert(std::is_base_of_v<DataPoint, TDataPointType>);

        if (m_dataPointsMap.find(dataPoint.getPath()) == m_dataPointsMap.end()) {
            throw InvalidValueException(
                fmt::format("{} is not contained in result!", dataPoint.getPath()));
        }

        auto result = m_dataPointsMap.at(dataPoint.getPath());
        return std::dynamic_pointer_cast<TDataPointType>(result);
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
