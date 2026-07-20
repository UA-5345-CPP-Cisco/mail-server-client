#include <utility>

#include <mail_server_sdk/http/HttpClient.h>

namespace ISXMailServerSDK::http {
bool HttpResponse::is_success() const
{
  return status_code >= 200 && status_code < 300;
}

HttpClient::HttpClient(std::string host, std::string port) : m_host(std::move(host)), m_port(std::move(port))
{
}

HttpResponse HttpClient::get(std::string target, Headers headers) const
{
  net::io_context io_context;
  tcp::resolver resolver(io_context);
  beast::tcp_stream stream(io_context);

  auto const results = resolver.resolve(m_host, m_port);
  stream.connect(results);

  boost_http::request<boost_http::string_body> request{boost_http::verb::get, std::move(target), 11};

  request.set(boost_http::field::host, m_host);
  request.set(boost_http::field::user_agent, "mail-server-sdk");
  ApplyHeaders(request, headers);
  request.prepare_payload();

  boost_http::write(stream, request);

  beast::flat_buffer buffer;
  boost_http::response<boost_http::string_body> response;
  boost_http::read(stream, buffer, response);

  beast::error_code error;
  stream.socket().shutdown(tcp::socket::shutdown_both, error);

  HttpResponse result;
  result.status_code = response.result_int();
  result.reason = std::string(response.reason());
  result.raw_body = response.body();

  if (!result.raw_body.empty())
  {
    beast::error_code parse_error;
    result.body = json::parse(result.raw_body, parse_error);
    if (parse_error)
    {
      result.body = json::object{{"raw_body", result.raw_body}};
    }
  }
  else
  {
    result.body = nullptr;
  }

  return result;
}

HttpResponse HttpClient::post_json(std::string target, json::value body, Headers headers) const
{
  net::io_context io_context;
  tcp::resolver resolver(io_context);
  beast::tcp_stream stream(io_context);

  auto const results = resolver.resolve(m_host, m_port);
  stream.connect(results);

  boost_http::request<boost_http::string_body> request{boost_http::verb::post, std::move(target), 11};

  request.set(boost_http::field::host, m_host);
  request.set(boost_http::field::user_agent, "mail-server-sdk");
  request.set(boost_http::field::content_type, "application/json");
  ApplyHeaders(request, headers);
  request.body() = json::serialize(body);
  request.prepare_payload();

  boost_http::write(stream, request);

  beast::flat_buffer buffer;
  boost_http::response<boost_http::string_body> response;
  boost_http::read(stream, buffer, response);

  beast::error_code error;
  stream.socket().shutdown(tcp::socket::shutdown_both, error);

  HttpResponse result;
  result.status_code = response.result_int();
  result.reason = std::string(response.reason());
  result.raw_body = response.body();

  if (!result.raw_body.empty())
  {
    beast::error_code parse_error;
    result.body = json::parse(result.raw_body, parse_error);
    if (parse_error)
    {
      result.body = json::object{{"raw_body", result.raw_body}};
    }
  }
  else
  {
    result.body = nullptr;
  }

  return result;
}

void HttpClient::ApplyHeaders(boost_http::request<boost_http::string_body>& request, const Headers& headers) const
{
  for (const Header& header : headers)
  {
    request.set(header.first, header.second);
  }
}
} // namespace ISXMailServerSDK::http
