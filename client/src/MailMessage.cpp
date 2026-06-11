#include "mail_client/MailMessage.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cctype>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace mail_client {

namespace {

std::string Trim(std::string_view value)
{
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front()))) {
        value.remove_prefix(1);
    }
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) {
        value.remove_suffix(1);
    }
    return std::string(value);
}

bool IsAtomCharacter(char value)
{
    const unsigned char character = static_cast<unsigned char>(value);
    return std::isalnum(character) || value == '!' || value == '#' || value == '$' ||
           value == '%' || value == '&' || value == '\'' || value == '*' ||
           value == '+' || value == '-' || value == '/' || value == '=' ||
           value == '?' || value == '^' || value == '_' || value == '`' ||
           value == '{' || value == '|' || value == '}' || value == '~' ||
           value == '.';
}

bool IsDomainLabelValid(std::string_view label)
{
    if (label.empty() || label.front() == '-' || label.back() == '-') {
        return false;
    }

    return std::all_of(label.begin(), label.end(), [](char value) {
        const unsigned char character = static_cast<unsigned char>(value);
        return std::isalnum(character) || value == '-';
    });
}

std::string SanitizeHeaderValue(std::string_view value)
{
    std::string output;
    output.reserve(value.size());
    for (char character : value) {
        if (character == '\r' || character == '\n') {
            output.push_back(' ');
        } else {
            output.push_back(character);
        }
    }
    return Trim(output);
}

std::string QuoteDisplayName(std::string_view displayName)
{
    std::string output{"\""};
    for (char character : displayName) {
        if (character == '\\' || character == '"') {
            output.push_back('\\');
        }
        if (character != '\r' && character != '\n') {
            output.push_back(character);
        }
    }
    output.push_back('"');
    return output;
}

std::string FormatMailboxList(const std::vector<Mailbox>& mailboxes)
{
    std::ostringstream stream;
    for (std::size_t index = 0; index < mailboxes.size(); ++index) {
        if (index > 0) {
            stream << ", ";
        }
        stream << FormatMailbox(mailboxes[index]);
    }
    return stream.str();
}

std::tm Gmtime(std::time_t value)
{
    std::tm result{};
#if defined(_WIN32)
    gmtime_s(&result, &value);
#else
    gmtime_r(&value, &result);
#endif
    return result;
}

std::string FormatDateHeader()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    const std::tm utc = Gmtime(time);

    std::ostringstream stream;
    stream << std::put_time(&utc, "%a, %d %b %Y %H:%M:%S +0000");
    return stream.str();
}

std::string MakeMessageId(std::string_view domain)
{
    static std::atomic_uint64_t counter{1};
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now).count();

    std::ostringstream stream;
    stream << '<' << micros << '.' << counter.fetch_add(1) << "@";
    if (domain.empty()) {
        stream << "localhost";
    } else {
        stream << SanitizeHeaderValue(domain);
    }
    stream << '>';
    return stream.str();
}

void RequireMailbox(const Mailbox& mailbox, std::string_view role)
{
    if (!IsValidEmailAddress(mailbox.address)) {
        throw std::invalid_argument(std::string("Invalid ") + std::string(role) + " address");
    }
}

}

bool IsValidEmailAddress(std::string_view address)
{
    const std::string trimmed = Trim(address);
    if (trimmed.size() != address.size()) {
        return false;
    }

    const std::string_view value(trimmed);
    const std::size_t at = value.find('@');
    if (at == std::string_view::npos || at == 0 || at + 1 >= value.size()) {
        return false;
    }
    if (value.find('@', at + 1) != std::string_view::npos) {
        return false;
    }

    const std::string_view local = value.substr(0, at);
    const std::string_view domain = value.substr(at + 1);
    if (!std::all_of(local.begin(), local.end(), IsAtomCharacter)) {
        return false;
    }
    if (local.front() == '.' || local.back() == '.' || local.find("..") != std::string_view::npos) {
        return false;
    }

    std::size_t start = 0;
    while (start < domain.size()) {
        const std::size_t end = domain.find('.', start);
        const std::string_view label = domain.substr(start, end == std::string_view::npos
                                                               ? std::string_view::npos
                                                               : end - start);
        if (!IsDomainLabelValid(label)) {
            return false;
        }
        if (end == std::string_view::npos) {
            break;
        }
        start = end + 1;
    }

    return true;
}

std::string FormatMailbox(const Mailbox& mailbox)
{
    RequireMailbox(mailbox, "mailbox");
    if (Trim(mailbox.displayName).empty()) {
        return mailbox.address;
    }
    return QuoteDisplayName(SanitizeHeaderValue(mailbox.displayName)) + " <" + mailbox.address + ">";
}

std::vector<std::string> CollectEnvelopeRecipients(const EmailMessage& message)
{
    std::vector<std::string> recipients;
    const auto append = [&recipients](const std::vector<Mailbox>& mailboxes) {
        for (const Mailbox& mailbox : mailboxes) {
            RequireMailbox(mailbox, "recipient");
            recipients.push_back(mailbox.address);
        }
    };

    append(message.to);
    append(message.cc);
    append(message.bcc);
    return recipients;
}

std::string NormalizeToCrlf(std::string_view value)
{
    std::string output;
    output.reserve(value.size() + 8);

    for (std::size_t index = 0; index < value.size(); ++index) {
        const char character = value[index];
        if (character == '\r') {
            output += "\r\n";
            if (index + 1 < value.size() && value[index + 1] == '\n') {
                ++index;
            }
        } else if (character == '\n') {
            output += "\r\n";
        } else {
            output.push_back(character);
        }
    }

    return output;
}

std::string ComposeRfc5322Message(const EmailMessage& message, std::string_view messageIdDomain)
{
    RequireMailbox(message.from, "sender");
    const std::vector<std::string> recipients = CollectEnvelopeRecipients(message);
    if (recipients.empty()) {
        throw std::invalid_argument("At least one recipient is required");
    }

    std::ostringstream stream;
    stream << "Date: " << FormatDateHeader() << "\r\n";
    stream << "Message-ID: " << MakeMessageId(messageIdDomain) << "\r\n";
    stream << "From: " << FormatMailbox(message.from) << "\r\n";
    if (!message.to.empty()) {
        stream << "To: " << FormatMailboxList(message.to) << "\r\n";
    }
    if (!message.cc.empty()) {
        stream << "Cc: " << FormatMailboxList(message.cc) << "\r\n";
    }
    stream << "Subject: " << SanitizeHeaderValue(message.subject) << "\r\n";
    stream << "MIME-Version: 1.0\r\n";
    stream << "Content-Type: text/plain; charset=utf-8\r\n";
    stream << "Content-Transfer-Encoding: 8bit\r\n";

    for (const auto& [name, value] : message.extraHeaders) {
        if (!name.empty() && name.find(':') == std::string::npos) {
            stream << SanitizeHeaderValue(name) << ": " << SanitizeHeaderValue(value) << "\r\n";
        }
    }

    stream << "\r\n";
    stream << NormalizeToCrlf(message.body);
    if (!message.body.empty()) {
        const std::string normalized = NormalizeToCrlf(message.body);
        if (!normalized.ends_with("\r\n")) {
            stream << "\r\n";
        }
    }

    return stream.str();
}

}
