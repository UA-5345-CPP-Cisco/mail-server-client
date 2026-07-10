#include "headers/database/RegistrationHandler.h"
#include "../../../../libs/storage/include/storage/UserRepository.h"

#include <cryptopp/argon2.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
using namespace CryptoPP;

RegistrationHandler::RegistrationHandler(Storage::Database& db, QObject* parent) : QObject(parent), m_db(db) {}

std::string toHex(const CryptoPP::byte* data, size_t size) 
{
    std::string hex;
    StringSource(data, size, true, new HexEncoder(new StringSink(hex)));
    return hex;
}

bool RegistrationHandler::registerUser(const QString& username, const QString& email, const QString& password)
{
    byte salt[16];
    AutoSeededRandomPool crypto_random;
    crypto_random.GenerateBlock(salt, sizeof(salt));

    byte hash[32];
    Argon2 argon2(Argon2::ARGON2ID);

    std::string raw_password = password.toStdString();

    argon2.DeriveKey
    (
        hash, sizeof(hash),
        reinterpret_cast<const byte*>(raw_password.data()), raw_password.size(),
        salt, sizeof(salt),
        3,
        65536,
        4
    );

    std::string hash_hex = toHex(hash, sizeof(hash));
    std::string salt_hex = toHex(salt, sizeof(salt));

    Storage::UserRepository repo(m_db);
    std::string combined = salt_hex + ":" + hash_hex;

    return repo.CreateUser(username.toStdString(), email.toStdString(), combined) != -1;
}