#include <algorithm>
#include <array>
#include <cctype>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string_view>

#include "smtp/Session.hpp"

namespace smtp {

namespace {

std::string Trim(std::string_view value)
{
  while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front())))
  {
    value.remove_prefix(1);
  }

  while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())))
  {
    value.remove_suffix(1);
  }

  return std::string(value);
}

std::string ToUpper(std::string value)
{
  std::transform(value.begin(),
                 value.end(),
                 value.begin(),
                 [](unsigned char character) { return static_cast<char>(std::toupper(character)); });
  return value;
}

bool StartsWithCaseInsensitive(std::string_view value, std::string_view prefix)
{
  if (value.size() < prefix.size())
  {
    return false;
  }

  for (std::size_t index = 0; index < prefix.size(); ++index)
  {
    if (std::toupper(static_cast<unsigned char>(value[index])) !=
        std::toupper(static_cast<unsigned char>(prefix[index])))
    {
      return false;
    }
  }

  return true;
}

std::string ExtractPathArgument(std::string_view commandArgument)
{
  std::string value = Trim(commandArgument);

  const std::size_t colon = value.find(':');
  if (colon != std::string::npos)
  {
    value = Trim(std::string_view(value).substr(colon + 1));
  }

  const std::size_t parameters = value.find(' ');
  if (parameters != std::string::npos)
  {
    value = value.substr(0, parameters);
  }

  if (value.size() >= 2 && value.front() == '<' && value.back() == '>')
  {
    value = value.substr(1, value.size() - 2);
  }

  return value;
}

std::optional<std::string> DecodeBase64(std::string_view input)
{
  static const std::array<int, 256> table = []
  {
    std::array<int, 256> values{};
    values.fill(-1);
    for (int character = 'A'; character <= 'Z'; ++character)
    {
      values[static_cast<std::size_t>(character)] = character - 'A';
    }
    for (int character = 'a'; character <= 'z'; ++character)
    {
      values[static_cast<std::size_t>(character)] = character - 'a' + 26;
    }
    for (int character = '0'; character <= '9'; ++character)
    {
      values[static_cast<std::size_t>(character)] = character - '0' + 52;
    }
    values[static_cast<std::size_t>('+')] = 62;
    values[static_cast<std::size_t>('/')] = 63;
    return values;
  }();

  std::string output;
  int buffer = 0;
  int bits = -8;

  for (unsigned char character : input)
  {
    if (std::isspace(character))
    {
      continue;
    }
    if (character == '=')
    {
      break;
    }

    const int value = table[character];
    if (value < 0)
    {
      return std::nullopt;
    }

    buffer = (buffer << 6) | value;
    bits += 6;

    if (bits >= 0)
    {
      output.push_back(static_cast<char>((buffer >> bits) & 0xFF));
      bits -= 8;
    }
  }

  return output;
}

bool ParsePlainAuth(std::string_view encoded, std::string& username, std::string& secret)
{
  const std::optional<std::string> decoded = DecodeBase64(encoded);
  if (!decoded)
  {
    return false;
  }

  const std::string& value = *decoded;
  const std::size_t firstNull = value.find('\0');
  if (firstNull == std::string::npos)
  {
    return false;
  }

  const std::size_t secondNull = value.find('\0', firstNull + 1);
  if (secondNull == std::string::npos)
  {
    return false;
  }

  username = value.substr(firstNull + 1, secondNull - firstNull - 1);
  secret = value.substr(secondNull + 1);
  return !username.empty();
}

bool IsAuthAllowed(const SmtpSessionState& state, const SmtpSessionContext& context)
{
  return state.tlsActive || context.config.allowPlainAuthenticationWithoutTls;
}

void MarkAuthenticated(SmtpSessionState& state, const AuthResult& result)
{
  state.authenticated = result.accepted;
  state.authenticatedIdentity = result.identity;
  state.authStage = SmtpAuthStage::None;
  state.pendingAuthUsername.clear();
}

} // namespace

