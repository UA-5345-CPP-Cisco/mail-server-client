#include "smtp/Services.hpp"

#include <algorithm>
#include <sstream>
#include <utility>

namespace smtp {

namespace {

std::string ToString(std::uint64_t value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

}

AuthService::AuthService(std::unordered_map<std::string, std::string> users)
    : users_(std::move(users))
{
}

void AuthService::AddUser(std::string username, std::string secret)
{
    std::lock_guard<std::mutex> lock(mutex_);
    users_[std::move(username)] = std::move(secret);
}

AuthResult AuthService::Authenticate(const AuthRequest& request)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const auto user = users_.find(request.username);
    if (user == users_.end()) {
        return {};
    }

    if (user->second != request.secret) {
        return {};
    }

    return AuthResult{true, request.username};
}

dbSQLite::dbSQLite(dbConfig config)
    : config_(std::move(config))
{
}

std::string dbSQLite::Save(const MailMessage& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const std::string id = ToString(nextMessageId_++);
    messages_.emplace(id, message);
    return id;
}

std::optional<MailMessage> dbSQLite::Retrieve(std::string_view messageId)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const auto message = messages_.find(std::string(messageId));
    if (message == messages_.end()) {
        return std::nullopt;
    }

    return message->second;
}

std::optional<std::string> CacheService::Get(std::string_view key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const auto value = values_.find(std::string(key));
    if (value == values_.end()) {
        return std::nullopt;
    }

    return value->second;
}

void CacheService::Put(std::string_view key, std::string value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    values_[std::string(key)] = std::move(value);
}

void DeliveryService::QueueForDelivery(const MailMessage& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    queuedMessages_.push(message);
}

void LookupService::Allow(std::string key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    allowed_.insert(std::move(key));
}

void LookupService::Block(std::string key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    blocked_.insert(std::move(key));
}

LookupResult LookupService::Lookup(const LookupRequest& request)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (request.key.empty()) {
        return {};
    }

    if (blocked_.find(request.key) != blocked_.end()) {
        return {};
    }

    if (
        allowed_.empty() ||
        allowed_.find(request.key) != allowed_.end()
    ) {
        return LookupResult{true, request.key};
    }

    return {};
}

}
