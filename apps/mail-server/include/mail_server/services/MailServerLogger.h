#pragma once

#include <string_view>

#include <logger/Logger.h>
#include <mail_server/Configuration.h>

namespace ISXMailServer {

class MailServerLogger
{
  public:
  explicit MailServerLogger(LoggerConfiguration configuration);

  void Log(LogLevel level, std::string_view message);

  private:
  LoggerConfiguration m_configuration;
  Logging::Logger m_logger;

  [[nodiscard]] Logging::LogLevel MapLogLevel(LogLevel level) const;
  [[nodiscard]] const char* ToString(LogLevel level) const;
};

} // namespace ISXMailServer
