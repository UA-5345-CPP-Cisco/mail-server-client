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

http::HttpResponse MailServerClient::Login(std::string email, std::string password) const
{
  return m_http_client.post_json("/login",
                                 boost::json::object{{"email", std::move(email)}, {"password", std::move(password)}});
}

http::HttpResponse MailServerClient::Register(std::string username, std::string email, std::string password) const
{
  return m_http_client.post_json("/register",
                                 boost::json::object{{"username", std::move(username)},
                                                     {"email", std::move(email)},
                                                     {"password", std::move(password)}});
}

http::HttpResponse MailServerClient::GetMails(std::string user_email) const
{
  return m_http_client.get("/mails", {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::SendMail(std::string from,
                                              std::vector<std::string> to,
                                              std::string subject,
                                              std::string body) const
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

http::HttpResponse MailServerClient::StarMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/star", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::UnstarMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/unstar", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::ArchiveMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/archive", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::UnarchiveMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/unarchive", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::ReadMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/read", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::UnreadMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/unread", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

http::HttpResponse MailServerClient::DeleteMail(std::int64_t message_id, std::string user_email) const
{
  return m_http_client.post_json(
    "/mails/delete", boost::json::object{{"id", message_id}}, {{"X-User-Email", std::move(user_email)}});
}

} // namespace ISXMailServerSDK
