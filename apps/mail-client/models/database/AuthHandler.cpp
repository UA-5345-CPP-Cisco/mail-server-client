#include "headers/database/AuthHandler.h"

#include <cryptopp/argon2.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>

#include "mail_storage/UserRepository.h"
using namespace CryptoPP;

AuthHandler::AuthHandler(Storage::Database& db, QObject* parent) : QObject(parent), m_db(db)
{
}

// ----- HELPER FUNCTION TO CONVERT BYTE ARRAY TO HEX STRING -----
std::string toHex(const CryptoPP::byte* data, size_t size)
{
  std::string hex;
  StringSource(data, size, true, new HexEncoder(new StringSink(hex)));
  return hex;
}

// ----- HELPER FUNCTION TO CONVERT HEX STRING TO BYTE ARRAY -----
std::vector<CryptoPP::byte> fromHex(const std::string& hex)
{
  std::vector<CryptoPP::byte> bytes;

  StringSource(hex, true, new HexDecoder(new VectorSink(bytes)));

  return bytes;
}

// ----- REGISTRATION -----
bool AuthHandler::registerUser(const QString& username,
                               const QString& email,
                               const QString& password)
{
  byte salt[16];
  AutoSeededRandomPool crypto_random;
  crypto_random.GenerateBlock(salt, sizeof(salt));

  byte hash[32];
  Argon2 argon2(Argon2::ARGON2ID);

  std::string raw_password = password.toStdString();
  argon2.DeriveKey(hash,
                   sizeof(hash),
                   reinterpret_cast<const byte*>(raw_password.data()),
                   raw_password.size(),
                   salt,
                   sizeof(salt),
                   3,
                   65536,
                   4);

  std::string hash_hex = toHex(hash, sizeof(hash));
  std::string salt_hex = toHex(salt, sizeof(salt));

  Storage::UserRepository repo(m_db);
  std::string combined = salt_hex + ":" + hash_hex;

  return repo.CreateUser(username.toStdString(), email.toStdString(), combined) != -1;
}

// ----- LOGIN -----
AuthHandler::AuthResult AuthHandler::loginUser(const QString& email, const QString& password)
{
  auto user_data = getMockUser(email.toStdString());

  if (!user_data)
  {
    return AuthResult::UserNotFound;
  }

  std::string combined = user_data->passwordData;
  std::string username = user_data->username;

  if (!verifyPassword(password, combined))
  {
    return AuthResult::WrongPassword;
  }

  Storage::UserRepository repo(m_db);

  if (repo.UserExists(email.toStdString()))
  {
    return AuthResult::UserAlreadyExists;
  }

  if (repo.CreateUser(username, email.toStdString(), combined) == -1)
  {
    return AuthResult::DatabaseError;
  }

  m_currentUserName = QString::fromStdString(username);

  return AuthResult::Success;
}

// ---- PASSWORD VERIFICATION FUNCTION ----
bool AuthHandler::verifyPassword(const QString& password, const std::string& storedPasswordData)
{
  size_t colonPos = storedPasswordData.find(':');

  if (colonPos == std::string::npos)
  {
    return false;
  }

  std::string saltHex = storedPasswordData.substr(0, colonPos);
  std::string hashHex = storedPasswordData.substr(colonPos + 1);

  std::vector<byte> salt = fromHex(saltHex);
  std::vector<byte> storedHash = fromHex(hashHex);

  byte hash[32];
  Argon2 argon2(Argon2::ARGON2ID);

  std::string rawPassword = password.toStdString();

  argon2.DeriveKey(hash,
                   sizeof(hash),
                   reinterpret_cast<const byte*>(rawPassword.data()),
                   rawPassword.size(),
                   salt.data(),
                   salt.size(),
                   3,
                   65536,
                   4);

  return storedHash.size() == sizeof(hash) &&
         CryptoPP::VerifyBufsEqual(hash, storedHash.data(), sizeof(hash));
}

// ----- MOCK USER DATA -----
std::optional<AuthHandler::MockUser> AuthHandler::getMockUser(const std::string& email)
{
  static const MockUser users[] = {
    {"test1@user.com",
     "Rainbow Dash",
     "F458979513E497DDA7A5EA069BFEADAF:"
     "9ADF582FFC589AB9B9E6E809C96BE6D6EB6417371BE84BDFE9862DE5D23EBC3F"},

    {"test2@user.com",
     "Fluttershy",
     "330BDA3176F1050E100C982CA6B330D4:"
     "5505897DBB3D8EDF105A38BC1C2865A25FFA377A3914EEC96CDA699CF524A506"}};

  for (const MockUser& user : users)
  {
    if (user.email == email)
    {
      return user;
    }
  }

  return std::nullopt;
}

QString AuthHandler::getLastLoggedInName() const
{
  return m_currentUserName;
}