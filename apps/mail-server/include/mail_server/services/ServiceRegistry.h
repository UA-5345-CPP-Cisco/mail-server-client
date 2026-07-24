#pragma once

#include <filesystem>
#include <memory>

#include <mail_server/services/MailServerConfigurator.h>
#include <mail_server/services/MailServerLogger.h>
#include <mail_server/services/MailStorage.h>

namespace ISXMailServer {

class ServiceRegistry
{
  public:
  static void Initialize(const std::filesystem::path& configuration_path);

  [[nodiscard]] static MailServerConfigurator& Configurator();
  [[nodiscard]] static MailServerLogger& Logger();
  [[nodiscard]] static MailStorage& Storage();

  private:
  static std::unique_ptr<MailServerConfigurator> s_configurator;
  static std::unique_ptr<MailServerLogger> s_logger;
  static std::unique_ptr<MailStorage> s_storage;
};

} // namespace ISXMailServer
