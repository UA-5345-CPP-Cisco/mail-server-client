#include "mail_client/SmtpClient.hpp"

#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct CommandLine {
    std::string command;
    mail_client::SmtpClientSettings settings;
    std::optional<mail_client::SmtpCredentials> credentials;
    mail_client::EmailMessage message;
    std::size_t first{1};
    std::size_t last{10};
};

void PrintUsage()
{
    std::cout
        << "mail-client check --host 127.0.0.1 --port 2525\n"
        << "mail-client auth --host 127.0.0.1 --port 2525 --username alice@example.com --password secret\n"
        << "mail-client send --host 127.0.0.1 --port 2525 --from alice@example.com "
        << "--to bob@example.com --subject Hello --body \"Hi\"\n"
        << "mail-client count --host 127.0.0.1 --port 2525 --username bob@example.com --password secret\n"
        << "mail-client fetch --host 127.0.0.1 --port 2525 --username bob@example.com "
        << "--password secret --first 1 --last 10\n"
        << "\n"
        << "Options:\n"
        << "  --host VALUE        SMTP host, default 127.0.0.1\n"
        << "  --port VALUE        SMTP port, default 2525\n"
        << "  --client VALUE      EHLO client name, default mail-client.local\n"
        << "  --tls off|auto|required\n"
        << "  --verify-tls        Verify the TLS certificate when STARTTLS is used\n"
        << "  --ca-file VALUE     CA file for TLS verification\n"
        << "  --username VALUE    SMTP AUTH username\n"
        << "  --password VALUE    SMTP AUTH password\n"
        << "  --from VALUE        Sender address for send\n"
        << "  --from-name VALUE   Sender display name for send\n"
        << "  --to VALUE          Recipient address for send; repeat for many\n"
        << "  --cc VALUE          Cc address for send; repeat for many\n"
        << "  --bcc VALUE         Bcc address for send; repeat for many\n"
        << "  --subject VALUE     Message subject for send\n"
        << "  --body VALUE        Message body for send\n"
        << "  --first VALUE       First 1-based mailbox index for fetch, default 1\n"
        << "  --last VALUE        Last 1-based mailbox index for fetch, default 10\n";
}

std::string RequireValue(int& index, int argc, char* argv[], std::string_view option)
{
    if (index + 1 >= argc) {
        throw std::invalid_argument(std::string("Missing value for ") + std::string(option));
    }
    ++index;
    return argv[index];
}

std::uint16_t ParsePort(const std::string& value)
{
    const unsigned long port = std::stoul(value);
    if (port == 0 || port > 65535) {
        throw std::invalid_argument("Port must be between 1 and 65535");
    }
    return static_cast<std::uint16_t>(port);
}

std::size_t ParseIndex(const std::string& value, std::string_view option)
{
    const unsigned long parsed = std::stoul(value);
    if (parsed == 0) {
        throw std::invalid_argument(std::string(option) + " must be at least 1");
    }
    return static_cast<std::size_t>(parsed);
}

mail_client::StartTlsMode ParseTlsMode(const std::string& value)
{
    if (value == "off") {
        return mail_client::StartTlsMode::Disabled;
    }
    if (value == "auto") {
        return mail_client::StartTlsMode::WhenAvailable;
    }
    if (value == "required") {
        return mail_client::StartTlsMode::Required;
    }
    throw std::invalid_argument("TLS mode must be off, auto, or required");
}

