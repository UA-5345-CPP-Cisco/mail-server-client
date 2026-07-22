#pragma once

#include <string>
#include <utility>
#include <vector>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

namespace ISXMailServerSDK::http {
namespace net = boost::asio;
namespace beast = boost::beast;
namespace boost_http = beast::http;
namespace json = boost::json;

using tcp = net::ip::tcp;

using Header = std::pair<std::string, std::string>;
using Headers = std::vector<Header>;

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

  [[nodiscard]] HttpResponse get(std::string target, Headers headers = {}) const;

  [[nodiscard]] HttpResponse post_json(std::string target, json::value body, Headers headers = {}) const;

  private:
  std::string m_host;
  std::string m_port;

  void ApplyHeaders(boost_http::request<boost_http::string_body>& request, const Headers& headers) const;
};
} // namespace ISXMailServerSDK::http
