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

#ifndef VEHICLE_APP_SDK_IVEHICLEDATABROKERCLIENT_H
#define VEHICLE_APP_SDK_IVEHICLEDATABROKERCLIENT_H

#include "sdk/AsyncResult.h"
#include "sdk/DataPointReply.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace velocitas {

class DataPointReply;
class DataPointValue;

/**
 * @brief Interface for implementing VehicleDataBroker clients.
 *
 */
class IVehicleDataBrokerClient {
public:
    using SetErrorMap_t = std::map<std::string, std::string>;

    virtual ~IVehicleDataBrokerClient() = default;

    IVehicleDataBrokerClient(const IVehicleDataBrokerClient&)            = delete;
    IVehicleDataBrokerClient(IVehicleDataBrokerClient&&)                 = delete;
    IVehicleDataBrokerClient& operator=(const IVehicleDataBrokerClient&) = delete;
    IVehicleDataBrokerClient& operator=(IVehicleDataBrokerClient&&)      = delete;

    /**
     * @brief Returns data points for a list of data point paths from the VDB.
     *
     * @param datapoints The list of data point paths to query.
     *
     * @return The AsyncResult containing the values of all requested data points
     */
    virtual AsyncResultPtr_t<DataPointReply>
    getDatapoints(const std::vector<std::string>& datapoints) = 0;

    /**
     * @brief Set datapoint values in the VDB.
     *
     * @return AsyncResultPtr_t<SetErrorMap_t> A map which contains [key, error] entries
     * if a data point could not be set.
     */
    virtual AsyncResultPtr_t<SetErrorMap_t>
    setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) = 0;

    /**
     * @brief Subscribe to updates for the given query.
     *
     * @param query The query to subscribe to.
     *
     * @return The subscription to the data points.
     */
    virtual AsyncSubscriptionPtr_t<DataPointReply> subscribe(const std::string& query) = 0;

    /**
     * @brief Create an instance of the IVehicleDataBrokerClient.
     *
     * @param serviceName  The name of the VDB.
     *
     * @return std::shared_ptr<IVehicleDataBrokerClient>
     */
    static std::shared_ptr<IVehicleDataBrokerClient> createInstance(const std::string& serviceName);

protected:
    IVehicleDataBrokerClient() = default;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_IVEHICLEDATABROKERCLIENT_H
