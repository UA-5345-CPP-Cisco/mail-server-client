#include <iostream>
#include <string>
#include <cassert>

#include "MailBuilder.hpp"
#include "SmtpClient.hpp"

int tests_passed = 0;
int tests_failed = 0;

void ReportTest(const std::string& name, bool passed)
{
    if (passed)
    {
        std::cout << "[PASSED] " << name << std::endl;
        tests_passed++;
    }
    else
    {
        std::cout << "[FAILED] " << name << std::endl;
        tests_failed++;
    }
}

// ========== MAILBUILDER TESTS ==========

void TestBuildBasicMail()
{
    ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
            .SetSender("vlad@gmail.com")
            .AddRecipient({"bob@gmail.com"})
            .SetSubject("Hello")
            .SetBody("Privet")
            .Build();

    ReportTest("Sender is set", mail.sender == "vlad@gmail.com");
    ReportTest("Subject is set", mail.subject == "Hello");
    ReportTest("Body is set", mail.body == "Privet");
    ReportTest("Recipients count is 1", mail.recipients.size() == 1);
    ReportTest("Recipient is correct", mail.recipients[0] == "bob@gmail.com");
}

void TestBuildMultipleRecipients()
{
    ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
            .SetSender("vlad@gmail.com")
            .AddRecipient({"bob@gmail.com", "alice@gmail.com", "charlie@gmail.com"})
            .SetSubject("Group mail")
            .SetBody("Hello everyone")
            .Build();

    ReportTest("Multiple recipients count", mail.recipients.size() == 3);
    ReportTest("First recipient", mail.recipients[0] == "bob@gmail.com");
    ReportTest("Second recipient", mail.recipients[1] == "alice@gmail.com");
    ReportTest("Third recipient", mail.recipients[2] == "charlie@gmail.com");
}

void TestBuildWithoutSenderThrows()
{
    bool exception_thrown = false;

    try
    {
        ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
                .AddRecipient({"bob@gmail.com"})
                .SetSubject("No sender")
                .Build();
    }
    catch (const std::runtime_error&)
    {
        exception_thrown = true;
    }

    ReportTest("Build without sender throws exception", exception_thrown);
}

void TestBuildWithoutRecipientsThrows()
{
    bool exception_thrown = false;

    try
    {
        ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
                .SetSender("vlad@gmail.com")
                .SetSubject("No recipients")
                .Build();
    }
    catch (const std::runtime_error&)
    {
        exception_thrown = true;
    }

    ReportTest("Build without recipients throws exception", exception_thrown);
}

void TestBuildWithoutSender()
{
    ISXSmtpClient::MailBuilder builder;
    builder.AddRecipient({"bob@gmail.com"});
    builder.SetSubject("Test");
    builder.SetBody("Body");

    bool exception_thrown = false;
    try
    {
        builder.Build();
    }
    catch (const std::runtime_error&)
    {
        exception_thrown = true;
    }

    ReportTest("Non-chained build without sender throws", exception_thrown);
}

void TestChainOrder()
{
    ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
            .SetBody("Body first")
            .SetSubject("Subject second")
            .AddRecipient({"bob@gmail.com"})
            .SetSender("vlad@gmail.com")
            .Build();

    ReportTest("Chain order does not matter - sender", mail.sender == "vlad@gmail.com");
    ReportTest("Chain order does not matter - body", mail.body == "Body first");
    ReportTest("Chain order does not matter - subject", mail.subject == "Subject second");
}

void TestEmptySubjectAndBody()
{
    ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
            .SetSender("vlad@gmail.com")
            .AddRecipient({"bob@gmail.com"})
            .Build();

    ReportTest("Empty subject is allowed", mail.subject.empty());
    ReportTest("Empty body is allowed", mail.body.empty());
}

// ========== SMTPCLIENT TESTS ==========

void TestCheckConnectionBadHost()
{
    ISXSmtpClient::SmtpClient client("192.0.2.1", 25);
    bool result = client.CheckConnection();

    ReportTest("CheckConnection to bad host returns false", result == false);
}

void TestSendMailBadHost()
{
    ISXSmtpClient::SmtpClient client("192.0.2.1", 25);

    ISXSmtpClient::Mail mail = ISXSmtpClient::MailBuilder()
            .SetSender("vlad@gmail.com")
            .AddRecipient({"bob@gmail.com"})
            .SetSubject("Test")
            .SetBody("Hello")
            .Build();

    bool result = client.SendMailMessage(mail);

    ReportTest("SendMailMessage to bad host returns false", result == false);
}

void TestSetServerAddress()
{
    ISXSmtpClient::SmtpClient client("old-address", 25);
    client.set_server_address("192.0.2.1");
    client.set_port(587);

    bool result = client.CheckConnection();

    ReportTest("After set_server_address still works (returns false for bad host)", result == false);
}

// ========== MAIN ==========

int main()
{
    std::cout << "========== MAILBUILDER TESTS ==========" << std::endl;
    TestBuildBasicMail();
    TestBuildMultipleRecipients();
    TestBuildWithoutSenderThrows();
    TestBuildWithoutRecipientsThrows();
    TestBuildWithoutSender();
    TestChainOrder();
    TestEmptySubjectAndBody();

    std::cout << std::endl;
    std::cout << "========== SMTPCLIENT TESTS ==========" << std::endl;
    TestCheckConnectionBadHost();
    TestSendMailBadHost();
    TestSetServerAddress();

    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "TOTAL: " << tests_passed << " passed, " << tests_failed << " failed" << std::endl;
    std::cout << "==========================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}