/**
 * Copyright (c) 2024-2025 Contributors to the Eclipse Foundation
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

#include "BrokerAsyncGrpcFacade.h"
#include "Metadata.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <memory>
#include <string>

namespace velocitas {

class GrpcClient;

namespace kuksa_val_v2 {

/**
 * Provides the Graph API to access vehicle signals via the kuksa.val.v2 API
 */
class BrokerClient : public IVehicleDataBrokerClient {
public:
    BrokerClient(const std::string& vdbAddress, const std::string& vdbServiceName);
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
    void onGetValuesResponse(const kuksa::val::v2::GetValuesResponse& response,
                             const MetadataList_t& metadataList, size_t numRequestedSignals,
                             const AsyncResultPtr_t<DataPointReply>& result);
    void onGetValuesError(const grpc::Status& status, const MetadataList_t& metadataList,
                          const AsyncResultPtr_t<DataPointReply>& result);

    std::shared_ptr<BrokerAsyncGrpcFacade> m_asyncBrokerFacade;
    std::shared_ptr<MetadataAgent>         m_metadataAgent;
    std::unique_ptr<GrpcClient>            m_activeCalls;
};

} // namespace kuksa_val_v2
} // namespace velocitas

#endif // VEHICLE_APP_SDK_VDB_GRPC_KUKSA_VAL_V2_BROKERCLIENT_H
