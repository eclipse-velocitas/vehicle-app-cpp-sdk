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

#ifndef VEHICLE_APP_SDK_KUKSAVALV2CLIENT_H
#define VEHICLE_APP_SDK_KUKSAVALV2CLIENT_H

#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <memory>

namespace velocitas {

class RecurringJob;
class KuksaValV2AsyncGrpcFacade;

/**
 * Provides the Graph API to access vehicle signals via the kuksa.val.v2 API
 */
class KuksaValV2Client : public IVehicleDataBrokerClient {
public:
    explicit KuksaValV2Client(const std::string& vdbAddress, std::string vdbServiceName);
    explicit KuksaValV2Client(const std::string& vdbserviceName);

    ~KuksaValV2Client() override;

    KuksaValV2Client(const KuksaValV2Client&)            = delete;
    KuksaValV2Client(KuksaValV2Client&&)                 = delete;
    KuksaValV2Client& operator=(const KuksaValV2Client&) = delete;
    KuksaValV2Client& operator=(KuksaValV2Client&&)      = delete;

    AsyncResultPtr_t<DataPointReply>
    getDatapoints(const std::vector<std::string>& datapoints) override;

    AsyncResultPtr_t<SetErrorMap_t>
    setDatapoints(const std::vector<std::unique_ptr<DataPointValue>>& datapoints) override;

    AsyncSubscriptionPtr_t<DataPointReply> subscribe(const std::string& query) override;

private:
    std::shared_ptr<KuksaValV2AsyncGrpcFacade> m_asyncBrokerFacade;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_KUKSAVALV2CLIENT_H
