#include "../include/mail_storage/Database.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>

namespace Storage {

Database::Database(const std::filesystem::path& path)
{
  const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;

  const std::string database_path = path.u8string();
  sqlite3* raw_connection = nullptr;

  const int result = sqlite3_open_v2(database_path.c_str(), &raw_connection, flags, nullptr);

  ConnectionPtr connection(raw_connection);

  if (result != SQLITE_OK)
  {
    std::string error_message = "Failed to open SQLite database";

    if (raw_connection != nullptr)
    {
      error_message += ": ";
      error_message += sqlite3_errmsg(raw_connection);
    }
    else
    {
      error_message += ": ";
      error_message += sqlite3_errstr(result);
    }

    throw std::runtime_error(error_message);
  }

  m_connection = std::move(connection);
  Configure();
}

void Database::ConnectionDeleter::operator()(sqlite3* connection) const noexcept
{
  if (connection != nullptr)
  {
    sqlite3_close(connection);
  }
}

void Database::Execute(const std::string& sql)
{
  char* error_message = nullptr;

  const int result =
    sqlite3_exec(m_connection.get(), sql.c_str(), nullptr, nullptr, &error_message);

  if (result != SQLITE_OK)
  {
    std::string message = "SQLite execute failed";

    if (error_message != nullptr)
    {
      message += ": ";
      message += error_message;
      sqlite3_free(error_message);
    }
    else
    {
      message += ": ";
      message += sqlite3_errmsg(m_connection.get());
    }

    throw std::runtime_error(message);
  }
}

void Database::Configure()
{
  const int result = sqlite3_busy_timeout(m_connection.get(), 5000);

  if (result != SQLITE_OK)
  {
    throw std::runtime_error(std::string("SQLite busy timeout configuration failed: ") +
                             sqlite3_errmsg(m_connection.get()));
  }

  Execute("PRAGMA foreign_keys = ON;");
  Execute("PRAGMA journal_mode = WAL;");
  Execute("PRAGMA synchronous = NORMAL;");
}

} // namespace Storage
