#include <iostream>
#include <stdexcept>
#include <utility>

#include <mail_server/services/MailServerLogger.h>

namespace ISXMailServer {

MailServerLogger::MailServerLogger(LoggerConfiguration configuration) :
  m_configuration(std::move(configuration)),
  m_logger(m_configuration.file_path.string(), MapLogLevel(m_configuration.level), true)
{
}

void MailServerLogger::Log(LogLevel level, std::string_view message)
{
  if (level < m_configuration.level)
  {
    return;
  }

  m_logger.Log(MapLogLevel(level), message);

  if (m_configuration.print_in_console)
  {
    std::clog << '[' << ToString(level) << "] " << message << '\n';
  }
}

Logging::LogLevel MailServerLogger::MapLogLevel(LogLevel level) const
{
  switch (level)
  {
  case LogLevel::Trace:
    return Logging::LogLevel::Trace;
  case LogLevel::Debug:
    return Logging::LogLevel::Debug;
  case LogLevel::Info:
    return Logging::LogLevel::Info;
  case LogLevel::Warning:
    return Logging::LogLevel::Warning;
  case LogLevel::Error:
    return Logging::LogLevel::Error;
  }

  throw std::runtime_error("Unsupported mail server log level");
}

const char* MailServerLogger::ToString(LogLevel level) const
{
  switch (level)
  {
  case LogLevel::Trace:
    return "TRACE";
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warning:
    return "WARN";
  case LogLevel::Error:
    return "ERROR";
  }

  return "UNKNOWN";
}

} // namespace ISXMailServer
