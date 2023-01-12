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

#include "sdk/DataPointBatch.h"
#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/QueryBuilder.h"
#include "sdk/VehicleApp.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"
#include "vehicle_model/Vehicle.h"

#include <fmt/core.h>
#include <nlohmann/json.hpp>

namespace example {

class SetDataPointsApp : public velocitas::VehicleApp {
public:
    SetDataPointsApp()
        : VehicleApp(
              velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
              velocitas::IPubSubClient::createInstance("localhost:1883", "SetDataPointsApp")) {}

    void onStart() override {
        velocitas::logger().info("Setting data points!");

        // set multiple data points at the same time
        Vehicle.setMany()
            .add(Vehicle.Cabin.Seat.Row1.Pos1.Position, 1000)
            .add(Vehicle.Cabin.Seat.Row1.Pos2.Position, 1000)
            .apply()
            ->await();

        // set a single data point
        Vehicle.Speed.set(100.0F)->await();

        // get a single data point
        Vehicle.Cabin.Seat.Row1.Pos1.Position.get()->await();
    }

private:
    velocitas::Vehicle Vehicle;
};

} // namespace example

int main(int argc, char** argv) {
    example::SetDataPointsApp myApp;
    myApp.run();
    return 0;
}
