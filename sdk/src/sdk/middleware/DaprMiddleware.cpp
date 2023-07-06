/**
 * Copyright (c) 2022-2023 Robert Bosch GmbH
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

#include "DaprMiddleware.h"

#include "NativeMiddleware.h"
#include "sdk/Logger.h"
#include "sdk/Utils.h"
#include "sdk/dapr/DaprSupport.h"

#include <stdexcept>
#include <string>

#include <cpr/cpr.h>
#include <fmt/core.h>

#include <thread>

namespace velocitas {

namespace {

constexpr char const* ENV_DAPR_GRPC_PORT = "DAPR_GRPC_PORT";
constexpr char const* ENV_DAPR_HTTP_PORT = "DAPR_HTTP_PORT";

constexpr char const* DAPR_APP_ID_KEY = "dapr-app-id";

void waitForSidecar() {
    auto sidecarHttpPort = getEnvVar(ENV_DAPR_HTTP_PORT);
    if (sidecarHttpPort.empty()) {
        logger().error("dapr: env {} not set. Cannot check presence of sidecar!",
                       ENV_DAPR_HTTP_PORT);
        throw std::runtime_error(
            fmt::format("env {} not set! Cannot check presence of sidecar!", ENV_DAPR_HTTP_PORT));
    }

    logger().info("dapr: env {} set. Waiting for sidecar at port {} ...", ENV_DAPR_HTTP_PORT,
                  sidecarHttpPort);
    for (auto success = false; !success;) {
        try {
            constexpr auto STATUS_NO_CONTENT{204};
            logger().info("dapr: Requesting side car health endpoint...");

            const auto response = cpr::Get(
                cpr::Url(fmt::format("http://localhost:{}/v1.0/healthz", sidecarHttpPort)));
            success = response.status_code == STATUS_NO_CONTENT;
            logger().debug("dapr: Health endpoint returned status code: {}, {}",
                           response.status_code, response.text);
        } catch (const std::exception& e) {
            logger().warn("dapr: Exception occurred requesting health endpoint: {}", e.what());
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // anonymous namespace

namespace dapr {
void waitForSidecar() { velocitas::waitForSidecar(); }
} // namespace dapr

void DaprMiddleware::waitUntilReady() { waitForSidecar(); }

std::string DaprMiddleware::getServiceLocation(const std::string& serviceName) const {
    std::ignore = serviceName;

    auto daprSidecarGrpcPort = getEnvVar(ENV_DAPR_GRPC_PORT);
    if (daprSidecarGrpcPort.empty()) {
        throw std::runtime_error(
            fmt::format("{} not set! Cannot connect to Dapr sidecar!", ENV_DAPR_GRPC_PORT));
    }
    return "localhost:" + daprSidecarGrpcPort;
};

Middleware::Metadata DaprMiddleware::getMetadata(const std::string& serviceName) const {
    auto appId = getEnvVar(StringUtils::toUpper(serviceName) + "_DAPR_APP_ID");
    if (appId.empty()) {
        appId = StringUtils::toLower(serviceName);
    }
    return {{DAPR_APP_ID_KEY, appId}};
}

std::shared_ptr<IPubSubClient>
DaprMiddleware::createPubSubClient(const std::string& clientId) const {
    logger().warn("Velocitas' C++ SDK does not yet support Dapr PubSub "
                  "-> connecting directly to MQTT broker!");

    return NativeMiddleware().createPubSubClient(clientId);
}

} // namespace velocitas
