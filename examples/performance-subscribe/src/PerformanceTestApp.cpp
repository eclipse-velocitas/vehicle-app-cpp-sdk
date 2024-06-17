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

#include "PerformanceTestApp.h"
#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/vdb/IVehicleDataBrokerClient.h"

#include <csignal>
#include <filesystem>
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

int32_t extractMicroseconds(const std::chrono::high_resolution_clock::time_point& timepoint) {
    const auto timeSinceEpoch = timepoint.time_since_epoch();
    const auto usecondsSinceEpoch =
        std::chrono::duration_cast<std::chrono::microseconds>(timeSinceEpoch).count();
    return static_cast<int32_t>(usecondsSinceEpoch % std::micro::den);
}

std::ostream& operator<<(std::ostream&                                         ostr,
                         const std::chrono::high_resolution_clock::time_point& timestamp) {
    auto time = std::chrono::high_resolution_clock::to_time_t(timestamp);
    ostr << std::put_time(std::localtime(&time), "%T");
    const int32_t microseconds         = extractMicroseconds(timestamp);
    const int     NUM_AFTER_DOT_DIGITS = 6;
    ostr << "." << std::setw(NUM_AFTER_DOT_DIGITS) << std::setfill('0') << microseconds;
    return ostr;
}

} // anonymous namespace

PerformanceTestApp::PerformanceTestApp(std::vector<std::string> signalList)
    : VehicleApp(velocitas::IVehicleDataBrokerClient::createInstance("vehicledatabroker"),
                 velocitas::IPubSubClient::createInstance("PerformanceTestApp"))
    , m_signalList{std::move(signalList)} {}

void PerformanceTestApp::onStart() {
    velocitas::logger().info("Subscribing to signals ...");
    for (auto path : m_signalList) {
        subscribeDataPoints("SELECT " + path)
            ->onItem([path](const velocitas::DataPointReply& reply) {
                auto value     = getValueRepresentation(*reply.getUntyped(path));
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

const char DEFAULT_CONFIG_FILENAME[] = "subscription_signals.json";

std::string getDefaultConfigFilePath(const char* appBinaryPath) {
    std::filesystem::path path = appBinaryPath;
    path.replace_filename(DEFAULT_CONFIG_FILENAME);
    return std::string{path};
}

std::vector<std::string> readSignalNameFromFiles(const std::string& configFile) {
    velocitas::logger().info("Reading signal list from file {}.", configFile);
    const auto  config     = nlohmann::json::parse(std::ifstream(configFile));
    const auto& signalList = config["signals"];

    std::vector<std::string> signalNames;
    signalNames.reserve(signalList.size());
    for (const auto& signal : signalList) {
        const auto& signalName = signal["path"];
        if (!signalName.empty()) {
            velocitas::logger().debug("    {}", signalName);
            signalNames.push_back(signalName);
        } else {
            velocitas::logger().warn("Signal entry without 'path' found!");
        }
    }
    return signalNames;
}

} // anonymous namespace

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);

    const auto configFile = (argc > 1) ? std::string(argv[1]) : getDefaultConfigFilePath(argv[0]);
    auto       signalList = readSignalNameFromFiles(configFile);

    myApp = std::make_unique<example::PerformanceTestApp>(std::move(signalList));
    myApp->run();
    return 0;
}
