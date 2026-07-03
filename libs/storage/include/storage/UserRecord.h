#pragma once

#include <cstdint>
#include <string>

namespace Storage {

enum class UserStatus
{
  Active,
  Disabled
};

struct UserRecord
{
  std::int64_t id{};
  std::string username;
  std::string email;
  std::string password_hash;
  UserStatus status{UserStatus::Active};
  std::string created_at;
};

} // namespace Storage
