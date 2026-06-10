#pragma once

#include "smtp/ServerConfig.hpp"

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

// Concrete authentication component planned for later implementation.
// It should validate SMTP AUTH credentials and return the authenticated identity.
class AuthService : public IAuthService {
public:
    AuthResult Authenticate(const AuthRequest& request) override;
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
public:
    explicit dbSQLite(dbConfig config);

    std::string Save(const MailMessage& message) override;
    std::optional<MailMessage> Retrieve(std::string_view messageId) override;

private:
    dbConfig config_;
};

// Cache boundary for data that should not always hit storage or external APIs.
class ICacheService {
public:
    virtual ~ICacheService() = default;

    virtual std::optional<std::string> Get(std::string_view key) = 0;
    virtual void Put(std::string_view key, std::string value) = 0;
};

// Concrete cache component planned for later implementation.
// It should hide the chosen cache backend from SMTP protocol code.
class CacheService : public ICacheService {
public:
    std::optional<std::string> Get(std::string_view key) override;
    void Put(std::string_view key, std::string value) override;
};

// Delivery/notification boundary for accepted mail after storage/session handling.
class IDeliveryService {
public:
    virtual ~IDeliveryService() = default;

    virtual void QueueForDelivery(const MailMessage& message) = 0;
};

// Concrete delivery component planned for later implementation.
// It should queue accepted mail for downstream delivery or notification work.
class DeliveryService : public IDeliveryService {
public:
    void QueueForDelivery(const MailMessage& message) override;
};

// Lookup boundary for the external/free lookup API. A mock implementation can
// satisfy this interface until the real API integration is ready.
class ILookupService {
public:
    virtual ~ILookupService() = default;

    virtual LookupResult Lookup(const LookupRequest& request) = 0;
};

// External/free lookup API component planned for later implementation.
// It should isolate HTTP/API details from SMTP session handling.
class LookupService : public ILookupService {
public:
    LookupResult Lookup(const LookupRequest& request) override;
};

}
