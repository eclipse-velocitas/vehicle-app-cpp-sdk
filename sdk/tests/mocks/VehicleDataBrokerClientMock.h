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

#ifndef VEHICLE_APP_SDK_VEHICLEDATABROKERCLIENTMOCK_H
#define VEHICLE_APP_SDK_VEHICLEDATABROKERCLIENTMOCK_H

#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <gmock/gmock.h>

namespace velocitas {

class VehicleDataBrokerClientMock : public IVehicleDataBrokerClient {
public:
    MOCK_METHOD(AsyncResultPtr_t<DataPointReply>, getDatapoints,
                (const std::vector<std::string>& datapoints));

    MOCK_METHOD(AsyncResultPtr_t<IVehicleDataBrokerClient::SetErrorMap_t>, setDatapoints,
                (const std::vector<std::unique_ptr<DataPointValue>>& datapoints));

    MOCK_METHOD(AsyncSubscriptionPtr_t<DataPointReply>, subscribe, (const std::string& query));
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_VEHICLEDATABROKERCLIENTMOCK_H
