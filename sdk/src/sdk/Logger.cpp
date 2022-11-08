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
    void info(const std::string& msg) override {
        fmt::print(fmt::fg(fmt::color::white), "{}, INFO  : {}\n", std::chrono::system_clock::now(),
                   msg);
        std::fflush(stdout);
    }

    void warn(const std::string& msg) override {
        fmt::print(fmt::fg(fmt::color::yellow), "{}, WARN  : {}\n",
                   std::chrono::system_clock::now(), msg);
        std::fflush(stdout);
    }

    void error(const std::string& msg) override {
        fmt::print(fmt::fg(fmt::color::red), "{}, ERROR : {}\n", std::chrono::system_clock::now(),
                   msg);
        std::fflush(stdout);
    }

    void debug(const std::string& msg) override {
        fmt::print(fmt::fg(fmt::color::brown), "{}, DEBUG : {}\n", std::chrono::system_clock::now(),
                   msg);
        std::fflush(stdout);
    }
};

Logger::Logger()
    : m_impl(std::make_unique<ConsoleLogger>()) {}

} // namespace velocitas
