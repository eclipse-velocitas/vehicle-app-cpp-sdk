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

#ifndef VEHICLE_APP_SDK_LOGGER_H
#define VEHICLE_APP_SDK_LOGGER_H

#include <chrono>
#include <cstdio>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <string>

namespace velocitas {

/**
 * @brief Logger implementation for logging to the console.
 *
 */
class ConsoleLogger {
public:
    void info(const std::string& msg) const {
        fmt::print(fmt::fg(fmt::color::white), "{}, INFO  : {}\n", std::chrono::system_clock::now(),
                   msg);
        std::fflush(stdout);
    }

    void warn(const std::string& msg) const {
        fmt::print(fmt::fg(fmt::color::yellow), "{}, WARN  : {}\n",
                   std::chrono::system_clock::now(), msg);
        std::fflush(stdout);
    }

    void error(const std::string& msg) const {
        fmt::print(fmt::fg(fmt::color::red), "{}, ERROR : {}\n", std::chrono::system_clock::now(),
                   msg);
        std::fflush(stdout);
    }

    void debug(const std::string& msg) const {
        fmt::print(fmt::fg(fmt::color::brown), "{}, DEBUG : {}\n", std::chrono::system_clock::now(),
                   msg);
        std::fflush(stdout);
    }
};

/**
 * @brief Logger template implementation which will delegate the calls to the
 * implementing class.
 *
 * @tparam TImpl  The class implementing the logger functionality.
 */
template <typename TImpl> class Logger {
public:
    /**
     * @brief Proxy function which applies string formatting prior to passing it
     *        to the actual implementation.
     */
    template <typename... T> void info(const std::string& msg, T... args) {
        info(fmt::format(msg, args...));
    }

    /**
     * @brief Proxy function which applies string formatting prior to passing it
     *        to the actual implementation.
     */
    template <typename... T> void warn(const std::string& msg, T... args) {
        warn(fmt::format(msg, args...));
    }

    /**
     * @brief Proxy function which applies string formatting prior to passing it
     *        to the actual implementation.
     */
    template <typename... T> void error(const std::string& msg, T... args) {
        error(fmt::format(msg, args...));
    }

    /**
     * @brief Proxy function which applies string formatting prior to passing it
     *        to the actual implementation.
     */
    template <typename... T> void debug(const std::string& msg, T... args) {
        debug(fmt::format(msg, args...));
    }

    void info(const std::string& msg) const { m_impl.info(msg); }
    void warn(const std::string& msg) const { m_impl.warn(msg); }
    void error(const std::string& msg) const { m_impl.error(msg); }
    void debug(const std::string& msg) const { m_impl.debug(msg); }

private:
    TImpl m_impl;
};

/**
 * @brief Return the global logger instance.
 *
 * @return Logger<ConsoleLogger>&
 */
inline Logger<ConsoleLogger>& logger() {
    static Logger<ConsoleLogger> logger2;
    return logger2;
}

} // namespace velocitas

#endif // VEHICLE_APP_SDK_LOGGER_H
