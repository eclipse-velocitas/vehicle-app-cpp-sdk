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

#include "sdk/Logger.h"
#include "sdk/Utils.h"
#include "sdk/dapr/DaprSupport.h"

#include <stdexcept>
#include <string>

#include <cpr/cpr.h>
#include <fmt/core.h>

#include <thread>

namespace velocitas {

static const std::string ENV_DAPR_GRPC_PORT = "DAPR_GRPC_PORT";
static const std::string ENV_DAPR_HTTP_PORT = "DAPR_HTTP_PORT";

static const std::string DAPR_APP_ID_KEY = "dapr-app-id";

namespace dapr {

void waitForSidecar() {
    auto sidecarHttpPort = getEnvVar(ENV_DAPR_HTTP_PORT);
    if (sidecarHttpPort.empty()) {
        logger().info("dapr: env {} not set. Continuing without sidecar health check ...",
                      ENV_DAPR_HTTP_PORT);
    } else {
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
                logger().debug("dapr: Exception occurred during health endpoint: {}", e.what());
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

} // namespace dapr

void DaprMiddleware::waitUntilReady() { dapr::waitForSidecar(); }

std::string DaprMiddleware::getServiceLocation(const std::string& serviceName) const {
    std::ignore = serviceName;

    auto daprSidecarGrpcPort = getEnvVar(ENV_DAPR_GRPC_PORT);
    if (daprSidecarGrpcPort.empty()) {
        throw std::runtime_error(
            fmt::format("{} not set! Cannot connect to Dapr sidecar!", ENV_DAPR_GRPC_PORT));
    }
    return "grpc:://localhost:" + daprSidecarGrpcPort;
};

Middleware::Metadata DaprMiddleware::getMetadata(const std::string& serviceName) const {
    auto appId = getEnvVar(StringUtils::toUpper(serviceName) + "_DAPR_APP_ID");
    if (appId.empty()) {
        appId = StringUtils::toLower(serviceName);
    }
    return {{DAPR_APP_ID_KEY, appId}};
}

} // namespace velocitas
