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

#ifndef VEHICLE_APP_SDK_VEHICLEDATABROKERCLIENT_H
#define VEHICLE_APP_SDK_VEHICLEDATABROKERCLIENT_H

#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <memory>
#include <vector>

namespace velocitas {

class RecurringJob;
class BrokerAsyncGrpcFacade;

/**
 * VehicleDataBrokerClient provides the Graph API to access vehicle services
 * and vehicle signals.
 */
class VehicleDataBrokerClient : public IVehicleDataBrokerClient {
public:
    explicit VehicleDataBrokerClient(const std::string& vdbAddress, std::string vdbAppId);
    explicit VehicleDataBrokerClient(const std::string& vdbAppId);

    ~VehicleDataBrokerClient() override;

    VehicleDataBrokerClient(const VehicleDataBrokerClient&)            = delete;
    VehicleDataBrokerClient(VehicleDataBrokerClient&&)                 = delete;
    VehicleDataBrokerClient& operator=(const VehicleDataBrokerClient&) = delete;
    VehicleDataBrokerClient& operator=(VehicleDataBrokerClient&&)      = delete;

    AsyncResultPtr_t<DataPointValues>
    getDatapoints(const std::vector<std::string>& datapoints) override;

    AsyncResultPtr_t<SetErrorMap_t>
    setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) override;

    AsyncSubscriptionPtr_t<DataPointValues> subscribe(const std::string& query) override;

private:
    static std::string getVdbEndpointAddress();

    std::shared_ptr<BrokerAsyncGrpcFacade> m_asyncBrokerFacade;
    std::string                            m_vdbAppId;
};
} // namespace velocitas

#endif // VEHICLE_APP_SDK_VEHICLEDATABROKERCLIENT_H
