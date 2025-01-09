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

#include "sdk/IPubSubClient.h"
#include "sdk/VehicleApp.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <memory>

int main() {
    const auto vehicleApp = std::make_unique<velocitas::VehicleApp>(
        velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
        velocitas::IPubSubClient::createInstance("SetDataPointsApp"));
    vehicleApp->onStart();
    vehicleApp->onStop();
    return 0;
}
