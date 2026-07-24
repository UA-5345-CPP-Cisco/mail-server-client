#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

#include <boost/json.hpp>
#include <boost/system/error_code.hpp>
#include <mail_server/services/MailServerConfigurator.h>

namespace ISXMailServer {

namespace {

const boost::json::object& GetObject(const boost::json::object& object, std::string_view field)
{
  auto const* value = object.if_contains(field);
  if (value == nullptr || !value->is_object())
  {
    throw std::runtime_error(std::string{field} + " must be an object");
  }

  return value->as_object();
}

std::string GetString(const boost::json::object& object,
                      std::string_view field,
                      const std::string& default_value = "")
{
  auto const* value = object.if_contains(field);
  if (value == nullptr)
  {
    return default_value;
  }

  if (!value->is_string())
  {
    throw std::runtime_error(std::string{field} + " must be a string");
  }

  return value->as_string().c_str();
}

bool GetBool(const boost::json::object& object, std::string_view field, bool default_value)
{
  auto const* value = object.if_contains(field);
  if (value == nullptr)
  {
    return default_value;
  }

  if (!value->is_bool())
  {
    throw std::runtime_error(std::string{field} + " must be a boolean");
  }

  return value->as_bool();
}

std::uint16_t
GetPort(const boost::json::object& object, std::string_view field, std::uint16_t default_value)
{
  auto const* value = object.if_contains(field);
  if (value == nullptr)
  {
    return default_value;
  }

  if (!value->is_int64() && !value->is_uint64())
  {
    throw std::runtime_error(std::string{field} + " must be an integer");
  }

  const std::uint64_t port =
    value->is_uint64() ? value->as_uint64() : static_cast<std::uint64_t>(value->as_int64());
  if (port == 0 || port > std::numeric_limits<std::uint16_t>::max())
  {
    throw std::runtime_error(std::string{field} + " is outside the valid range");
  }

  return static_cast<std::uint16_t>(port);
}

} // namespace

MailServerConfigurator::MailServerConfigurator(const std::filesystem::path& path)
{
  Load(path);
  Validate();
}

const MailServerConfiguration& MailServerConfigurator::Configuration() const noexcept
{
  return m_configuration;
}

void MailServerConfigurator::Load(const std::filesystem::path& path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    throw std::runtime_error("Unable to read mail server configuration '" + path.string() + "'");
  }

  const std::string text{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
  boost::system::error_code error;
  boost::json::value document = boost::json::parse(text, error);

  if (error)
  {
    throw std::runtime_error("Unable to parse mail server configuration '" + path.string() +
                             "': " + error.message());
  }

  if (!document.is_object())
  {
    throw std::runtime_error("Mail server configuration must be a JSON object");
  }

  const boost::json::object& root = document.as_object();
  const boost::json::object& database = GetObject(root, "database");
  const boost::json::object& server = GetObject(root, "server");
  const boost::json::object& smtp = GetObject(root, "smtp");
  const boost::json::object& logging = GetObject(root, "logging");

  m_configuration.database.storage_path = GetString(database, "storage_path");
  m_configuration.database.migrations_path = GetString(database, "migrations_path");
  m_configuration.server.host = GetString(server, "host");
  m_configuration.server.port = GetPort(server, "port", 8080);
  m_configuration.smtp.host = GetString(smtp, "host");
  m_configuration.smtp.port = GetPort(smtp, "port", 2525);
  m_configuration.logger.file_path = GetString(logging, "file_path");
  m_configuration.logger.print_in_console = GetBool(logging, "print_in_console", false);
  m_configuration.logger.level = ParseLogLevel(GetString(logging, "level", "Info"));
}

void MailServerConfigurator::Validate() const
{
  if (m_configuration.database.storage_path.empty())
  {
    throw std::runtime_error("database.storage_path must not be empty");
  }
  if (m_configuration.database.migrations_path.empty())
  {
    throw std::runtime_error("database.migrations_path must not be empty");
  }
  if (m_configuration.server.host.empty())
  {
    throw std::runtime_error("server.host must not be empty");
  }
  if (m_configuration.smtp.host.empty())
  {
    throw std::runtime_error("smtp.host must not be empty");
  }
  if (m_configuration.logger.file_path.empty())
  {
    throw std::runtime_error("logging.file_path must not be empty");
  }
}

LogLevel MailServerConfigurator::ParseLogLevel(const std::string& level) const
{
  if (level == "Trace")
  {
    return LogLevel::Trace;
  }
  if (level == "Debug")
  {
    return LogLevel::Debug;
  }
  if (level == "Info")
  {
    return LogLevel::Info;
  }
  if (level == "Warning")
  {
    return LogLevel::Warning;
  }
  if (level == "Error")
  {
    return LogLevel::Error;
  }

  throw std::runtime_error("Unsupported logging.level: " + level);
}

} // namespace ISXMailServer
