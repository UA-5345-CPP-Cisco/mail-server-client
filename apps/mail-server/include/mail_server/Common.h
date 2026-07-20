#pragma once

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

namespace ISXMailServer {

using tcp = net::ip::tcp;

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;

inline Response
MakeJsonResponse(const http::request<http::string_body>& request, const http::status status, const json::value& body)
{
  Response response(status, request.version());

  response.set(http::field::server, "boost-beast");
  response.set(http::field::content_type, "application/json");
  response.keep_alive(request.keep_alive());

  response.body() = json::serialize(body);
  response.prepare_payload();

  return response;
}

inline Response MakeError(Request const& request, const http::status status, std::string_view message)
{
  return MakeJsonResponse(request, status, json::object{{"error", message}});
}

} // namespace ISXMailServer
