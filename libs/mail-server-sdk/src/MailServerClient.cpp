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

} // namespace ISXMailServerSDK