void SmtpSessionHandler::HandleEvent(const SmtpEvent& event, SmtpSessionState& state, SmtpSessionContext& context)
{
  switch (event.type)
  {
  case SmtpEventType::Connected:
    HandleConnected(event, state, context);
    break;
  case SmtpEventType::MessageReceived:
    HandleMessageReceived(event, state, context);
    break;
  case SmtpEventType::TlsSucceeded:
    state.tlsActive = true;
    state.tlsHandshakePending = false;
    state.clientName.clear();
    ResetMailTransaction(state);
    state.phase = SmtpSessionPhase::WaitingForGreeting;
    break;
  case SmtpEventType::TlsFailed:
    state.tlsHandshakePending = false;
    state.phase = SmtpSessionPhase::Closed;
    context.socketsManager.Close(state.connectionId);
    break;
  case SmtpEventType::Disconnected:
    HandleDisconnected(event, state, context);
    break;
  }
}

void SmtpSessionHandler::HandleConnected(const SmtpEvent&, SmtpSessionState& state, SmtpSessionContext& context)
{
  state.phase = SmtpSessionPhase::WaitingForGreeting;
  SendReply(context, state.connectionId, 220, context.config.serverName + " ESMTP ready");
}

void SmtpSessionHandler::HandleMessageReceived(const SmtpEvent& event,
                                               SmtpSessionState& state,
                                               SmtpSessionContext& context)
{
  std::string line = Trim(event.payload);

  if (state.authStage == SmtpAuthStage::PlainResponse)
  {
    std::string username;
    std::string secret;
    if (!ParsePlainAuth(line, username, secret))
    {
      SendReply(context, state.connectionId, 501, "Invalid authentication response");
      state.authStage = SmtpAuthStage::None;
      return;
    }

    MarkAuthenticated(state, context.authService.Authenticate(AuthRequest{"PLAIN", username, secret}));
    SendReply(context,
              state.connectionId,
              state.authenticated ? 235 : 535,
              state.authenticated ? "Authentication successful" : "Authentication failed");
    return;
  }

  if (state.authStage == SmtpAuthStage::LoginUsername)
  {
    const std::optional<std::string> username = DecodeBase64(line);
    if (!username || username->empty())
    {
      SendReply(context, state.connectionId, 501, "Invalid username");
      state.authStage = SmtpAuthStage::None;
      return;
    }

    state.pendingAuthUsername = *username;
    state.authStage = SmtpAuthStage::LoginPassword;
    SendReply(context, state.connectionId, 334, "UGFzc3dvcmQ6");
    return;
  }

  if (state.authStage == SmtpAuthStage::LoginPassword)
  {
    const std::optional<std::string> secret = DecodeBase64(line);
    if (!secret)
    {
      SendReply(context, state.connectionId, 501, "Invalid password");
      state.authStage = SmtpAuthStage::None;
      state.pendingAuthUsername.clear();
      return;
    }

    MarkAuthenticated(state,
                      context.authService.Authenticate(AuthRequest{"LOGIN", state.pendingAuthUsername, *secret}));
    SendReply(context,
              state.connectionId,
              state.authenticated ? 235 : 535,
              state.authenticated ? "Authentication successful" : "Authentication failed");
    return;
  }

  if (state.phase == SmtpSessionPhase::ReceivingMessage)
  {
    if (line == ".")
    {
      const std::int64_t messageId = StoreMessage(state, context);
      SendReply(context, state.connectionId, 250, "Message accepted as " + std::to_string(messageId));
      ResetMailTransaction(state);
      return;
    }

    if (StartsWithCaseInsensitive(line, ".."))
    {
      line.erase(line.begin());
    }

    if (state.messageBuffer.size() + line.size() + 2 > context.config.maxMessageSizeBytes)
    {
      SendReply(context, state.connectionId, 552, "Message size exceeds fixed maximum message size");
      ResetMailTransaction(state);
      return;
    }

    state.messageBuffer += line;
    state.messageBuffer += "\r\n";
    return;
  }

  std::string command;
  std::string argument;
  const std::size_t space = line.find(' ');
  if (space == std::string::npos)
  {
    command = ToUpper(line);
  }
  else
  {
    command = ToUpper(line.substr(0, space));
    argument = Trim(std::string_view(line).substr(space + 1));
  }

  if (command == "HELO" || command == "EHLO")
  {
    state.clientName = argument;
    state.phase = SmtpSessionPhase::ReadyForMail;

    if (command == "EHLO")
    {
      context.socketsManager.Send(state.connectionId, "250-" + context.config.serverName + "\r\n");
      if (context.config.tls.enabled && !state.tlsActive)
      {
        context.socketsManager.Send(state.connectionId, "250-STARTTLS\r\n");
      }
      if (!state.authenticated && IsAuthAllowed(state, context))
      {
        context.socketsManager.Send(state.connectionId, "250-AUTH PLAIN LOGIN\r\n");
      }
      context.socketsManager.Send(state.connectionId,
                                  "250 SIZE " + std::to_string(context.config.maxMessageSizeBytes) + "\r\n");
    }
    else
    {
      SendReply(context, state.connectionId, 250, context.config.serverName);
    }
    return;
  }

  if (command == "STARTTLS")
  {
    if (!context.config.tls.enabled)
    {
      SendReply(context, state.connectionId, 454, "TLS not available");
      return;
    }
    if (state.tlsActive || state.tlsHandshakePending)
    {
      SendReply(context, state.connectionId, 503, "TLS is already active");
      return;
    }
    if (state.phase == SmtpSessionPhase::ReceivingMessage)
    {
      SendReply(context, state.connectionId, 503, "Cannot start TLS during DATA");
      return;
    }

    ResetMailTransaction(state);
    state.clientName.clear();
    state.authenticated = false;
    state.authenticatedIdentity.clear();
    state.tlsHandshakePending = true;
    state.phase = SmtpSessionPhase::WaitingForGreeting;
    SendReply(context, state.connectionId, 220, "Ready to start TLS");
    context.socketsManager.StartTls(state.connectionId);
    return;
  }

  if (command == "AUTH")
  {
    if (state.authenticated)
    {
      SendReply(context, state.connectionId, 503, "Already authenticated");
      return;
    }
    if (!IsAuthAllowed(state, context))
    {
      SendReply(context, state.connectionId, 538, "Encryption required for requested authentication mechanism");
      return;
    }

    std::istringstream stream(argument);
    std::string mechanism;
    std::string initialResponse;
    stream >> mechanism >> initialResponse;
    mechanism = ToUpper(mechanism);

    if (mechanism == "PLAIN")
    {
      if (initialResponse.empty())
      {
        state.authStage = SmtpAuthStage::PlainResponse;
        SendReply(context, state.connectionId, 334, "");
        return;
      }

      std::string username;
      std::string secret;
      if (!ParsePlainAuth(initialResponse, username, secret))
      {
        SendReply(context, state.connectionId, 501, "Invalid authentication response");
        return;
      }

      MarkAuthenticated(state, context.authService.Authenticate(AuthRequest{"PLAIN", username, secret}));
      SendReply(context,
                state.connectionId,
                state.authenticated ? 235 : 535,
                state.authenticated ? "Authentication successful" : "Authentication failed");
      return;
    }

    if (mechanism == "LOGIN")
    {
      if (!initialResponse.empty())
      {
        const std::optional<std::string> username = DecodeBase64(initialResponse);
        if (!username || username->empty())
        {
          SendReply(context, state.connectionId, 501, "Invalid username");
          return;
        }

        state.pendingAuthUsername = *username;
        state.authStage = SmtpAuthStage::LoginPassword;
        SendReply(context, state.connectionId, 334, "UGFzc3dvcmQ6");
        return;
      }

      state.authStage = SmtpAuthStage::LoginUsername;
      SendReply(context, state.connectionId, 334, "VXNlcm5hbWU6");
      return;
    }

    SendReply(context, state.connectionId, 504, "Unsupported authentication mechanism");
    return;
  }

  if (command == "MAIL")
  {
    if (context.config.requireAuthentication && !state.authenticated)
    {
      SendReply(context, state.connectionId, 530, "Authentication required");
      return;
    }

    if (!StartsWithCaseInsensitive(argument, "FROM:"))
    {
      SendReply(context, state.connectionId, 501, "Expected MAIL FROM:<address>");
      return;
    }

    const std::string sender = ExtractPathArgument(argument);
    if (sender.empty())
    {
      SendReply(context, state.connectionId, 501, "Invalid sender");
      return;
    }

    ResetMailTransaction(state);
    state.sender = sender;
    state.phase = SmtpSessionPhase::MailTransaction;
    SendReply(context, state.connectionId, 250, "Sender OK");
    return;
  }

  if (command == "RCPT")
  {
    if (state.sender.empty())
    {
      SendReply(context, state.connectionId, 503, "Need MAIL FROM before RCPT TO");
      return;
    }
    if (!StartsWithCaseInsensitive(argument, "TO:"))
    {
      SendReply(context, state.connectionId, 501, "Expected RCPT TO:<address>");
      return;
    }

    const std::string recipient = ExtractPathArgument(argument);
    if (recipient.empty())
    {
      SendReply(context, state.connectionId, 501, "Invalid recipient");
      return;
    }

    state.recipients.push_back(recipient);
    state.phase = SmtpSessionPhase::MailTransaction;
    SendReply(context, state.connectionId, 250, "Recipient OK");
    return;
  }

  if (command == "DATA")
  {
    if (state.sender.empty() || state.recipients.empty())
    {
      SendReply(context, state.connectionId, 503, "Need MAIL FROM and RCPT TO before DATA");
      return;
    }

    state.phase = SmtpSessionPhase::ReceivingMessage;
    state.messageBuffer.clear();
    SendReply(context, state.connectionId, 354, "End data with <CR><LF>.<CR><LF>");
    return;
  }

  if (command == "RSET")
  {
    ResetMailTransaction(state);
    SendReply(context, state.connectionId, 250, "Reset OK");
    return;
  }

  if (command == "VRFY")
  {
    SendReply(context, state.connectionId, 252, "Cannot VRFY user, but will accept message");
    return;
  }

  if (command == "EXPN")
  {
    SendReply(context, state.connectionId, 502, "EXPN not implemented");
    return;
  }

  if (command == "HELP")
  {
    SendReply(context,
              state.connectionId,
              214,
              "Commands: HELO EHLO MAIL RCPT DATA RSET VRFY EXPN HELP NOOP QUIT STARTTLS AUTH");
    return;
  }

  if (command == "NOOP")
  {
    SendReply(context, state.connectionId, 250, "OK");
    return;
  }

  if (command == "QUIT")
  {
    state.phase = SmtpSessionPhase::Closing;
    SendReply(context, state.connectionId, 221, context.config.serverName + " closing connection");
    context.socketsManager.Close(state.connectionId);
    return;
  }

  SendReply(context, state.connectionId, 500, "Command unrecognized");
}