CommandLine ParseCommandLine(int argc, char* argv[])
{
    if (argc < 2) {
        throw std::invalid_argument("Missing command");
    }

    CommandLine parsed;
    parsed.command = argv[1];

    for (int index = 2; index < argc; ++index) {
        const std::string option = argv[index];
        if (option == "--host") {
            parsed.settings.host = RequireValue(index, argc, argv, option);
        } else if (option == "--port") {
            parsed.settings.port = ParsePort(RequireValue(index, argc, argv, option));
        } else if (option == "--client") {
            parsed.settings.clientName = RequireValue(index, argc, argv, option);
        } else if (option == "--tls") {
            parsed.settings.startTlsMode = ParseTlsMode(RequireValue(index, argc, argv, option));
        } else if (option == "--verify-tls") {
            parsed.settings.tlsVerificationMode = mail_client::TlsVerificationMode::VerifyPeer;
        } else if (option == "--ca-file") {
            parsed.settings.certificateAuthorityFile = RequireValue(index, argc, argv, option);
        } else if (option == "--username") {
            if (!parsed.credentials) {
                parsed.credentials = mail_client::SmtpCredentials{};
            }
            parsed.credentials->username = RequireValue(index, argc, argv, option);
        } else if (option == "--password") {
            if (!parsed.credentials) {
                parsed.credentials = mail_client::SmtpCredentials{};
            }
            parsed.credentials->password = RequireValue(index, argc, argv, option);
        } else if (option == "--from") {
            parsed.message.from.address = RequireValue(index, argc, argv, option);
        } else if (option == "--from-name") {
            parsed.message.from.displayName = RequireValue(index, argc, argv, option);
        } else if (option == "--to") {
            parsed.message.to.push_back({RequireValue(index, argc, argv, option), {}});
        } else if (option == "--cc") {
            parsed.message.cc.push_back({RequireValue(index, argc, argv, option), {}});
        } else if (option == "--bcc") {
            parsed.message.bcc.push_back({RequireValue(index, argc, argv, option), {}});
        } else if (option == "--subject") {
            parsed.message.subject = RequireValue(index, argc, argv, option);
        } else if (option == "--body") {
            parsed.message.body = RequireValue(index, argc, argv, option);
        } else if (option == "--first") {
            parsed.first = ParseIndex(RequireValue(index, argc, argv, option), option);
        } else if (option == "--last") {
            parsed.last = ParseIndex(RequireValue(index, argc, argv, option), option);
        } else {
            throw std::invalid_argument("Unknown option: " + option);
        }
    }

    return parsed;
}

const mail_client::SmtpCredentials& RequireCredentials(const CommandLine& commandLine)
{
    if (!commandLine.credentials ||
        commandLine.credentials->username.empty() ||
        commandLine.credentials->password.empty()) {
        throw std::invalid_argument("--username and --password are required for this command");
    }
    return *commandLine.credentials;
}

void AuthenticateIfNeeded(mail_client::SmtpClient& client,
                          const std::optional<mail_client::SmtpCredentials>& credentials)
{
    if (credentials) {
        client.Authenticate(*credentials);
    }
}

int Run(const CommandLine& commandLine)
{
    mail_client::SmtpClient client(commandLine.settings);
    client.Connect();

    if (commandLine.command == "check") {
        AuthenticateIfNeeded(client, commandLine.credentials);
        const mail_client::SmtpReply reply = client.Noop();
        std::cout << "Server responded: " << reply.code << ' ' << reply.Message() << '\n';
        client.Disconnect();
        return 0;
    }

    if (commandLine.command == "auth") {
        const mail_client::SmtpCredentials& credentials = RequireCredentials(commandLine);
        client.Authenticate(credentials);
        std::cout << "Account ready: " << credentials.username << '\n';
        client.Disconnect();
        return 0;
    }

    if (commandLine.command == "send") {
        mail_client::EmailMessage message = commandLine.message;
        if (message.from.address.empty() && commandLine.credentials) {
            message.from.address = commandLine.credentials->username;
        }

        AuthenticateIfNeeded(client, commandLine.credentials);
        const mail_client::DeliveryReport report = client.SendEmail(message);
        std::cout << "Message accepted";
        if (!report.serverMessageId.empty()) {
            std::cout << " as " << report.serverMessageId;
        }
        std::cout << '\n';
        client.Disconnect();
        return 0;
    }

    if (commandLine.command == "count") {
        const mail_client::SmtpCredentials& credentials = RequireCredentials(commandLine);
        client.Authenticate(credentials);
        std::cout << "Mail count: " << client.GetMailCount() << '\n';
        client.Disconnect();
        return 0;
    }

    if (commandLine.command == "fetch") {
        const mail_client::SmtpCredentials& credentials = RequireCredentials(commandLine);
        if (commandLine.last < commandLine.first) {
            throw std::invalid_argument("--last must be greater than or equal to --first");
        }

        client.Authenticate(credentials);
        const std::vector<mail_client::RetrievedMail> messages =
            client.GetMails(commandLine.first, commandLine.last);

        if (messages.empty()) {
            std::cout << "No messages\n";
        }

        for (const mail_client::RetrievedMail& message : messages) {
            std::cout << "--- Mail " << message.index << " (id " << message.messageId << ") ---\n";
            std::cout << message.rawContent;
            if (!message.rawContent.ends_with('\n')) {
                std::cout << '\n';
            }
        }

        client.Disconnect();
        return 0;
    }

    throw std::invalid_argument("Command must be check, auth, send, count, or fetch");
}

}

int main(int argc, char* argv[])
{
    try {
        if (argc == 2 && std::string(argv[1]) == "--help") {
            PrintUsage();
            return 0;
        }

        const CommandLine commandLine = ParseCommandLine(argc, argv);
        return Run(commandLine);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n\n";
        PrintUsage();
        return 1;
    }
}
