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

//register, auth
class IAuthService {
public:
    virtual ~IAuthService() = default;

    virtual AuthResult Authenticate(const AuthRequest& request) = 0;
};

//db
class IMailStorage {
public:
    virtual ~IMailStorage() = default;

    virtual std::string Save(const MailMessage& message) = 0;
    virtual std::optional<MailMessage> Retrieve(std::string_view messageId) = 0;
};

//cache
class ICacheService {
public:
    virtual ~ICacheService() = default;

    virtual std::optional<std::string> Get(std::string_view key) = 0;
    virtual void Put(std::string_view key, std::string value) = 0;
};

//notifications, delivery
class IDeliveryService {
public:
    virtual ~IDeliveryService() = default;

    virtual void QueueForDelivery(const MailMessage& message) = 0;
};

//lookup (make mock for now)
class ILookupService {
public:
    virtual ~ILookupService() = default;

    virtual LookupResult Lookup(const LookupRequest& request) = 0;
};

}
