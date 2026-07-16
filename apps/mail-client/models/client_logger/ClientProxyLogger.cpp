#include "headers/client_logger/ClientProxyLogger.h"

namespace ISXClientLogger {

  ClientLogger::ClientLogger(ILogger& logger, ILoggerProvider& provider) :
    m_logger(logger),
    m_loggerProvider(provider)
  {
  }

  void ClientLogger::Log(Logging::LogLevel level, std::string_view message)
  {
    if (static_cast<int>(level) < static_cast<int>(m_loggerProvider.MinimumLogLevel()))
    {
      return;
    }

    m_logger.Log(level, message);
  }

  ClientLoggerProvider::ClientLoggerProvider(ISXConfig::ClientConfig config) :
      m_clientConfig(config)
  {

  }
  Logging::LogLevel ClientLoggerProvider::MinimumLogLevel() const
  {
    return m_clientConfig.logLevel;
  }

}