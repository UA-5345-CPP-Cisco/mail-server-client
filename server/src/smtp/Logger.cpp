#include "smtp/Logger.hpp"

namespace smtp {

void Logger::Log(LogLevel, std::string_view)
{
    // Format the log level, timestamp, and message.
    // Write to the configured output: console, file, system logger, or test sink.
    // Keep logging failures from throwing through server/session code.
}

}
