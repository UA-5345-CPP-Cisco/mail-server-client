#include <QCoreApplication>
#include <gtest/gtest.h>
#include "client_logger/ClientConfigReader.h"
#include "client_logger/ClientProxyLogger.h"
#include "service/Service.h"
#include "logger/Logger.h"

int main(int argc, char** argv)
{
  QCoreApplication app(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  ISXConfig::ClientConfigReader reader;
  auto config = reader.ReadConfig(ISXConfig::ClientConfigReader::ConfigPath());

  if (!config)
  {
    return -1;
  }

  ISXClientLogger::ClientLoggerProvider logger_provider(config.value());
  ISXClientLogger::ClientLogger logger(Logging::Logger::Instance(), logger_provider);

  ISXService::Service::Initialize(logger);

  return RUN_ALL_TESTS();
}
