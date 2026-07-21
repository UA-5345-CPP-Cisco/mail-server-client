#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "smtp_server_sdk/MailBuilder.h"

TEST(MailBuilderTest, BuildsMail)
{
  const Smtp::Mail mail = Smtp::MailBuilder()
                            .SetSender("sender@example.com")
                            .AddRecipients({"first@example.com", "second@example.com"})
                            .SetSubject("Subject")
                            .SetBody("Body")
                            .Build();

  EXPECT_EQ(mail.sender, "sender@example.com");
  EXPECT_EQ(mail.recipients.size(), 2);
  EXPECT_EQ(mail.subject, "Subject");
  EXPECT_EQ(mail.body, "Body");
}

TEST(MailBuilderTest, RequiresSender)
{
  EXPECT_THROW(Smtp::MailBuilder().AddRecipients({"recipient@example.com"}).Build(), std::runtime_error);
}

TEST(MailBuilderTest, RequiresRecipient)
{
  EXPECT_THROW(Smtp::MailBuilder().SetSender("sender@example.com").Build(), std::runtime_error);
}
