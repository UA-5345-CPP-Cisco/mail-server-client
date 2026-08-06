#include <cstdint>
#include <stdexcept>
#include <string>

#include <mail_server/handlers/StarHandler.h>
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

std::string ReadRequiredUserEmail(Request const& request)
{
  const auto user_email_header = request.find("X-User-Email");
  if (user_email_header == request.end() || user_email_header->value().empty())
  {
    throw std::runtime_error("X-User-Email header is required");
  }

  return std::string{user_email_header->value()};
}

Response SetStarred(Request const& request, bool starred)
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
    const std::string user_email = ReadRequiredUserEmail(request);

    const bool updated = ServiceRegistry::Storage().SetStarred(message_id, user_email, starred);
    if (!updated)
    {
      return MakeError(request, http::status::not_found, "Message not found for user");
    }

    ServiceRegistry::Logger().Log(LogLevel::Info,
                                  "Message " + std::to_string(message_id) + (starred ? " starred" : " unstarred"));

    return MakeJsonResponse(
      request, http::status::ok, json::object{{"id", message_id}, {"starred", starred}, {"status", "ok"}});
  }
  catch (const std::exception& exception)
  {
    return MakeError(request, http::status::bad_request, exception.what());
  }
}

} // namespace

Response StarHandler(Request const& request)
{
  return SetStarred(request, true);
}

Response UnstarHandler(Request const& request)
{
  return SetStarred(request, false);
}

} // namespace ISXMailServer
