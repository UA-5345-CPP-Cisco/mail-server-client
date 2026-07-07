#include <string>

#include <gtest/gtest.h>

#include "smtp_server_sdk/SmtpResult.h"

TEST(SmtpResultTest, SuccessIsOk)
{
  const Smtp::SmtpResult result = Smtp::SmtpResult::Success();

  EXPECT_TRUE(result.ok());
  EXPECT_EQ(result.error, Smtp::SmtpError::None);
  EXPECT_EQ(result.smtpCode, 250);
}

TEST(SmtpResultTest, FailIsNotOk)
{
  const Smtp::SmtpResult result =
    Smtp::SmtpResult::Fail(Smtp::SmtpError::ConnectionFailed, "Connection refused", 0);

  EXPECT_FALSE(result.ok());
  EXPECT_EQ(result.error, Smtp::SmtpError::ConnectionFailed);
  EXPECT_EQ(result.message, "Connection refused");
}

TEST(SmtpResultTest, FailPreservesSmtpCode)
{
  const Smtp::SmtpResult result =
    Smtp::SmtpResult::Fail(Smtp::SmtpError::RecipientRejected, "No such user", 550);

  EXPECT_FALSE(result.ok());
  EXPECT_EQ(result.error, Smtp::SmtpError::RecipientRejected);
  EXPECT_EQ(result.smtpCode, 550);
  EXPECT_EQ(result.message, "No such user");
}

TEST(SmtpResultTest, FailDefaultsCodeToZero)
{
  const Smtp::SmtpResult result =
    Smtp::SmtpResult::Fail(Smtp::SmtpError::Timeout, "Timed out");

  EXPECT_EQ(result.smtpCode, 0);
}

TEST(SmtpResultTest, DifferentErrorTypesAreDistinct)
{
  const auto dns = Smtp::SmtpResult::Fail(Smtp::SmtpError::DnsResolutionFailed, "");
  const auto auth = Smtp::SmtpResult::Fail(Smtp::SmtpError::AuthenticationFailed, "");
  const auto timeout = Smtp::SmtpResult::Fail(Smtp::SmtpError::Timeout, "");
  const auto server = Smtp::SmtpResult::Fail(Smtp::SmtpError::ServerError, "");
  const auto unknown = Smtp::SmtpResult::Fail(Smtp::SmtpError::Unknown, "");

  EXPECT_NE(dns.error, auth.error);
  EXPECT_NE(auth.error, timeout.error);
  EXPECT_NE(timeout.error, server.error);
  EXPECT_NE(server.error, unknown.error);
}