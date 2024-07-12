/**
 * Copyright (c) 2022-2024 Contributors to the Eclipse Foundation
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
#include "vehicle/Vehicle.h"

#include <csignal>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

namespace example {

class SetDataPointsApp : public velocitas::VehicleApp {
public:
    SetDataPointsApp()
        : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                     velocitas::IPubSubClient::createInstance("SetDataPointsApp")) {}

    void onStart() override {
        // set a single data point
        try {
            velocitas::logger().info("Setting single data point ...");

            Vehicle.Speed.set(100.0F)->await();

            velocitas::logger().info("Setting single data point successfully done.");
        } catch (velocitas::AsyncException& e) {
            velocitas::logger().error("Error on setting single data point: {}", e.what());
        }

        // set multiple data points at the same time
        try {
            velocitas::logger().info("Setting batch of data points ...");

            auto result = Vehicle.setMany()
                              .add(Vehicle.Cabin.Seat.Row1.DriverSide.Position, 1000)
                              .add(Vehicle.Cabin.Seat.Row1.PassengerSide.Position, 1000)
                              .apply()
                              ->await();

            if (result.empty()) {
                velocitas::logger().info("Setting batch of data points successfully done.");
            } else {
                velocitas::logger().error("Some data points of batch could not be set:");
                for (auto datapointError : result) {
                    velocitas::logger().error("    '{}' -> {}", datapointError.first,
                                              datapointError.second);
                }
            }
        } catch (velocitas::AsyncException& e) {
            velocitas::logger().error("Error on setting batch of data points: {}", e.what());
        }

        velocitas::logger().info("Done. (Press Ctrl+C to terminate the app.)");
    }

private:
    velocitas::Vehicle Vehicle;
};

} // namespace example

std::unique_ptr<example::SetDataPointsApp> myApp;

void signal_handler(int sig) {
    velocitas::logger().info("App terminating signal received: {}", sig);
    myApp->stop();
}

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);

    myApp = std::make_unique<example::SetDataPointsApp>();
    myApp->run();
    return 0;
}
