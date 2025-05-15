/**
 * Copyright (c) 2022-2025 Contributors to the Eclipse Foundation
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

#include "sdk/Logger.h"

#include <chrono>
#include <cstdio>
#include <fmt/chrono.h>
#include <fmt/color.h>

namespace velocitas {

/**
 * @brief Logger implementation for logging to the console.
 *
 */
class ConsoleLogger : public ILogger {
public:
    void info(const std::string& msg) override { log("INFO ", fmt::color::white, msg); }

    void warn(const std::string& msg) override { log("WARN ", fmt::color::yellow, msg); }

    void error(const std::string& msg) override { log("ERROR", fmt::color::red, msg); }

    void debug(const std::string& msg) override { log("DEBUG", fmt::color::brown, msg); }

private:
    void log(const std::string& level, fmt::color color, const std::string& msg) {
        fmt::print(fmt::fg(color), "{}, {} : {}\n", std::chrono::system_clock::now(), level, msg);
        std::fflush(stdout);
    }
};

Logger::Logger()
    : m_impl(std::make_unique<ConsoleLogger>()) {}

} // namespace velocitas
