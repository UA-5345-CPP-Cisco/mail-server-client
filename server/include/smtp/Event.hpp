#pragma once

#include <cstdint>
#include <string>

namespace smtp {

// Stable identifier for one client connection.
typedef std::uint64_t ConnectionId;

inline bool operator==(ConnectionId left, ConnectionId right) noexcept
{
    return left.value == right.value;
}

enum class SmtpEventType {
    Connected,
    MessageReceived,
    Disconnected
};

// Complete event produced by the sockets manager and routed to one session.
// The session/protocol layer decides what a received message means.
struct SmtpEvent {
    SmtpEventType type{SmtpEventType::MessageReceived};
    ConnectionId connectionId;
    std::string payload;
};

}
