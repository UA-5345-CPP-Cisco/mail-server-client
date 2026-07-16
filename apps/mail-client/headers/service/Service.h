#pragma once

#include "logger/Logger.h"
#include "headers/client_logger/ClientProxyLogger.h"

namespace ISXService
{

class Service
{
public:
  static void Initialize(ISXClientLogger::ClientLogger& logger);
  static ISXClientLogger::ClientLogger& Logger();
private:
  inline static ISXClientLogger::ClientLogger* m_logger = nullptr;
};

}
