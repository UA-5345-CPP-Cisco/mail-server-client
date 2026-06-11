#include "smtp/Services.hpp"

#include <algorithm>
#include <cctype>
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

std::string NormalizeAccountKey(std::string_view value)
{
    std::string result(value);
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char character) {
        return !std::isspace(character);
    }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char character) {
        return !std::isspace(character);
    }).base(), result.end());

    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return result;
}

}

AuthService::AuthService(std::unordered_map<std::string, std::string> users)
    : users_(std::move(users))
{
}

void AuthService::AddUser(std::string username, std::string secret)
{
    std::lock_guard<std::mutex> lock(mutex_);
    users_[NormalizeAccountKey(username)] = std::move(secret);
}

AuthResult AuthService::Authenticate(const AuthRequest& request)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const std::string identity = NormalizeAccountKey(request.username);
    if (identity.empty()) {
        return {};
    }

    const auto user = users_.find(identity);
    if (user == users_.end()) {
        users_[identity] = request.secret;
        return AuthResult{true, identity, true};
    }

    if (user->second != request.secret) {
        return {};
    }

    return AuthResult{true, identity, false};
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

    for (const std::string& recipient : message.recipients) {
        const std::string key = NormalizeAccountKey(recipient);
        if (!key.empty()) {
            recipientMessageIds_[key].push_back(id);
        }
    }

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

std::size_t dbSQLite::CountForRecipient(std::string_view recipient)
{
    std::lock_guard<std::mutex> lock(mutex_);
    const auto mailbox = recipientMessageIds_.find(NormalizeAccountKey(recipient));
    if (mailbox == recipientMessageIds_.end()) {
        return 0;
    }

    return mailbox->second.size();
}

std::vector<StoredMailMessage> dbSQLite::ListForRecipient(std::string_view recipient,
                                                          std::size_t first,
                                                          std::size_t last)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<StoredMailMessage> result;

    if (first == 0 || last < first) {
        return result;
    }

    const auto mailbox = recipientMessageIds_.find(NormalizeAccountKey(recipient));
    if (mailbox == recipientMessageIds_.end()) {
        return result;
    }

    const std::vector<std::string>& ids = mailbox->second;
    if (first > ids.size()) {
        return result;
    }

    last = std::min(last, ids.size());
    result.reserve(last - first + 1);

    for (std::size_t index = first; index <= last; ++index) {
        const std::string& id = ids[index - 1];
        const auto message = messages_.find(id);
        if (message != messages_.end()) {
            result.push_back(StoredMailMessage{index, id, message->second});
        }
    }

    return result;
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

    if (blocked_.contains(request.key)) {
        return {};
    }

    if (allowed_.empty() || allowed_.contains(request.key)) {
        return LookupResult{true, request.key};
    }

    return {};
}

}
