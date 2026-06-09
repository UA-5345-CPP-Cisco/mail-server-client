#pragma once

#include <string_view>

namespace smtp {

enum class LogLevel {
    Info,
    Warning,
    Error
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void Log(LogLevel level, std::string_view message) = 0;
};

}
