#include <utility>

#include <mail_server_sdk/MailServerClient.h>

namespace ISXMailServerSDK {

MailServerClient::MailServerClient(std::string host, std::string port) : m_http_client(std::move(host), std::move(port))
{
}

http::HttpResponse MailServerClient::hello(std::string name) const
{
  return m_http_client.post_json("/hello", boost::json::object{{"name", std::move(name)}});
}

http::HttpResponse MailServerClient::GetMails(std::string user_email) const
{
  return m_http_client.get("/mails", {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse
MailServerClient::SendMail(std::string from, std::vector<std::string> to, std::string subject, std::string body) const
{
  boost::json::array recipients;

  for (const std::string& recipient : to)
  {
    recipients.push_back(boost::json::string(recipient));
  }

  return m_http_client.post_json("/send-mail",
                                 boost::json::object{{"from", std::move(from)},
                                                     {"to", std::move(recipients)},
                                                     {"subject", std::move(subject)},
                                                     {"body", std::move(body)}});
}

} // namespace ISXMailServerSDK
