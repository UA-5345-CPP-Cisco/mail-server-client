#include "smtp/Logger.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

namespace smtp {

namespace {

const char* ToString(LogLevel level)
{
    switch (level) {
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARN";
    case LogLevel::Error:
        return "ERROR";
    }

    return "UNKNOWN";
}

}

void Logger::Log(LogLevel level, std::string_view message)
{
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::lock_guard<std::mutex> lock(mutex_);
    std::clog << '[' << now << "] [" << ToString(level) << "] " << message << '\n';
}

}
