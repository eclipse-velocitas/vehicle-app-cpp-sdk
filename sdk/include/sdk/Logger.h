/**
 * Copyright (c) 2022-2023 Contributors to the Eclipse Foundation
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

#include <fmt/core.h>
#include <memory>
#include <string>

namespace velocitas {

/**
 * @brief Logger interface for implementing your own loggers.
 *
 */
class ILogger {
public:
    ILogger()          = default;
    virtual ~ILogger() = default;

    ILogger(const ILogger&)            = delete;
    ILogger(ILogger&&)                 = delete;
    ILogger& operator=(const ILogger&) = delete;
    ILogger& operator=(ILogger&&)      = delete;

    /**
     * @brief Log a message with info level.
     *
     * @param msg The message to log.
     */
    virtual void info(const std::string& msg) = 0;

    /**
     * @brief Log a message with warn level.
     *
     * @param msg The message to log.
     */
    virtual void warn(const std::string& msg) = 0;

    /**
     * @brief Log a message with error level.
     *
     * @param msg The message to log.
     */
    virtual void error(const std::string& msg) = 0;

    /**
     * @brief Log a message with debug level.
     *
     * @param msg The message to log.
     */
    virtual void debug(const std::string& msg) = 0;
};

/**
 * @brief Allows logging of messages with different log levels.
 *
 * @details Handles string formatting of the log message and its arguments
 *          then forwards the formatted message to the real logger
 *          implementation. (Bridge pattern)
 */
class Logger {
public:
    Logger();

    /**
     * @brief Log a message with info level.
     *
     * @tparam T    Type of the format arguments.
     * @param msg   The format message.
     * @param args  The format arguments.
     */
    template <typename... T> void info(const std::string& msg, const T&... args) {
        m_impl->info(fmt::format(msg, args...));
    }

    /**
     * @brief Log a message with warn level.
     *
     * @tparam T    Type of the format arguments.
     * @param msg   The format message.
     * @param args  The format arguments.
     */
    template <typename... T> void warn(const std::string& msg, const T&... args) {
        m_impl->warn(fmt::format(msg, args...));
    }

    /**
     * @brief Log a message with error level.
     *
     * @tparam T    Type of the format arguments.
     * @param msg   The format message.
     * @param args  The format arguments.
     */
    template <typename... T> void error(const std::string& msg, const T&... args) {
        m_impl->error(fmt::format(msg, args...));
    }

    /**
     * @brief Log a message with debug level.
     *
     * @tparam T    Type of the format arguments.
     * @param msg   The format message.
     * @param args  The format arguments.
     */
    template <typename... T> void debug(const std::string& msg, const T&... args) {
        m_impl->debug(fmt::format(msg, args...));
    }

    /**
     * @brief Set the Logger Implementation object
     *
     * @param impl The new implementation to use.
     */
    void setLoggerImplementation(std::unique_ptr<ILogger>&& impl) { m_impl = std::move(impl); }

private:
    std::unique_ptr<ILogger> m_impl;
};

/**
 * @brief Return the global logger instance.
 *
 * @return Logger&
 */
inline Logger& logger() {
    static Logger loggerInstance{};
    return loggerInstance;
}

} // namespace velocitas

#endif // VEHICLE_APP_SDK_LOGGER_H
