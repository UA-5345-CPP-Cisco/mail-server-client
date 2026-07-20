#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace ISXMailServer {

enum class LogLevel
{
  Trace,
  Debug,
  Info,
  Warning,
  Error
};

struct DatabaseConfiguration
{
  std::filesystem::path storage_path;
  std::filesystem::path migrations_path;
};

struct ServerConfiguration
{
  std::string host;
  std::uint16_t port{};
};

struct SmtpConfiguration
{
  std::string host;
  std::uint16_t port{};
};

struct LoggerConfiguration
{
  std::filesystem::path file_path;
  bool print_in_console{};
  LogLevel level{LogLevel::Info};
};

struct MailServerConfiguration
{
  DatabaseConfiguration database;
  ServerConfiguration server;
  SmtpConfiguration smtp;
  LoggerConfiguration logger;
};

} // namespace ISXMailServer
