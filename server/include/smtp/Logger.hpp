#pragma once

#include <mutex>
#include <string_view>

namespace smtp {

enum class LogLevel {
    Info,
    Warning,
    Error
};

// Logging boundary shared by server, sessions, and infrastructure components.
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void Log(LogLevel level, std::string_view message) = 0;
};

// Concrete logger component planned for later implementation.
// It should decide where logs go: console, file, system logger, or a mix.
class Logger : public ILogger {
public:
    void Log(LogLevel level, std::string_view message) override;

private:
    std::mutex mutex_;
};

}
