#include "headers/service/Service.h"

namespace ISXService
{

void Service::Initialize(ISXClientLogger::ClientLogger& logger)
{
  m_logger = &logger;
}

ISXClientLogger::ClientLogger& Service::Logger()
{
  return *m_logger;
}

} // namespace ISXService