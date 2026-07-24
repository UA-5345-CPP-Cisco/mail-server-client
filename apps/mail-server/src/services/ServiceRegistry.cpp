#include <stdexcept>

#include <mail_server/services/ServiceRegistry.h>

namespace ISXMailServer {

std::unique_ptr<MailServerConfigurator> ServiceRegistry::s_configurator;
std::unique_ptr<MailServerLogger> ServiceRegistry::s_logger;
std::unique_ptr<MailStorage> ServiceRegistry::s_storage;

void ServiceRegistry::Initialize(const std::filesystem::path& configuration_path)
{
  s_configurator = std::make_unique<MailServerConfigurator>(configuration_path);
  s_logger = std::make_unique<MailServerLogger>(s_configurator->Configuration().logger);
  s_storage = std::make_unique<MailStorage>(s_configurator->Configuration().database);

  s_logger->Log(LogLevel::Info, "Mail server services initialized");
}

MailServerConfigurator& ServiceRegistry::Configurator()
{
  if (!s_configurator)
  {
    throw std::runtime_error("ServiceRegistry is not initialized");
  }

  return *s_configurator;
}

MailServerLogger& ServiceRegistry::Logger()
{
  if (!s_logger)
  {
    throw std::runtime_error("ServiceRegistry is not initialized");
  }

  return *s_logger;
}

MailStorage& ServiceRegistry::Storage()
{
  if (!s_storage)
  {
    throw std::runtime_error("ServiceRegistry is not initialized");
  }

  return *s_storage;
}

} // namespace ISXMailServer
