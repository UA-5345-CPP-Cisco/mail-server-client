#pragma once

#include <filesystem>

#include "smtp/ServerConfig.hpp"

namespace smtp {

class Configuration
{
  public:
  explicit Configuration(const std::filesystem::path& path);

  const DbConfig& Database() const noexcept;
  const TlsConfig& Tls() const noexcept;
  const ServerConfig& Server() const noexcept;

  private:
  DbConfig database_;
  ServerConfig server_;

  void Load(const std::filesystem::path& path);
  void Validate() const;
};

} // namespace smtp
