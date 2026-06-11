#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace mail_client {

struct Mailbox {
    std::string address;
    std::string displayName;
};

struct EmailMessage {
    Mailbox from;
    std::vector<Mailbox> to;
    std::vector<Mailbox> cc;
    std::vector<Mailbox> bcc;
    std::string subject;
    std::string body;
    std::map<std::string, std::string> extraHeaders;
};

bool IsValidEmailAddress(std::string_view address);
std::string FormatMailbox(const Mailbox& mailbox);
std::vector<std::string> CollectEnvelopeRecipients(const EmailMessage& message);
std::string NormalizeToCrlf(std::string_view value);
std::string ComposeRfc5322Message(const EmailMessage& message,
                                  std::string_view messageIdDomain = "localhost");

}
