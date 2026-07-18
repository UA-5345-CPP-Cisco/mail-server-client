#pragma once

#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

namespace ISXMailServerSDK::http {
namespace net = boost::asio;
namespace beast = boost::beast;
namespace boost_http = beast::http;
namespace json = boost::json;

using tcp = net::ip::tcp;

struct HttpResponse
{
  unsigned status_code{};
  std::string reason;
  std::string raw_body;
  json::value body;

  [[nodiscard]] bool is_success() const;
};

class HttpClient
{
  public:
  HttpClient(std::string host, std::string port);

  [[nodiscard]] HttpResponse post_json(std::string target, json::value body) const;

  private:
  std::string m_host;
  std::string m_port;
};
} // namespace ISXMailServerSDK::http
