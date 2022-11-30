/**
 * Copyright (c) 2022 Robert Bosch GmbH
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

#include "sdk/dapr/DaprSupport.h"

#include "sdk/Logger.h"

#include <cpr/cpr.h>
#include <fmt/core.h>

#include <thread>

namespace velocitas::dapr {

void waitForSidecar() {
    auto* const envPort = std::getenv("DAPR_HTTP_PORT");
    if (envPort == nullptr) {
        logger().info("dapr: env DAPR_HTTP_PORT not set. Continuing without dapr ...");
    } else {
        logger().info("dapr: env DAPR_HTTP_PORT set. Waiting for sidecar ...");
        const auto targetPort = atoi(envPort);
        for (auto success = false; !success;) {
            try {
                constexpr auto STATUS_NO_CONTENT{204};
                logger().info("dapr: Requesting side car health endpoint...");

                const auto response =
                    cpr::Get(cpr::Url(fmt::format("http://localhost:{}/v1.0/healthz", targetPort)));
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

} // namespace velocitas::dapr
