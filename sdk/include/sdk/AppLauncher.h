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

#ifndef VEHICLE_APP_SDK_APPLAUNCHER_H
#define VEHICLE_APP_SDK_APPLAUNCHER_H

#include "sdk/Logger.h"

#include <csignal>
#include <memory>

namespace velocitas {

/**
 * @brief Flips a boolean value within a scope.
 *
 */
template <typename TAppClass> class AppLauncher {
public:
    AppLauncher()  = delete;
    ~AppLauncher() = delete;

    static void signalHandler(int /*unused*/) {
        logger().info("App terminating signal received.");
        app->stop();
    }

    template <typename... TArgs> static void runApp(TArgs... args) {
        signal(SIGINT, &AppLauncher::signalHandler);
        app = std::make_unique<TAppClass>(args...);
        app->run();
    }

private:
    static std::unique_ptr<TAppClass> app;
};

} // namespace velocitas

#endif // VEHICLE_APP_SDK_APPLAUNCHER_H
