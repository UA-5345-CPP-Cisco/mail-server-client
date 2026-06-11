#include "mail_client/MailMessage.hpp"
#include "mail_client/SmtpClient.hpp"

#include <cassert>
#include <string>
#include <vector>

using mail_client::Base64Encode;
using mail_client::Base64Decode;
using mail_client::BuildSmtpDataBlock;
using mail_client::CollectEnvelopeRecipients;
using mail_client::ComposeRfc5322Message;
using mail_client::EmailMessage;
using mail_client::IsValidEmailAddress;
using mail_client::Mailbox;

namespace {

void TestAddressValidation()
{
    assert(IsValidEmailAddress("alice@example.com"));
    assert(IsValidEmailAddress("first.last+tag@localhost"));
    assert(!IsValidEmailAddress(" alice@example.com "));
    assert(!IsValidEmailAddress("missing-at"));
    assert(!IsValidEmailAddress("@example.com"));
    assert(!IsValidEmailAddress("alice@"));
    assert(!IsValidEmailAddress("alice..test@example.com"));
    assert(!IsValidEmailAddress("alice@example..com"));
}

void TestEnvelopeRecipients()
{
    EmailMessage message;
    message.from = {"alice@example.com", "Alice"};
    message.to = {Mailbox{"bob@example.com", "Bob"}};
    message.cc = {Mailbox{"carol@example.com", "Carol"}};
    message.bcc = {Mailbox{"dan@example.com", "Dan"}};

    const std::vector<std::string> recipients = CollectEnvelopeRecipients(message);
    assert((recipients == std::vector<std::string>{
                              "bob@example.com",
                              "carol@example.com",
                              "dan@example.com"}));
}

void TestMessageComposition()
{
    EmailMessage message;
    message.from = {"alice@example.com", "Alice Example"};
    message.to = {Mailbox{"bob@example.com", "Bob"}};
    message.bcc = {Mailbox{"secret@example.com", "Secret"}};
    message.subject = "Hello\r\nInjected";
    message.body = "Line one\n.Line two";

    const std::string raw = ComposeRfc5322Message(message, "client.test");
    assert(raw.find("From: \"Alice Example\" <alice@example.com>\r\n") != std::string::npos);
    assert(raw.find("To: \"Bob\" <bob@example.com>\r\n") != std::string::npos);
    assert(raw.find("Bcc:") == std::string::npos);
    assert(raw.find("Subject: Hello  Injected\r\n") != std::string::npos);
    assert(raw.find("\r\n\r\nLine one\r\n.Line two\r\n") != std::string::npos);
}

void TestSmtpHelpers()
{
    assert(Base64Encode("") == "");
    assert(Base64Encode("alice") == "YWxpY2U=");
    assert(Base64Encode(std::string("\0alice\0secret", 13)) == "AGFsaWNlAHNlY3JldA==");
    assert(Base64Decode("YWxpY2U=").value() == "alice");
    assert(!Base64Decode("not valid!").has_value());

    const std::string data = BuildSmtpDataBlock("Subject: Test\n\n.Line\nLast");
    assert(data == "Subject: Test\r\n\r\n..Line\r\nLast\r\n.\r\n");
    assert(BuildSmtpDataBlock("") == ".\r\n");
}

}

int main()
{
    TestAddressValidation();
    TestEnvelopeRecipients();
    TestMessageComposition();
    TestSmtpHelpers();
    return 0;
}
