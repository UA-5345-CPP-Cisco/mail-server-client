#pragma once

#include <string>

#include <mail_server_sdk/http/HttpClient.h>

namespace ISXMailServerSDK {
class MailServerClient
{
  public:
  MailServerClient(std::string host, std::string port);

  [[nodiscard]] http::HttpResponse hello(std::string name) const;

  private:
  http::HttpClient m_http_client;
};
} // namespace ISXMailServerSDK
