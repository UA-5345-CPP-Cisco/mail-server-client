#pragma once

#include <string>

namespace Smtp {

enum class SmtpError {
  None,
  ConnectionFailed,
  DnsResolutionFailed,
  AuthenticationFailed,
  RecipientRejected,
  Timeout,
  ServerError,
  Unknown
};

struct SmtpReply {
  int code = 0;
  std::string text;
};

struct SmtpResult {
  SmtpError error = SmtpError::None;
  std::string message;
  int smtpCode = 0;

  bool ok() const { return error == SmtpError::None; }

  static SmtpResult Success()
  {
    return {SmtpError::None, "Message sent successfully", 250};
  }

  static SmtpResult Fail(SmtpError err, const std::string& msg, int code = 0)
  {
    return {err, msg, code};
  }
};

}