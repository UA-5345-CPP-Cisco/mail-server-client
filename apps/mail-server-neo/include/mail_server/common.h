#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

#include <memory>
#include <string>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

using tcp = net::ip::tcp;

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;

inline Response make_json_response(
  const http::request<http::string_body>& request,
  const http::status status,
  const json::value& body
)
{
  Response response(status, request.version());

  response.set(http::field::server, "boost-beast");
  response.set(http::field::content_type, "application/json");
  response.keep_alive(request.keep_alive());

  response.body() = json::serialize(body);
  response.prepare_payload();

  return response;
}

inline Response make_error(
  Request const& request,
  const http::status status,
  std::string_view message
)
{
  return make_json_response(
    request,
    status,
    json::object{
      {"error", message}
    });
}