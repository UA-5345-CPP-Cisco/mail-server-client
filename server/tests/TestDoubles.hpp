#pragma once

#include "smtp/Logger.hpp"
#include "smtp/Services.hpp"
#include "smtp/Session.hpp"
#include "smtp/SocketsManager.hpp"
#include "smtp/ThreadPool.hpp"

#include <algorithm>
#include <cstddef>
#include <future>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace smtp::test {

struct SentMessage {
    ConnectionId connectionId{0};
    std::string text;
};

class FakeSocketsManager : public ISocketsManager {
public:
    void Start(const ServerConfig& config) override
    {
        started = true;
        startedConfig = config;
    }

    std::vector<SmtpEvent> PollEvents() override
    {
        std::vector<SmtpEvent> result = std::move(pendingEvents);
        pendingEvents.clear();
        return result;
    }

    void Send(ConnectionId connectionId, std::string_view message) override
    {
        sentMessages.push_back(SentMessage{connectionId, std::string(message)});
    }

    void Close(ConnectionId connectionId) override
    {
        closedConnections.push_back(connectionId);
    }

    void StartTls(ConnectionId connectionId) override
    {
        tlsStarts.push_back(connectionId);
    }

    void Continue(ConnectionId connectionId) override
    {
        continuedConnections.push_back(connectionId);
    }

    void Stop() override
    {
        stopped = true;
    }

    std::vector<std::string> SentTexts(ConnectionId connectionId) const
    {
        std::vector<std::string> result;
        for (const SentMessage& message : sentMessages) {
            if (message.connectionId == connectionId) {
                result.push_back(message.text);
            }
        }
        return result;
    }

    std::string LastSentText(ConnectionId connectionId) const
    {
        std::string result;
        for (const SentMessage& message : sentMessages) {
            if (message.connectionId == connectionId) {
                result = message.text;
            }
        }
        return result;
    }

    bool started{false};
    bool stopped{false};
    ServerConfig startedConfig;
    std::vector<SmtpEvent> pendingEvents;
    std::vector<SentMessage> sentMessages;
    std::vector<ConnectionId> closedConnections;
    std::vector<ConnectionId> tlsStarts;
    std::vector<ConnectionId> continuedConnections;
};

class ImmediateThreadPool : public IThreadPool {
public:
    std::future<void> Enqueue(std::function<void()> task) override
    {
        ++enqueueCount;

        std::packaged_task<void()> packagedTask(std::move(task));
        std::future<void> future = packagedTask.get_future();
        packagedTask();
        return future;
    }

    int enqueueCount{0};
};

class FakeAuthService : public IAuthService {
public:
    AuthResult Authenticate(const AuthRequest& request) override
    {
        if (throwOnAuthenticate) {
            throw std::runtime_error("auth failure");
        }

        requests.push_back(request);
        if (!accepted) {
            return {};
        }

        return AuthResult{true, identity.empty() ? request.username : identity};
    }

    bool accepted{true};
    bool throwOnAuthenticate{false};
    std::string identity;
    std::vector<AuthRequest> requests;
};

class FakeMailStorage : public IMailStorage {
public:
    std::string Save(const MailMessage& message) override
    {
        if (throwOnSave) {
            throw std::runtime_error("storage failure");
        }

        savedMessages.push_back(message);
        messages[nextId] = message;
        return nextId;
    }

    std::optional<MailMessage> Retrieve(std::string_view messageId) override
    {
        const auto iterator = messages.find(std::string(messageId));
        if (iterator == messages.end()) {
            return std::nullopt;
        }

        return iterator->second;
    }

    std::size_t CountForRecipient(std::string_view recipient) override
    {
        const auto iterator = mailboxMessages.find(std::string(recipient));
        if (iterator == mailboxMessages.end()) {
            return 0;
        }

        return iterator->second.size();
    }

    std::vector<StoredMailMessage> ListForRecipient(std::string_view recipient,
                                                    std::size_t first,
                                                    std::size_t last) override
    {
        const auto iterator = mailboxMessages.find(std::string(recipient));
        if (iterator == mailboxMessages.end() || first == 0 || last < first) {
            return {};
        }

        const std::vector<StoredMailMessage>& messagesForRecipient = iterator->second;
        if (first > messagesForRecipient.size()) {
            return {};
        }

        last = std::min(last, messagesForRecipient.size());
        return std::vector<StoredMailMessage>(messagesForRecipient.begin() + static_cast<std::ptrdiff_t>(first - 1),
                                              messagesForRecipient.begin() + static_cast<std::ptrdiff_t>(last));
    }

    bool throwOnSave{false};
    std::string nextId{"42"};
    std::vector<MailMessage> savedMessages;
    std::unordered_map<std::string, MailMessage> messages;
    std::unordered_map<std::string, std::vector<StoredMailMessage>> mailboxMessages;
};

class FakeCacheService : public ICacheService {
public:
    std::optional<std::string> Get(std::string_view key) override
    {
        const auto iterator = values.find(std::string(key));
        if (iterator == values.end()) {
            return std::nullopt;
        }

        return iterator->second;
    }

    void Put(std::string_view key, std::string value) override
    {
        values[std::string(key)] = std::move(value);
    }

    std::unordered_map<std::string, std::string> values;
};

class FakeDeliveryService : public IDeliveryService {
public:
    void QueueForDelivery(const MailMessage& message) override
    {
        queuedMessages.push_back(message);
    }

    std::vector<MailMessage> queuedMessages;
};

class FakeLookupService : public ILookupService {
public:
    LookupResult Lookup(const LookupRequest& request) override
    {
        requests.push_back(request);
        if (!found) {
            return {};
        }

        return LookupResult{true, value.empty() ? request.key : value};
    }

    bool found{true};
    std::string value;
    std::vector<LookupRequest> requests;
};

class RecordingLogger : public ILogger {
public:
    void Log(LogLevel level, std::string_view message) override
    {
        entries.push_back(Entry{level, std::string(message)});
    }

    struct Entry {
        LogLevel level;
        std::string message;
    };

    std::vector<Entry> entries;
};

struct HandlerHarness {
    HandlerHarness()
        : state{connectionId},
          context{
              config,
              sockets,
              auth,
              storage,
              cache,
              delivery,
              lookup,
              logger
          }
    {
        config.serverName = "mx.test";
    }

    void Deliver(SmtpEventType type, std::string payload = {})
    {
        handler.HandleEvent(SmtpEvent{type, connectionId, std::move(payload)}, state, context);
    }

    static constexpr ConnectionId connectionId{7};

    ServerConfig config;
    FakeSocketsManager sockets;
    FakeAuthService auth;
    FakeMailStorage storage;
    FakeCacheService cache;
    FakeDeliveryService delivery;
    FakeLookupService lookup;
    RecordingLogger logger;
    SmtpSessionHandler handler;
    SmtpSessionState state;
    SmtpSessionContext context;
};

}
