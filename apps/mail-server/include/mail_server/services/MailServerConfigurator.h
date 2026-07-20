#pragma once

#include <filesystem>

#include <mail_server/Configuration.h>

namespace ISXMailServer {

class MailServerConfigurator
{
  public:
  explicit MailServerConfigurator(const std::filesystem::path& path);

  [[nodiscard]] const MailServerConfiguration& Configuration() const noexcept;

  private:
  MailServerConfiguration m_configuration;

  void Load(const std::filesystem::path& path);

  void Validate() const;

  [[nodiscard]] LogLevel ParseLogLevel(const std::string& level) const;
};

} // namespace ISXMailServer
