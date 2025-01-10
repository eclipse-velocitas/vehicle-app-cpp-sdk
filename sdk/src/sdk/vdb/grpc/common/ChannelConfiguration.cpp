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

#include "ChannelConfiguration.h"

#include "sdk/Logger.h"
#include "sdk/Utils.h"

#include <fstream>
#include <grpcpp/support/channel_arguments.h>
#include <nlohmann/json.hpp>

extern char** environ;

namespace velocitas {

namespace {
constexpr char const* ENV_VAR_CHANNEL_CONFIG = "SDV_VDB_CHANNEL_CONFIG_PATH";

constexpr char const* JSON_CHANNEL_ARGS_KEY = "channelArguments";
} // namespace

grpc::ChannelArguments getChannelArguments() {
    grpc::ChannelArguments chArgs;

    std::string chConfigFilepath = getEnvVar(ENV_VAR_CHANNEL_CONFIG);
    if (!chConfigFilepath.empty()) {
        auto ifs = std::ifstream(chConfigFilepath);
        if (ifs.is_open()) {
            try {
                velocitas::logger().info("Reading channel configuration from file {}.",
                                         chConfigFilepath);
                const auto  config      = nlohmann::json::parse(ifs);
                const auto& channelArgs = config[JSON_CHANNEL_ARGS_KEY];
                for (const auto& channelArg : channelArgs.items()) {
                    if (channelArg.value().is_number_integer()) {
                        chArgs.SetInt(channelArg.key(), channelArg.value());
                    } else if (channelArg.value().is_string()) {
                        chArgs.SetString(channelArg.key(), channelArg.value());
                    } else {
                        velocitas::logger().warn("Ignoring channel argument {} - unknown type.",
                                                 channelArg.key());
                    }
                }
            } catch (const nlohmann::json::exception& ex) {
                velocitas::logger().warn("Error reading channel configuration file {}.",
                                         chConfigFilepath);
            }
        } else {
            velocitas::logger().warn("Cannot open channel configuration file {}.",
                                     chConfigFilepath);
        }
    }

    return chArgs;
}

} // namespace velocitas
