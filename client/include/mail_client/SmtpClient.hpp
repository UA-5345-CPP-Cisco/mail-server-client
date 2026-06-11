#pragma once

#include "mail_client/MailMessage.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/host_name_verification.hpp>

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace mail_client {

enum class StartTlsMode {
    Disabled,
    WhenAvailable,
    Required
};

enum class TlsVerificationMode {
    None,
    VerifyPeer
};

enum class AuthMechanism {
    Auto,
    Plain,
    Login
};

struct SmtpClientSettings {
    std::string host{"127.0.0.1"};
    std::uint16_t port{2525};
    std::string clientName{"mail-client.local"};
    StartTlsMode startTlsMode{StartTlsMode::WhenAvailable};
    TlsVerificationMode tlsVerificationMode{TlsVerificationMode::None};
    std::string certificateAuthorityFile;
};

struct SmtpCredentials {
    std::string username;
    std::string password;
};

struct SmtpReply {
    int code{0};
    std::vector<std::string> lines;

    bool IsPositiveCompletion() const;
    bool IsPositiveIntermediate() const;
    std::string Message() const;
};

class SmtpClientError : public std::runtime_error {
public:
    explicit SmtpClientError(std::string message);
    SmtpClientError(std::string message, SmtpReply reply);

    const SmtpReply* Reply() const noexcept;

private:
    std::optional<SmtpReply> reply_;
};

struct DeliveryReport {
    SmtpReply reply;
    std::string serverMessageId;
};

struct RetrievedMail {
    std::size_t index{0};
    std::string messageId;
    std::string rawContent;
};

class SmtpClient {
public:
    explicit SmtpClient(SmtpClientSettings settings = {});
    ~SmtpClient();

    SmtpClient(const SmtpClient&) = delete;
    SmtpClient& operator=(const SmtpClient&) = delete;

    void Connect();
    void Disconnect();
    bool IsConnected() const noexcept;
    bool IsTlsActive() const noexcept;

    const std::unordered_map<std::string, std::string>& Capabilities() const noexcept;

    SmtpReply Noop();
    SmtpReply Reset();
    void Authenticate(const SmtpCredentials& credentials,
                      AuthMechanism mechanism = AuthMechanism::Auto);
    DeliveryReport SendEmail(const EmailMessage& message);
    DeliveryReport SendRawMessage(std::string_view sender,
                                  const std::vector<std::string>& recipients,
                                  std::string_view rawMessage);
    std::size_t GetMailCount();
    std::vector<RetrievedMail> GetMails(std::size_t first, std::size_t last);

private:
    using Tcp = boost::asio::ip::tcp;

    void SayHello();
    void StartTls();
    void AuthenticatePlain(const SmtpCredentials& credentials);
    void AuthenticateLogin(const SmtpCredentials& credentials);

    SmtpReply SendCommand(std::string_view command);
    SmtpReply ReadReply();
    void WriteAll(std::string_view text);
    std::string ReadLine();
    void ExpectCode(const SmtpReply& reply,
                    std::initializer_list<int> expectedCodes,
                    std::string_view action) const;
    bool HasCapability(std::string_view capability) const;
    void ParseCapabilities(const SmtpReply& reply);
    void CloseSocket() noexcept;

    SmtpClientSettings settings_;
    boost::asio::io_context ioContext_;
    boost::asio::ssl::context sslContext_;
    Tcp::socket socket_;
    boost::asio::streambuf readBuffer_;
    std::unique_ptr<boost::asio::ssl::stream<Tcp::socket&>> tlsStream_;
    std::unordered_map<std::string, std::string> capabilities_;
    bool connected_{false};
    bool tlsActive_{false};
};

std::string Base64Encode(std::string_view input);
std::optional<std::string> Base64Decode(std::string_view input);
std::string BuildSmtpDataBlock(std::string_view rawMessage);

}
