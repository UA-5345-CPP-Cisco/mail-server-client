#include "mail_client/SmtpClient.hpp"

#include <openssl/ssl.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <iomanip>
#include <initializer_list>
#include <optional>
#include <sstream>
#include <utility>

namespace mail_client {

namespace {

std::string ToUpper(std::string_view value)
{
    std::string output(value);
    std::transform(output.begin(), output.end(), output.begin(), [](unsigned char character) {
        return static_cast<char>(std::toupper(character));
    });
    return output;
}

bool IsReplyLine(std::string_view line)
{
    return line.size() >= 3 &&
           std::isdigit(static_cast<unsigned char>(line[0])) &&
           std::isdigit(static_cast<unsigned char>(line[1])) &&
           std::isdigit(static_cast<unsigned char>(line[2]));
}

int ParseReplyCode(std::string_view line)
{
    if (!IsReplyLine(line)) {
        throw SmtpClientError("Received malformed SMTP reply: " + std::string(line));
    }
    return ((line[0] - '0') * 100) + ((line[1] - '0') * 10) + (line[2] - '0');
}

std::string ReplyText(std::string_view line)
{
    if (line.size() <= 4) {
        return {};
    }
    return std::string(line.substr(4));
}

std::string BuildMailboxPath(std::string_view address)
{
    return "<" + std::string(address) + ">";
}

std::string ExtractAcceptedMessageId(const SmtpReply& reply)
{
    const std::string message = reply.Message();
    const std::string marker = " as ";
    const std::size_t start = message.find(marker);
    if (start == std::string::npos) {
        return {};
    }
    return message.substr(start + marker.size());
}

RetrievedMail ParseRetrievedMailLine(const std::string& line)
{
    std::istringstream stream(line);
    std::string tag;
    RetrievedMail result;
    std::string encodedRawContent;

    stream >> tag >> result.index >> result.messageId >> encodedRawContent;
    if (tag != "MAIL" || result.index == 0 || result.messageId.empty()) {
        throw SmtpClientError("Received malformed mailbox response: " + line);
    }

    const std::optional<std::string> decoded = Base64Decode(encodedRawContent);
    if (!decoded) {
        throw SmtpClientError("Received mailbox response with invalid message encoding");
    }

    result.rawContent = *decoded;
    return result;
}

}

bool SmtpReply::IsPositiveCompletion() const
{
    return code >= 200 && code < 300;
}

bool SmtpReply::IsPositiveIntermediate() const
{
    return code >= 300 && code < 400;
}

std::string SmtpReply::Message() const
{
    std::ostringstream stream;
    for (std::size_t index = 0; index < lines.size(); ++index) {
        if (index > 0) {
            stream << '\n';
        }
        stream << lines[index];
    }
    return stream.str();
}

SmtpClientError::SmtpClientError(std::string message)
    : std::runtime_error(std::move(message))
{
}

SmtpClientError::SmtpClientError(std::string message, SmtpReply reply)
    : std::runtime_error(std::move(message) + ": " + reply.Message()),
      reply_(std::move(reply))
{
}

const SmtpReply* SmtpClientError::Reply() const noexcept
{
    return reply_ ? &*reply_ : nullptr;
}

SmtpClient::SmtpClient(SmtpClientSettings settings)
    : settings_(std::move(settings)),
      sslContext_(boost::asio::ssl::context::tls_client),
      socket_(ioContext_)
{
}

SmtpClient::~SmtpClient()
{
    CloseSocket();
}

void SmtpClient::Connect()
{
    if (connected_) {
        return;
    }

    boost::system::error_code error;
    Tcp::resolver resolver(ioContext_);
    const auto endpoints = resolver.resolve(settings_.host, std::to_string(settings_.port), error);
    if (error) {
        throw SmtpClientError("Could not resolve SMTP server " + settings_.host + ": " + error.message());
    }

    boost::asio::connect(socket_, endpoints, error);
    if (error) {
        throw SmtpClientError("Could not connect to SMTP server " + settings_.host + ": " + error.message());
    }

    connected_ = true;
    const SmtpReply greeting = ReadReply();
    ExpectCode(greeting, {220}, "connect");

    SayHello();

    if (settings_.startTlsMode != StartTlsMode::Disabled) {
        const bool available = HasCapability("STARTTLS");
        if (!available && settings_.startTlsMode == StartTlsMode::Required) {
            throw SmtpClientError("SMTP server does not advertise STARTTLS");
        }
        if (available) {
            StartTls();
            SayHello();
        }
    }
}

void SmtpClient::Disconnect()
{
    if (!connected_) {
        return;
    }

    try {
        SendCommand("QUIT");
    } catch (const std::exception&) {
    }
    CloseSocket();
}

bool SmtpClient::IsConnected() const noexcept
{
    return connected_;
}

bool SmtpClient::IsTlsActive() const noexcept
{
    return tlsActive_;
}

const std::unordered_map<std::string, std::string>& SmtpClient::Capabilities() const noexcept
{
    return capabilities_;
}

SmtpReply SmtpClient::Noop()
{
    const SmtpReply reply = SendCommand("NOOP");
    ExpectCode(reply, {250}, "NOOP");
    return reply;
}

SmtpReply SmtpClient::Reset()
{
    const SmtpReply reply = SendCommand("RSET");
    ExpectCode(reply, {250}, "RSET");
    return reply;
}

void SmtpClient::Authenticate(const SmtpCredentials& credentials, AuthMechanism mechanism)
{
    if (credentials.username.empty()) {
        throw SmtpClientError("SMTP username is required");
    }

    if (mechanism == AuthMechanism::Auto) {
        const auto auth = capabilities_.find("AUTH");
        const std::string advertised = auth == capabilities_.end() ? std::string{} : auth->second;
        if (advertised.find("PLAIN") != std::string::npos) {
            mechanism = AuthMechanism::Plain;
        } else if (advertised.find("LOGIN") != std::string::npos) {
            mechanism = AuthMechanism::Login;
        } else {
            throw SmtpClientError("SMTP server does not advertise a supported AUTH mechanism");
        }
    }

    if (mechanism == AuthMechanism::Plain) {
        AuthenticatePlain(credentials);
        return;
    }

    AuthenticateLogin(credentials);
}

DeliveryReport SmtpClient::SendEmail(const EmailMessage& message)
{
    const std::string rawMessage = ComposeRfc5322Message(message, settings_.clientName);
    return SendRawMessage(message.from.address, CollectEnvelopeRecipients(message), rawMessage);
}

DeliveryReport SmtpClient::SendRawMessage(std::string_view sender,
                                          const std::vector<std::string>& recipients,
                                          std::string_view rawMessage)
{
    if (!IsValidEmailAddress(sender)) {
        throw SmtpClientError("Invalid sender address");
    }
    if (recipients.empty()) {
        throw SmtpClientError("At least one recipient is required");
    }

    const SmtpReply mailReply = SendCommand("MAIL FROM:" + BuildMailboxPath(sender));
    ExpectCode(mailReply, {250}, "MAIL FROM");

    try {
        for (const std::string& recipient : recipients) {
            if (!IsValidEmailAddress(recipient)) {
                throw SmtpClientError("Invalid recipient address: " + recipient);
            }
            const SmtpReply recipientReply = SendCommand("RCPT TO:" + BuildMailboxPath(recipient));
            ExpectCode(recipientReply, {250, 251, 252}, "RCPT TO");
        }

        const SmtpReply dataReply = SendCommand("DATA");
        ExpectCode(dataReply, {354}, "DATA");

        WriteAll(BuildSmtpDataBlock(rawMessage));
        const SmtpReply acceptedReply = ReadReply();
        ExpectCode(acceptedReply, {250}, "message body");
        return DeliveryReport{acceptedReply, ExtractAcceptedMessageId(acceptedReply)};
    } catch (const std::exception&) {
        try {
            Reset();
        } catch (const std::exception&) {
        }
        throw;
    }
}

std::size_t SmtpClient::GetMailCount()
{
    const SmtpReply reply = SendCommand("MAILCOUNT");
    ExpectCode(reply, {250}, "MAILCOUNT");

    std::size_t count = 0;
    std::istringstream stream(reply.Message());
    stream >> count;
    if (stream.fail()) {
        throw SmtpClientError("Received malformed mailbox count response", reply);
    }

    return count;
}

std::vector<RetrievedMail> SmtpClient::GetMails(std::size_t first, std::size_t last)
{
    if (first == 0 || last < first) {
        throw SmtpClientError("Mail range must be 1-based and inclusive");
    }

    const SmtpReply reply = SendCommand("GETMAILS " + std::to_string(first) + " " + std::to_string(last));
    ExpectCode(reply, {250}, "GETMAILS");

    std::vector<RetrievedMail> result;
    for (const std::string& line : reply.lines) {
        if (line == "OK" || line.empty()) {
            continue;
        }
        result.push_back(ParseRetrievedMailLine(line));
    }

    return result;
}

void SmtpClient::SayHello()
{
    SmtpReply reply = SendCommand("EHLO " + settings_.clientName);
    if (reply.code == 250) {
        ParseCapabilities(reply);
        return;
    }

    reply = SendCommand("HELO " + settings_.clientName);
    ExpectCode(reply, {250}, "HELO");
    capabilities_.clear();
}

void SmtpClient::StartTls()
{
    const SmtpReply reply = SendCommand("STARTTLS");
    ExpectCode(reply, {220}, "STARTTLS");

    if (!settings_.certificateAuthorityFile.empty()) {
        sslContext_.load_verify_file(settings_.certificateAuthorityFile);
    } else {
        boost::system::error_code ignored;
        sslContext_.set_default_verify_paths(ignored);
    }

    tlsStream_ = std::make_unique<boost::asio::ssl::stream<Tcp::socket&>>(socket_, sslContext_);

    if (!settings_.host.empty()) {
        SSL_set_tlsext_host_name(tlsStream_->native_handle(), settings_.host.c_str());
    }

    if (settings_.tlsVerificationMode == TlsVerificationMode::VerifyPeer) {
        tlsStream_->set_verify_mode(boost::asio::ssl::verify_peer);
        tlsStream_->set_verify_callback(boost::asio::ssl::host_name_verification(settings_.host));
    } else {
        tlsStream_->set_verify_mode(boost::asio::ssl::verify_none);
    }

    boost::system::error_code error;
    tlsStream_->handshake(boost::asio::ssl::stream_base::client, error);
    if (error) {
        throw SmtpClientError("STARTTLS handshake failed: " + error.message());
    }

    tlsActive_ = true;
    capabilities_.clear();
}

void SmtpClient::AuthenticatePlain(const SmtpCredentials& credentials)
{
    const std::string payload = std::string("\0", 1) + credentials.username +
                                std::string("\0", 1) + credentials.password;
    const SmtpReply reply = SendCommand("AUTH PLAIN " + Base64Encode(payload));
    ExpectCode(reply, {235}, "AUTH PLAIN");
}

void SmtpClient::AuthenticateLogin(const SmtpCredentials& credentials)
{
    SmtpReply reply = SendCommand("AUTH LOGIN");
    ExpectCode(reply, {334}, "AUTH LOGIN username prompt");

    reply = SendCommand(Base64Encode(credentials.username));
    ExpectCode(reply, {334}, "AUTH LOGIN password prompt");

    reply = SendCommand(Base64Encode(credentials.password));
    ExpectCode(reply, {235}, "AUTH LOGIN");
}

SmtpReply SmtpClient::SendCommand(std::string_view command)
{
    WriteAll(std::string(command) + "\r\n");
    return ReadReply();
}

SmtpReply SmtpClient::ReadReply()
{
    SmtpReply reply;

    while (true) {
        const std::string line = ReadLine();
        const int code = ParseReplyCode(line);
        if (reply.code == 0) {
            reply.code = code;
        } else if (reply.code != code) {
            throw SmtpClientError("SMTP server changed reply code inside a multiline response");
        }

        reply.lines.push_back(ReplyText(line));
        const bool more = line.size() > 3 && line[3] == '-';
        if (!more) {
            break;
        }
    }

    return reply;
}

void SmtpClient::WriteAll(std::string_view text)
{
    if (!connected_) {
        throw SmtpClientError("SMTP client is not connected");
    }

    boost::system::error_code error;
    if (tlsActive_ && tlsStream_) {
        boost::asio::write(*tlsStream_, boost::asio::buffer(text.data(), text.size()), error);
    } else {
        boost::asio::write(socket_, boost::asio::buffer(text.data(), text.size()), error);
    }

    if (error) {
        throw SmtpClientError("Could not write to SMTP server: " + error.message());
    }
}

std::string SmtpClient::ReadLine()
{
    if (!connected_) {
        throw SmtpClientError("SMTP client is not connected");
    }

    boost::system::error_code error;
    if (tlsActive_ && tlsStream_) {
        boost::asio::read_until(*tlsStream_, readBuffer_, "\n", error);
    } else {
        boost::asio::read_until(socket_, readBuffer_, "\n", error);
    }

    if (error) {
        throw SmtpClientError("Could not read from SMTP server: " + error.message());
    }

    std::istream stream(&readBuffer_);
    std::string line;
    std::getline(stream, line);
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

void SmtpClient::ExpectCode(const SmtpReply& reply,
                            std::initializer_list<int> expectedCodes,
                            std::string_view action) const
{
    if (std::find(expectedCodes.begin(), expectedCodes.end(), reply.code) == expectedCodes.end()) {
        throw SmtpClientError("SMTP " + std::string(action) + " failed", reply);
    }
}

bool SmtpClient::HasCapability(std::string_view capability) const
{
    return capabilities_.contains(ToUpper(capability));
}

void SmtpClient::ParseCapabilities(const SmtpReply& reply)
{
    capabilities_.clear();
    if (reply.lines.empty()) {
        return;
    }

    for (std::size_t index = 1; index < reply.lines.size(); ++index) {
        const std::string& line = reply.lines[index];
        std::istringstream stream(line);
        std::string key;
        stream >> key;
        if (key.empty()) {
            continue;
        }

        std::string rest;
        std::getline(stream, rest);
        if (!rest.empty() && rest.front() == ' ') {
            rest.erase(rest.begin());
        }
        capabilities_[ToUpper(key)] = ToUpper(rest);
    }
}

void SmtpClient::CloseSocket() noexcept
{
    boost::system::error_code ignored;
    if (tlsActive_ && tlsStream_) {
        tlsStream_->shutdown(ignored);
        tlsStream_.reset();
    }
    socket_.shutdown(Tcp::socket::shutdown_both, ignored);
    socket_.close(ignored);
    connected_ = false;
    tlsActive_ = false;
    capabilities_.clear();
    readBuffer_.consume(readBuffer_.size());
}

std::string Base64Encode(std::string_view input)
{
    static constexpr std::array<char, 64> alphabet{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
    };

    std::string output;
    output.reserve(((input.size() + 2) / 3) * 4);

    for (std::size_t index = 0; index < input.size(); index += 3) {
        const std::uint32_t byte0 = static_cast<unsigned char>(input[index]);
        const std::uint32_t byte1 = index + 1 < input.size()
                                        ? static_cast<unsigned char>(input[index + 1])
                                        : 0;
        const std::uint32_t byte2 = index + 2 < input.size()
                                        ? static_cast<unsigned char>(input[index + 2])
                                        : 0;
        const std::uint32_t triple = (byte0 << 16) | (byte1 << 8) | byte2;

        output.push_back(alphabet[(triple >> 18) & 0x3F]);
        output.push_back(alphabet[(triple >> 12) & 0x3F]);
        output.push_back(index + 1 < input.size() ? alphabet[(triple >> 6) & 0x3F] : '=');
        output.push_back(index + 2 < input.size() ? alphabet[triple & 0x3F] : '=');
    }

    return output;
}

std::optional<std::string> Base64Decode(std::string_view input)
{
    static constexpr std::array<int, 256> table = [] {
        std::array<int, 256> values{};
        values.fill(-1);
        for (int character = 'A'; character <= 'Z'; ++character) {
            values[static_cast<std::size_t>(character)] = character - 'A';
        }
        for (int character = 'a'; character <= 'z'; ++character) {
            values[static_cast<std::size_t>(character)] = character - 'a' + 26;
        }
        for (int character = '0'; character <= '9'; ++character) {
            values[static_cast<std::size_t>(character)] = character - '0' + 52;
        }
        values[static_cast<std::size_t>('+')] = 62;
        values[static_cast<std::size_t>('/')] = 63;
        return values;
    }();

    std::string output;
    int buffer = 0;
    int bits = -8;

    for (unsigned char character : input) {
        if (std::isspace(character)) {
            continue;
        }
        if (character == '=') {
            break;
        }

        const int value = table[character];
        if (value < 0) {
            return std::nullopt;
        }

        buffer = (buffer << 6) | value;
        bits += 6;

        if (bits >= 0) {
            output.push_back(static_cast<char>((buffer >> bits) & 0xFF));
            bits -= 8;
        }
    }

    return output;
}

std::string BuildSmtpDataBlock(std::string_view rawMessage)
{
    const std::string normalized = NormalizeToCrlf(rawMessage);
    if (normalized.empty()) {
        return ".\r\n";
    }

    std::string output;
    output.reserve(normalized.size() + 8);

    std::size_t lineStart = 0;
    while (lineStart <= normalized.size()) {
        const std::size_t lineEnd = normalized.find("\r\n", lineStart);
        const std::string_view line = lineEnd == std::string::npos
                                          ? std::string_view(normalized).substr(lineStart)
                                          : std::string_view(normalized).substr(lineStart, lineEnd - lineStart);

        if (!line.empty() && line.front() == '.') {
            output.push_back('.');
        }
        output.append(line);
        output += "\r\n";

        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 2;
        if (lineStart == normalized.size()) {
            break;
        }
    }

    output += ".\r\n";
    return output;
}

}
