/**
 * Copyright (c) 2024 Contributors to the Eclipse Foundation
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

#ifndef VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERCLIENT_H
#define VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERCLIENT_H

#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <memory>

namespace velocitas::kuksa_val_v2 {

class BrokerAsyncGrpcFacade;

/**
 * Provides the Graph API to access vehicle signals via the kuksa.val.v2 API
 */
class BrokerClient : public IVehicleDataBrokerClient {
public:
    explicit BrokerClient(const std::string& vdbAddress, std::string vdbServiceName);
    explicit BrokerClient(const std::string& vdbserviceName);

    ~BrokerClient() override;

    BrokerClient(const BrokerClient&)            = delete;
    BrokerClient(BrokerClient&&)                 = delete;
    BrokerClient& operator=(const BrokerClient&) = delete;
    BrokerClient& operator=(BrokerClient&&)      = delete;

    AsyncResultPtr_t<DataPointReply>
    getDatapoints(const std::vector<std::string>& datapoints) override;

    AsyncResultPtr_t<SetErrorMap_t>
    setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) override;

    AsyncSubscriptionPtr_t<DataPointReply> subscribe(const std::string& query) override;

private:
    std::shared_ptr<BrokerAsyncGrpcFacade> m_asyncBrokerFacade;
};

} // namespace velocitas::kuksa_val_v2

#endif // VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERCLIENT_H
