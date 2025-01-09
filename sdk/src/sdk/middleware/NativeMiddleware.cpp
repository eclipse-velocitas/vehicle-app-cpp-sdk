/**
 * Copyright (c) 2023-2025 Contributors to the Eclipse Foundation
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

#include "sdk/IPubSubClient.h"
#include "sdk/Logger.h"
#include "sdk/Utils.h"

#include "fmt/core.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace velocitas {

static const std::unordered_map<std::string, std::string> DEFAULT_LOCATIONS = {
    {"mqtt", "localhost:1883"},
    {"vehicledatabroker", "localhost:55555"},
};

static std::string getDefaultLocation(const std::string& serviceName) {
    std::string defaultLocation;
    auto        iter = DEFAULT_LOCATIONS.find(StringUtils::toLower(serviceName));
    if (iter != DEFAULT_LOCATIONS.end()) {
        defaultLocation = SimpleUrlParse(iter->second).getNetLocation();
    }
    return defaultLocation;
};

static std::string getServiceEnvVarName(const std::string& serviceName) {
    return "SDV_" + StringUtils::toUpper(serviceName) + "_ADDRESS";
}

std::string NativeMiddleware::getServiceLocation(const std::string& serviceName) const {
    auto envVarName     = getServiceEnvVarName(serviceName);
    auto serviceAddress = SimpleUrlParse(getEnvVar(envVarName)).getNetLocation();
    if (!serviceAddress.empty()) {
        return serviceAddress;
    }

    serviceAddress = getDefaultLocation(serviceName);
    if (!serviceAddress.empty()) {
        logger().warn("Env variable '{}' defining location of service '{}' not properly set. "
                      "Taking default: '{}'",
                      envVarName, serviceName, serviceAddress);
        return serviceAddress;
    }

    const std::string errorMsg{
        fmt::format("Env variable '{}' defining location of service '{}' not set. Please define!",
                    envVarName, serviceName)};
    logger().error(errorMsg);
    throw std::runtime_error(errorMsg);
}

std::shared_ptr<IPubSubClient>
NativeMiddleware::createPubSubClient(const std::string& clientId) const {
    std::string brokerLocation = getServiceLocation("mqtt");
    return IPubSubClient::createInstance(brokerLocation, clientId);
}

} // namespace velocitas
