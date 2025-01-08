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

#ifndef VEHICLE_APP_SDK_DATAPOINTBATCH_H
#define VEHICLE_APP_SDK_DATAPOINTBATCH_H

#include "sdk/AsyncResult.h"
#include "sdk/DataPointValue.h"

#include <map>
#include <vector>

namespace velocitas {

/**
 * @brief Batch implementation for setting multiple data points
 * atomically inside the VDB.
 *
 */
class DataPointBatch final {
public:
    using SetErrorMap_t = std::map<std::string, std::string>;

    DataPointBatch() = default;

    /**
     * @brief Add a data point and its new value to the batch.
     * It does **not** set the value yet!
     *
     * @tparam TDataPoint       The type of the data point.
     * @param dataPoint         The reference to the data point to set.
     * @param value             The new value to set.
     * @return DataPointBatch&  A reference to the batch for method chaining.
     */
    template <typename TDataPoint>
    DataPointBatch& add(const TDataPoint& dataPoint, typename TDataPoint::value_type value) {
        m_dataPoints.emplace_back(
            std::make_unique<TypedDataPointValue<typename TDataPoint::value_type>>(
                dataPoint.getPath(), value, Timestamp{}));

        return *this;
    }

    /**
     * @brief Applies all data points and their values by invoking an atomic set
     * operation.
     *
     * @pre At least one point was added via @ref DataPointBatch:add
     *
     * @return AsyncResultPtr_t<SetErrorMap_t> The async result of the
     * operation. Contains a map of [dataPointPath, error] if any of the added data points
     * caused an error during set.
     */
    AsyncResultPtr_t<SetErrorMap_t> apply();

private:
    std::vector<std::unique_ptr<DataPointValue>> m_dataPoints;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_DATAPOINTBATCH_H
