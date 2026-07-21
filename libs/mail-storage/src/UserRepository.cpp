#include "../include/mail_storage/UserRepository.h"

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/mail_storage/Statement.h"

namespace Storage {
UserRepository::UserRepository(Database& database) : m_database(database)
{
}

std::int64_t
UserRepository::CreateUser(const std::string& username, const std::string& email, const std::string& password_hash)
{
  Statement statement(m_database,
                      R"SQL(
            INSERT INTO users (
                username,
                email,
                password_hash
            )
            VALUES (?, ?, ?);
        )SQL");

  statement.BindText(1, username);
  statement.BindText(2, email);
  statement.BindText(3, password_hash);
  statement.Step();

  return statement.LastInsertRowId();
}

std::optional<UserRecord> UserRepository::FindById(std::int64_t user_id) const
{
  Statement statement(m_database,
                      R"SQL(
            SELECT
                id,
                username,
                email,
                password_hash,
                status,
                created_at
            FROM users
            WHERE id = ?
            LIMIT 1;
        )SQL");

  statement.BindInt64(1, user_id);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadUser(statement);
}

std::optional<UserRecord> UserRepository::FindByEmail(const std::string& email) const
{
  Statement statement(m_database,
                      R"SQL(
            SELECT
                id,
                username,
                email,
                password_hash,
                status,
                created_at
            FROM users
            WHERE email = ?
            LIMIT 1;
        )SQL");

  statement.BindText(1, email);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadUser(statement);
}

std::optional<UserRecord> UserRepository::FindByUsername(const std::string& username) const
{
  Statement statement(m_database,
                      R"SQL(
            SELECT
                id,
                username,
                email,
                password_hash,
                status,
                created_at
            FROM users
            WHERE username = ?
            LIMIT 1;
        )SQL");

  statement.BindText(1, username);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadUser(statement);
}

bool UserRepository::UpdateStatus(std::int64_t user_id, UserStatus status)
{
  Statement statement(m_database,
                      R"SQL(
            UPDATE users
            SET status = ?
            WHERE id = ?;
        )SQL");

  statement.BindText(1, StatusToString(status));
  statement.BindInt64(2, user_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

UserRecord UserRepository::ReadUser(const Statement& statement) const
{
  UserRecord user;
  user.id = statement.ColumnInt64(0);
  user.username = statement.ColumnText(1);
  user.email = statement.ColumnText(2);
  user.password_hash = statement.ColumnText(3);
  user.status = StatusFromString(statement.ColumnText(4));
  user.created_at = statement.ColumnText(5);

  return user;
}

std::string UserRepository::StatusToString(UserStatus status) const
{
  switch (status)
  {
  case UserStatus::Active:
    return "active";
  case UserStatus::Disabled:
    return "disabled";
  }

  throw std::runtime_error("Unsupported user status");
}

UserStatus UserRepository::StatusFromString(const std::string& status) const
{
  if (status == "active")
  {
    return UserStatus::Active;
  }

  if (status == "disabled")
  {
    return UserStatus::Disabled;
  }

  throw std::runtime_error("Unsupported user status: " + status);
}

bool UserRepository::HasUsers()
{
  Statement statement(m_database, "SELECT COUNT(*) FROM users;");

  if (statement.Step())
  {
    return statement.ColumnInt64(0) > 0;
  }
  return false;
}

std::optional<UserRecord> UserRepository::FindActiveUser() const
{
  Statement statement(m_database,
                      R"SQL(
            SELECT
                id,
                username,
                email,
                password_hash,
                status,
                created_at
            FROM users
            WHERE status = 'active'
            LIMIT 1;
        )SQL");

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadUser(statement);
}

std::vector<UserRecord> UserRepository::FindAll() const
{
  std::vector<UserRecord> users;
  Statement statement(m_database, "SELECT id, username, email, password_hash, status, created_at FROM users;");
  while (statement.Step())
  {
    users.push_back(ReadUser(statement));
  }
  return users;
}

} // namespace Storage
