#include <mail_server/handlers/GetMailsHandler.h>
#include <mail_server/services/ServiceRegistry.h>

namespace ISXMailServer {

Response GetMailsHandler(Request const& request)
{
  const auto user_email_header = request.find("X-User-Email");

  if (user_email_header == request.end() || user_email_header->value().empty())
  {
    return MakeError(request, http::status::bad_request, "X-User-Email header is required");
  }

  const std::string user_email{user_email_header->value()};
  ServiceRegistry::Logger().Log(LogLevel::Debug, "Fetching mails for " + user_email);

  return MakeJsonResponse(
    request, http::status::ok, json::object{{"mails", ServiceRegistry::Storage().FindMailsForUser(user_email)}});
}

} // namespace ISXMailServer
