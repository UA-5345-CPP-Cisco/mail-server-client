#include <mail_server/handlers/HelloHandler.h>

namespace ISXMailServer {

Response HelloHandler(Request const& request)
{
  beast::error_code error;
  json::value input = json::parse(request.body(), error);

  if (error)
  {
    return MakeError(request, http::status::bad_request, "Invalid JSON");
  }

  if (!input.is_object())
  {
    return MakeError(request, http::status::bad_request, "Expected a JSON object");
  }

  auto const& object = input.as_object();

  auto const* name_value = object.if_contains("name");

  if (name_value == nullptr || !name_value->is_string())
  {
    return MakeError(request, http::status::bad_request, "\"name\" must be a string");
  }

  const std::string name = name_value->as_string().c_str();

  return MakeJsonResponse(
    request, http::status::ok, json::object{{"message", "Hello, " + name + "!"}});
}

} // namespace ISXMailServer
