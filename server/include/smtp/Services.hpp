#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace smtp {

struct AuthRequest {
    std::string username;
    std::string secret;
};

struct AuthResult {
    bool accepted{false};
    std::string identity;
};

struct MailMessage {
    std::string sender;
    std::string recipient;
    std::string rawContent;
};

struct LookupRequest {
    std::string key;
};

struct LookupResult {
    bool found{false};
    std::string value;
};

// User registration/authentication boundary used by SMTP AUTH behavior.
class IAuthService {
public:
    virtual ~IAuthService() = default;

    virtual AuthResult Authenticate(const AuthRequest& request) = 0;
};

// Database-facing mail storage boundary.
class IMailStorage {
public:
    virtual ~IMailStorage() = default;

    virtual std::string Save(const MailMessage& message) = 0;
    virtual std::optional<MailMessage> Retrieve(std::string_view messageId) = 0;
};

// SQLite-backed storage component planned for later implementation.
class dbSQLite : public IMailStorage {

};

// Cache boundary for data that should not always hit storage or external APIs.
class ICacheService {
public:
    virtual ~ICacheService() = default;

    virtual std::optional<std::string> Get(std::string_view key) = 0;
    virtual void Put(std::string_view key, std::string value) = 0;
};

// Delivery/notification boundary for accepted mail after storage/session handling.
class IDeliveryService {
public:
    virtual ~IDeliveryService() = default;

    virtual void QueueForDelivery(const MailMessage& message) = 0;
};

// Lookup boundary for the external/free lookup API. A mock implementation can
// satisfy this interface until the real API integration is ready.
class ILookupService {
public:
    virtual ~ILookupService() = default;

    virtual LookupResult Lookup(const LookupRequest& request) = 0;
};



}
