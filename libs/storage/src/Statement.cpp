#include "storage/Statement.h"

#include <cstdint>
#include <stdexcept>
#include <string>

namespace Storage {

Statement::Statement(Database& database, const std::string& sql)
{
  sqlite3_stmt* raw_statement = nullptr;
  const int result =
    sqlite3_prepare_v2(database.m_connection.get(), sql.c_str(), -1, &raw_statement, nullptr);

  m_statement.reset(raw_statement);

  if (result != SQLITE_OK)
  {
    throw std::runtime_error(std::string("Failed to prepare SQLite statement: ") +
                             sqlite3_errmsg(database.m_connection.get()));
  }
}

void Statement::StatementDeleter::operator()(sqlite3_stmt* statement) const noexcept
{
  if (statement != nullptr)
  {
    sqlite3_finalize(statement);
  }
}

void Statement::BindInt(int index, int value)
{
  if (sqlite3_bind_int(m_statement.get(), index, value) != SQLITE_OK)
  {
    ThrowError("Failed to bind integer");
  }
}

void Statement::BindInt64(int index, std::int64_t value)
{
  if (sqlite3_bind_int64(m_statement.get(), index, value) != SQLITE_OK)
  {
    ThrowError("Failed to bind 64-bit integer");
  }
}

void Statement::BindText(int index, const std::string& value)
{
  const int result =
    sqlite3_bind_text(m_statement.get(), index, value.c_str(), -1, SQLITE_TRANSIENT);

  if (result != SQLITE_OK)
  {
    ThrowError("Failed to bind text");
  }
}

void Statement::BindNull(int index)
{
  if (sqlite3_bind_null(m_statement.get(), index) != SQLITE_OK)
  {
    ThrowError("Failed to bind null");
  }
}

bool Statement::Step()
{
  const int result = sqlite3_step(m_statement.get());

  if (result == SQLITE_ROW)
  {
    return true;
  }

  if (result == SQLITE_DONE)
  {
    return false;
  }

  ThrowError("Failed to execute SQLite statement");
  return false;
}

std::int64_t Statement::ColumnInt64(int index) const
{
  return sqlite3_column_int64(m_statement.get(), index);
}

std::string Statement::ColumnText(int index) const
{
  const unsigned char* value = sqlite3_column_text(m_statement.get(), index);

  if (value == nullptr)
  {
    return {};
  }

  return reinterpret_cast<const char*>(value);
}

bool Statement::ColumnIsNull(int index) const
{
  return sqlite3_column_type(m_statement.get(), index) == SQLITE_NULL;
}

std::int64_t Statement::LastInsertRowId() const
{
  return sqlite3_last_insert_rowid(sqlite3_db_handle(m_statement.get()));
}

int Statement::ChangedRowCount() const
{
  return sqlite3_changes(sqlite3_db_handle(m_statement.get()));
}

void Statement::ThrowError(const std::string& message) const
{
  throw std::runtime_error(message + ": " + sqlite3_errmsg(sqlite3_db_handle(m_statement.get())));
}

} // namespace Storage
