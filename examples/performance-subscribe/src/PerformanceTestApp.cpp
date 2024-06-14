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
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <csignal>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

namespace example {

namespace {
std::string getValueRepresentation(const velocitas::DataPointValue& value) {
    if (!value.isValid()) {
        return toString(value.getFailure());
    }
    return value.getValueAsString();
}

std::ostream& operator<<(std::ostream&                                        ostr,
                         const std::chrono::high_resolution_clock::time_point timestamp) {
    std::time_t time = std::chrono::high_resolution_clock::to_time_t(timestamp);
    uint64_t    useconds =
        std::chrono::duration_cast<std::chrono::microseconds>(timestamp.time_since_epoch())
            .count() %
        std::micro::den;
    ostr << std::put_time(std::localtime(&time), "%T") << "." << std::setw(6) << std::setfill('0')
         << useconds;
    return ostr;
}

std::vector<std::string> readSignalNames(const std::string& configFile) {
    velocitas::logger().info("Reading signal list from file {}.", configFile);
    auto config     = nlohmann::json::parse(std::ifstream(configFile));
    auto signalList = config["signals"];

    std::vector<std::string> signalNames;
    signalNames.reserve(signalList.size());
    for (const auto& signal : signalList) {
        const auto& signalName = signal["path"];
        if (!signalName.empty()) {
            velocitas::logger().debug("    {}", signalName);
            signalNames.push_back(signalName);
        }
    }
    return signalNames;
}

} // anonymous namespace

PerformanceTestApp::PerformanceTestApp(std::string configFile)
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                 velocitas::IPubSubClient::createInstance("PerformanceTestApp"))
    , m_configFile{std::move(configFile)} {}

void PerformanceTestApp::onStart() {
    auto dataPointList = readSignalNames(m_configFile);
    velocitas::logger().info("Subscribing to signals ...");
    for (auto path : dataPointList) {
        subscribeDataPoints("SELECT " + path)
            ->onItem([path](const velocitas::DataPointReply& reply) {
                auto value     = getValueRepresentation(*reply.getGeneric(path));
                auto timestamp = std::chrono::high_resolution_clock::now();
                std::cout << timestamp << " - " << path << " - " << value << std::endl;
            })
            ->onError([path](const velocitas::Status& status) {
                velocitas::logger().error("Error on subscription for data point {}: {}", path,
                                          status.errorMessage());
            });
    }
}

} // namespace example

namespace {

std::unique_ptr<example::PerformanceTestApp> myApp;

void signal_handler(int sig) {
    velocitas::logger().info("App terminating signal received: {}", sig);
    myApp->stop();
}

const char DEFAULT_CONFIG_FILE[] = "examples/performance-subscribe/subscription_signals.json";

} // namespace

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);

    const auto* configFile = (argc > 1) ? argv[1] : DEFAULT_CONFIG_FILE;

    myApp = std::make_unique<example::PerformanceTestApp>(configFile);
    myApp->run();
    return 0;
}
