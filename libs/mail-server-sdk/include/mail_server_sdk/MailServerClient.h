#pragma once

#include <string>
#include <vector>

#include <mail_server_sdk/http/HttpClient.h>

namespace ISXMailServerSDK {
class MailServerClient
{
  public:
  MailServerClient(std::string host, std::string port);

  [[nodiscard]] http::HttpResponse hello(std::string name) const;

  [[nodiscard]] http::HttpResponse GetMails(std::string user_email) const;

  [[nodiscard]] http::HttpResponse
  SendMail(std::string from, std::vector<std::string> to, std::string subject, std::string body) const;

  private:
  http::HttpClient m_http_client;
};
} // namespace ISXMailServerSDK
