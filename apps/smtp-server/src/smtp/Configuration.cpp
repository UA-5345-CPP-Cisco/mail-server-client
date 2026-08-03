#include "smtp/Configuration.hpp"

#include <limits>
#include <stdexcept>
#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace smtp {

namespace {

template <class Value>
void ReadValue(const boost::property_tree::ptree& tree, const std::string& path, Value& destination)
{
  destination = tree.get<Value>(path, destination);
}

} // namespace

Configuration::Configuration(const std::filesystem::path& path)
{
  Load(path);
  Validate();
}

const DbConfig& Configuration::Database() const noexcept
{
  return database_;
}

const TlsConfig& Configuration::Tls() const noexcept
{
  return server_.tls;
}

const ServerConfig& Configuration::Server() const noexcept
{
  return server_;
}

void Configuration::Load(const std::filesystem::path& path)
{
  boost::property_tree::ptree tree;

  try
  {
    boost::property_tree::read_json(path.string(), tree);

    ReadValue(tree, "database.storage_path", database_.storagePath);
    ReadValue(tree, "database.migrations_path", database_.migrationsPath);
    ReadValue(tree, "server.host", server_.host);
    ReadValue(tree, "server.server_name", server_.serverName);
    ReadValue(tree, "server.max_connections", server_.maxConnections);
    ReadValue(tree, "server.max_message_size_bytes", server_.maxMessageSizeBytes);
    ReadValue(tree, "server.require_authentication", server_.requireAuthentication);
    ReadValue(tree, "server.allow_plain_authentication_without_tls", server_.allowPlainAuthenticationWithoutTls);
    ReadValue(tree, "delivery.batch_size", server_.delivery.batchSize);
    ReadValue(tree, "delivery.poll_interval_ms", server_.delivery.pollIntervalMilliseconds);
    ReadValue(tree, "tls.enabled", server_.tls.enabled);
    ReadValue(tree, "tls.certificate_path", server_.tls.certificatePath);
    ReadValue(tree, "tls.private_key_path", server_.tls.privateKeyPath);

    const unsigned int port = tree.get<unsigned int>("server.port", server_.port);
    if (port > std::numeric_limits<std::uint16_t>::max())
    {
      throw std::runtime_error("server.port is outside the valid range");
    }
    server_.port = static_cast<std::uint16_t>(port);
  }
  catch (const boost::property_tree::json_parser_error& error)
  {
    throw std::runtime_error("Unable to read SMTP configuration '" + path.string() + "': " + error.message());
  }
  catch (const boost::property_tree::ptree_error& error)
  {
    throw std::runtime_error("Invalid SMTP configuration '" + path.string() + "': " + error.what());
  }
}

void Configuration::Validate() const
{
  if (database_.storagePath.empty())
  {
    throw std::runtime_error("database.storage_path must not be empty");
  }
  if (database_.migrationsPath.empty())
  {
    throw std::runtime_error("database.migrations_path must not be empty");
  }
  if (server_.host.empty())
  {
    throw std::runtime_error("server.host must not be empty");
  }
  if (server_.serverName.empty())
  {
    throw std::runtime_error("server.server_name must not be empty");
  }
  if (server_.port == 0)
  {
    throw std::runtime_error("server.port must be greater than zero");
  }
  if (server_.maxConnections == 0)
  {
    throw std::runtime_error("server.max_connections must be greater than zero");
  }
  if (server_.maxMessageSizeBytes == 0)
  {
    throw std::runtime_error("server.max_message_size_bytes must be greater than zero");
  }
  if (server_.delivery.batchSize == 0)
  {
    throw std::runtime_error("delivery.batch_size must be greater than zero");
  }
  if (server_.delivery.pollIntervalMilliseconds == 0)
  {
    throw std::runtime_error("delivery.poll_interval_ms must be greater than zero");
  }
  if (server_.tls.enabled && (server_.tls.certificatePath.empty() || server_.tls.privateKeyPath.empty()))
  {
    throw std::runtime_error("tls.certificate_path and tls.private_key_path are required "
                             "when TLS is enabled");
  }
}

} // namespace smtp
