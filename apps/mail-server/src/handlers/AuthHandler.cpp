#include <optional>
#include <stdexcept>
#include <string>

#include <cryptopp/argon2.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <mail_server/handlers/AuthHandler.h>
#include <mail_server/services/ServiceRegistry.h>

namespace ISXMailServer {

namespace {

std::string ReadRequiredString(const json::object& object, std::string_view field)
{
  auto const* value = object.if_contains(field);
  if (value == nullptr || !value->is_string())
  {
    throw std::runtime_error("\"" + std::string{field} + "\" must be a string");
  }

  std::string result{value->as_string().c_str()};
  if (result.empty())
  {
    throw std::runtime_error("\"" + std::string{field} + "\" must not be empty");
  }

  return result;
}

std::string ToHex(const CryptoPP::byte* data, std::size_t size)
{
  std::string hex;
  CryptoPP::StringSource(data, size, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex)));
  return hex;
}

std::string FromHex(const std::string& hex)
{
  std::string bytes;
  CryptoPP::StringSource(hex, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(bytes)));
  return bytes;
}

std::string HashPassword(const std::string& password)
{
  CryptoPP::byte salt[16];
  CryptoPP::AutoSeededRandomPool random;
  random.GenerateBlock(salt, sizeof(salt));

  CryptoPP::byte hash[32];
  CryptoPP::Argon2 argon2(CryptoPP::Argon2::ARGON2ID);
  argon2.DeriveKey(hash,
                   sizeof(hash),
                   reinterpret_cast<const CryptoPP::byte*>(password.data()),
                   password.size(),
                   salt,
                   sizeof(salt),
                   3,
                   65536,
                   4);

  return ToHex(salt, sizeof(salt)) + ":" + ToHex(hash, sizeof(hash));
}

bool VerifyPassword(const std::string& password, const std::string& stored_hash)
{
  const std::size_t separator = stored_hash.find(':');
  if (separator == std::string::npos)
  {
    return password == stored_hash;
  }

  const std::string salt = FromHex(stored_hash.substr(0, separator));
  const std::string expected_hash = stored_hash.substr(separator + 1);

  CryptoPP::byte actual_hash[32];
  CryptoPP::Argon2 argon2(CryptoPP::Argon2::ARGON2ID);
  argon2.DeriveKey(actual_hash,
                   sizeof(actual_hash),
                   reinterpret_cast<const CryptoPP::byte*>(password.data()),
                   password.size(),
                   reinterpret_cast<const CryptoPP::byte*>(salt.data()),
                   salt.size(),
                   3,
                   65536,
                   4);

  return ToHex(actual_hash, sizeof(actual_hash)) == expected_hash;
}

json::object ReadJsonObject(Request const& request)
{
  beast::error_code error;
  json::value input = json::parse(request.body(), error);

  if (error)
  {
    throw std::runtime_error("Invalid JSON");
  }

  if (!input.is_object())
  {
    throw std::runtime_error("Expected a JSON object");
  }

  return input.as_object();
}

json::object UserResponse(const json::object& user)
{
  return json::object{{"user", user}};
}

} // namespace

Response RegisterHandler(Request const& request)
{
  try
  {
    const json::object object = ReadJsonObject(request);
    const std::string username = ReadRequiredString(object, "username");
    const std::string email = ReadRequiredString(object, "email");
    const std::string password = ReadRequiredString(object, "password");

    const json::object user =
      ServiceRegistry::Storage().CreateUser(username, email, HashPassword(password));
    ServiceRegistry::Logger().Log(LogLevel::Info, "Registered user " + email);

    return MakeJsonResponse(request, http::status::created, UserResponse(user));
  }
  catch (const std::exception& exception)
  {
    return MakeError(request, http::status::bad_request, exception.what());
  }
}

Response LoginHandler(Request const& request)
{
  try
  {
    const json::object object = ReadJsonObject(request);
    const std::string email = ReadRequiredString(object, "email");
    const std::string password = ReadRequiredString(object, "password");

    const std::optional<Storage::UserRecord> user =
      ServiceRegistry::Storage().FindUserByEmail(email);
    if (!user.has_value() || user->status != Storage::UserStatus::Active ||
        !VerifyPassword(password, user->password_hash))
    {
      return MakeError(request, http::status::unauthorized, "Invalid email or password");
    }

    ServiceRegistry::Logger().Log(LogLevel::Info, "Logged in user " + email);

    return MakeJsonResponse(
      request,
      http::status::ok,
      UserResponse(
        json::object{{"id", user->id}, {"username", user->username}, {"email", user->email}}));
  }
  catch (const std::exception& exception)
  {
    return MakeError(request, http::status::bad_request, exception.what());
  }
}

} // namespace ISXMailServer
