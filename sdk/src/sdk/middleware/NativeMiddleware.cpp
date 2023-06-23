/**
 * Copyright (c) 2023 Robert Bosch GmbH
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

#include "NativeMiddleware.h"

#include "sdk/Logger.h"
#include "sdk/Utils.h"
#include "sdk/dapr/DaprSupport.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace velocitas {

static const std::unordered_map<std::string, std::string> DEFAULT_LOCATIONS = {
    {"mqtt", "mqtt://localhost:1883"},
    {"vehicledatabroker", "grpc://localhost:55555"},
};

static std::string getDefaultLocation(const std::string& serviceName) {
    std::string defaultLocation;
    auto        iter = DEFAULT_LOCATIONS.find(StringUtils::toLower(serviceName));
    if (iter != DEFAULT_LOCATIONS.end()) {
        defaultLocation = iter->second;
    }
    return defaultLocation;
};

std::string NativeMiddleware::getServiceEnvVarName(const std::string& serviceName) const {
    return "SDV_" + StringUtils::toUpper(serviceName) + "_ADDRESS";
}

std::string NativeMiddleware::getServiceLocation(const std::string& serviceName) const {
    auto envVarName     = getServiceEnvVarName(serviceName);
    auto serviceAddress = getEnvVar(envVarName);
    if (serviceAddress.empty()) {
        serviceAddress = getDefaultLocation(serviceName);
    }
    return serviceAddress;
}

} // namespace velocitas
