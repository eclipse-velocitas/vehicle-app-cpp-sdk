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

#include <gtest/gtest.h>

using namespace velocitas;

const char JSON_TEST_STRING[] =
    "\"packages\": {\"devenv-runtimes\": \"v4.0.6\",\"devenv-github-workflows\": "
    "\"v6.1.3\"}";

class StringLogger : public ILogger {
public:
    enum class LogLevel { Unknown, Info, Warn, Error, Debug };

    void info(const std::string& msg) override {
        m_lastLevel   = LogLevel::Info;
        m_lastMessage = msg;
    }

    void warn(const std::string& msg) override {
        m_lastLevel   = LogLevel::Warn;
        m_lastMessage = msg;
    }

    void error(const std::string& msg) override {
        m_lastLevel   = LogLevel::Error;
        m_lastMessage = msg;
    }

    void debug(const std::string& msg) override {
        m_lastLevel   = LogLevel::Debug;
        m_lastMessage = msg;
    }

    [[nodiscard]] const std::string& getLogMessage() const { return m_lastMessage; }
    [[nodiscard]] LogLevel           getLogLevel() const { return m_lastLevel; }

private:
    std::string m_lastMessage;
    LogLevel    m_lastLevel{LogLevel::Unknown};
};

class Test_Logger : public ::testing::Test {
public:
    void SetUp() override {
        auto stringLogger = std::make_unique<StringLogger>();
        m_stringLogger    = &*stringLogger;
        logger().setLoggerImplementation(std::move(stringLogger));
    }

    StringLogger* m_stringLogger{};
};

TEST_F(Test_Logger, info_withoutArguments_correctMessageLoggedOnCorrectLevel) {
    logger().info("Hello World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Info);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().info(JSON_TEST_STRING);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Info);
    EXPECT_EQ(m_stringLogger->getLogMessage(), JSON_TEST_STRING);
}

TEST_F(Test_Logger, info_missingArgumentsException) {
    logger().info("Hello World {}");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Info);
    // No formatting if no arguments, we just treat it as a raw string
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World {}");

    // But here we expect an exception
    EXPECT_THROW(logger().info("Hello World {} {}", "Next missing"), std::runtime_error);
}

TEST_F(Test_Logger, info_withArguments_correctMessageLoggedOnCorrectLevel) {
    const auto testInt   = 1337;
    const auto testFloat = 9.312F;

    logger().info("Hello {}", "World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Info);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().info("Hello {} {}", "World", testInt);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Info);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World 1337");

    logger().info("{}, {}, {}", "Foo", testInt, testFloat);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Info);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Foo, 1337, 9.312");
}

TEST_F(Test_Logger, warn_withoutArguments_correctMessageLoggedOnCorrectLevel) {
    logger().warn("Hello World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Warn);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().warn(JSON_TEST_STRING);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Warn);
    EXPECT_EQ(m_stringLogger->getLogMessage(), JSON_TEST_STRING);
}

TEST_F(Test_Logger, warn_missingArgumentsException) {
    logger().warn("Hello World {}");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Warn);
    // No formatting if no arguments, we just treat it as a raw string
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World {}");

    // But here we expect an exception
    EXPECT_THROW(logger().warn("Hello World {} {}", "Next missing"), std::runtime_error);
}

TEST_F(Test_Logger, warn_withArguments_correctMessageLoggedOnCorrectLevel) {
    const auto testInt   = 1337;
    const auto testFloat = 9.312F;

    logger().warn("Hello {}", "World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Warn);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().warn("Hello {} {}", "World", testInt);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Warn);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World 1337");

    logger().warn("{}, {}, {}", "Foo", testInt, testFloat);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Warn);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Foo, 1337, 9.312");
}

TEST_F(Test_Logger, error_withoutArguments_correctMessageLoggedOnCorrectLevel) {
    logger().error("Hello World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Error);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().error(JSON_TEST_STRING);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Error);
    EXPECT_EQ(m_stringLogger->getLogMessage(), JSON_TEST_STRING);
}

TEST_F(Test_Logger, error_missingArgumentsException) {
    logger().error("Hello World {}");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Error);
    // No formatting if no arguments, we just treat it as a raw string
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World {}");

    // But here we expect an exception
    EXPECT_THROW(logger().error("Hello World {} {}", "Next missing"), std::runtime_error);
}

TEST_F(Test_Logger, error_withArguments_correctMessageLoggedOnCorrectLevel) {
    const auto testInt   = 1337;
    const auto testFloat = 9.312F;

    logger().error("Hello {}", "World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Error);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().error("Hello {} {}", "World", testInt);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Error);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World 1337");

    logger().error("{}, {}, {}", "Foo", testInt, testFloat);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Error);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Foo, 1337, 9.312");
}

TEST_F(Test_Logger, debug_withoutArguments_correctMessageLoggedOnCorrectLevel) {
    logger().debug("Hello World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Debug);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().debug(JSON_TEST_STRING);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Debug);
    EXPECT_EQ(m_stringLogger->getLogMessage(), JSON_TEST_STRING);
}

TEST_F(Test_Logger, debug_missingArgumentsException) {
    logger().debug("Hello World {}");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Debug);
    // No formatting if no arguments, we just treat it as a raw string
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World {}");

    // But here we expect an exception
    EXPECT_THROW(logger().debug("Hello World {} {}", "Next missing"), std::runtime_error);
}

TEST_F(Test_Logger, debug_withArguments_correctMessageLoggedOnCorrectLevel) {
    const auto testInt   = 1337;
    const auto testFloat = 9.312F;

    logger().debug("Hello {}", "World");
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Debug);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World");

    logger().debug("Hello {} {}", "World", testInt);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Debug);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Hello World 1337");

    logger().debug("{}, {}, {}", "Foo", testInt, testFloat);
    EXPECT_EQ(m_stringLogger->getLogLevel(), StringLogger::LogLevel::Debug);
    EXPECT_EQ(m_stringLogger->getLogMessage(), "Foo, 1337, 9.312");
}
