#pragma once

#include <cstdint>
#include <string>

namespace smtp {

struct ConnectionId {
    std::uint64_t value{0};
};

inline bool operator==(ConnectionId left, ConnectionId right) noexcept
{
    return left.value == right.value;
}

enum class SmtpEventType {
    Connected,
    MessageReceived,
    Disconnected
};

struct SmtpEvent {
    SmtpEventType type{SmtpEventType::MessageReceived};
    ConnectionId connectionId;
    std::string payload;
};

}
