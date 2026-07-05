#pragma once

#include <cstdint>
#include <string>

namespace smtp {

// Stable identifier for one client connection.
using ConnectionId = std::uint64_t;

enum class SmtpEventType
{
  Connected,
  MessageReceived,
  TlsSucceeded,
  TlsFailed,
  Disconnected
};

// Complete event produced by the sockets manager and routed to one session.
// The session/protocol layer decides what a received message means.
struct SmtpEvent
{
  SmtpEventType type{SmtpEventType::MessageReceived};
  ConnectionId connectionId{0};
  std::string payload;
};

} // namespace smtp
