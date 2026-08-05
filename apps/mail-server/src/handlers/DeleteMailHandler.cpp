#include <cstdint>
#include <stdexcept>
#include <string>

#include <mail_server/handlers/DeleteMailHandler.h>
#include <mail_server/services/ServiceRegistry.h>

namespace ISXMailServer {

namespace {

std::int64_t ReadRequiredInt64(const json::object& object, std::string_view field)
{
  auto const* value = object.if_contains(field);
  if (value == nullptr || (!value->is_int64() && !value->is_uint64()))
  {
    throw std::runtime_error("\"" + std::string{field} + "\" must be an integer");
  }

  return value->is_int64() ? value->as_int64() : static_cast<std::int64_t>(value->as_uint64());
}

} // namespace

Response DeleteMailHandler(Request const& request)
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

  try
  {
    const json::object& object = input.as_object();
    const std::int64_t message_id = ReadRequiredInt64(object, "id");

    const bool deleted = ServiceRegistry::Storage().DeleteMail(message_id);
    if (!deleted)
    {
      return MakeError(request, http::status::not_found, "Message not found");
    }

    ServiceRegistry::Logger().Log(LogLevel::Info, "Message " + std::to_string(message_id) + " deleted");

    return MakeJsonResponse(
      request, http::status::ok, json::object{{"id", message_id}, {"status", "deleted"}});
  }
  catch (const std::exception& exception)
  {
    return MakeError(request, http::status::bad_request, exception.what());
  }
}

} // namespace ISXMailServer
