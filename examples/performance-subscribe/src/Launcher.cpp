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

#include "PerformanceTestApp.h"
#include "sdk/Logger.h"

#include <nlohmann/json.hpp>

#include <csignal>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

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
        const std::string& signalName = signal["path"];
        if (!signalName.empty()) {
            velocitas::logger().debug("{}", signalName);
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

    const auto configFile =
        (argc > 1) ? std::filesystem::path(argv[1]).string() : getDefaultConfigFilePath(argv[0]);
    auto signalList = readSignalNameFromFiles(configFile);

    myApp = std::make_unique<example::PerformanceTestApp>(std::move(signalList));
    myApp->run();
    return 0;
}