void SmtpSessionHandler::HandleDisconnected(const SmtpEvent&, SmtpSessionState& state, SmtpSessionContext&)
{
  ResetMailTransaction(state);
  state.phase = SmtpSessionPhase::Closed;
}

std::int64_t SmtpSessionHandler::StoreMessage(const SmtpSessionState& state, SmtpSessionContext& context)
{
  std::lock_guard<std::mutex> lock(context.storageMutex);
  context.database.Execute("BEGIN IMMEDIATE;");

  try
  {
    const std::int64_t messageId = context.mailMessages.CreateMessage(std::nullopt,
                                                                      state.sender,
                                                                      std::nullopt,
                                                                      state.messageBuffer,
                                                                      std::nullopt,
                                                                      true,
                                                                      Storage::MailMessageStatus::Queued);

    for (const std::string& recipient : state.recipients)
    {
      context.messageRecipients.CreateRecipient(
        messageId, recipient, Storage::RecipientType::To, Storage::DeliveryStatus::Queued);
    }

    context.database.Execute("COMMIT;");
    return messageId;
  }
  catch (...)
  {
    try
    {
      context.database.Execute("ROLLBACK;");
    }
    catch (...)
    {
    }
    throw;
  }
}

void SmtpSessionHandler::ResetMailTransaction(SmtpSessionState& state)
{
  state.sender.clear();
  state.recipients.clear();
  state.messageBuffer.clear();
  if (state.phase != SmtpSessionPhase::Closed && state.phase != SmtpSessionPhase::Closing)
  {
    state.phase = SmtpSessionPhase::ReadyForMail;
  }
}

void SmtpSessionHandler::SendReply(SmtpSessionContext& context,
                                   ConnectionId connectionId,
                                   int statusCode,
                                   const std::string& message)
{
  context.socketsManager.Send(connectionId, std::to_string(statusCode) + " " + message + "\r\n");
}

} // namespace smtp
