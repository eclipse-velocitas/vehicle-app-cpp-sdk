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

#include "PerformanceTestApp.h"
#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/QueryBuilder.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <csignal>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

namespace example {

const auto JSON_FIELD_REQUEST_ID = "requestId";
const auto JSON_FIELD_POSITION   = "position";
const auto JSON_FIELD_STATUS     = "status";
const auto JSON_FIELD_MESSAGE    = "message";
const auto JSON_FIELD_RESULT     = "result";

PerformanceTestApp::PerformanceTestApp()
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                 velocitas::IPubSubClient::createInstance("PerformanceTestApp"))
    , m_vehicleModel(std::make_shared<velocitas::Vehicle>())
    , m_something{"Vehicle.Something"}
    , m_speed{"Vehicle.Speed"} {}

void PerformanceTestApp::onStart() {
    velocitas::logger().info("Subscribe for data points!");

    std::vector dataPointList{"Speed", "Something"};
    for (auto dataPointName : dataPointList) {
        velocitas::DataPoint* dataPoint = m_vehicleModel->getDataPoint(dataPointName);
        if (dataPoint) {
            auto path = dataPoint->getPath();
            subscribeDataPoints(velocitas::QueryBuilder::select(*dataPoint).build())
                ->onItem([dataPoint](const velocitas::DataPointReply& reply) {
                    auto timestamp = std::chrono::high_resolution_clock::now();
                    velocitas::logger().info("{} - {} - {}", timestamp, dataPoint->getPath(),
                                             reply.get(*dataPoint)->value());
                })
                ->onError([path](auto&& status) {
                    velocitas::logger().error("Error on subscription for data point {}: {}", path,
                                              status);
                });
        } else {
            velocitas::logger().error("Undefined data point: {}", dataPointName);
        }
    }
}

void PerformanceTestApp::onDataPointUpdate(const velocitas::DataPointReply& dataPoints) {
    velocitas::logger().info("Data point update received.");
    if (dataPoints.get(m_speed)->isValid()) {
        velocitas::logger().info("    Data point speed = {}", dataPoints.get(m_speed)->value());
    }
    if (dataPoints.get(m_something)->isValid()) {
        velocitas::logger().info("    Data point something = {}",
                                 dataPoints.get(m_something)->value());
    }
}

void PerformanceTestApp::onDataPointError(const velocitas::Status& status) {
    velocitas::logger().error("Data point: Error occurred during async invocation: {}",
                              status.errorMessage());
}

} // namespace example

std::unique_ptr<example::PerformanceTestApp> myApp;

void signal_handler(int sig) {
    velocitas::logger().info("App terminating signal received: {}", sig);
    myApp->stop();
}

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);

    myApp = std::make_unique<example::PerformanceTestApp>();
    myApp->run();
    return 0;
}
