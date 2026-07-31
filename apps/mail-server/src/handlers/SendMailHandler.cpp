#include <stdexcept>
#include <string>
#include <vector>

#include <mail_server/handlers/SendMailHandler.h>
#include <mail_server/services/ServiceRegistry.h>
#include <smtp_server_sdk/MailBuilder.h>
#include <smtp_server_sdk/SmtpClient.h>

namespace ISXMailServer {

namespace {

std::string ReadRequiredString(const json::object& object, std::string_view field)
{
  auto const* value = object.if_contains(field);
  if (value == nullptr || !value->is_string())
  {
    throw std::runtime_error("\"" + std::string{field} + "\" must be a string");
  }

  return value->as_string().c_str();
}

std::vector<std::string> ReadRecipients(const json::object& object)
{
  auto const* value = object.if_contains("to");
  if (value == nullptr || !value->is_array())
  {
    throw std::runtime_error("\"to\" must be an array of strings");
  }

  std::vector<std::string> recipients;
  for (const json::value& recipient : value->as_array())
  {
    if (!recipient.is_string())
    {
      throw std::runtime_error("\"to\" must be an array of strings");
    }

    recipients.emplace_back(recipient.as_string().c_str());
  }

  if (recipients.empty())
  {
    throw std::runtime_error("\"to\" must contain at least one recipient");
  }

  return recipients;
}

} // namespace

Response SendMailHandler(Request const& request)
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
    const std::string from = ReadRequiredString(object, "from");
    const std::vector<std::string> recipients = ReadRecipients(object);
    const std::string subject = ReadRequiredString(object, "subject");
    const std::string body = ReadRequiredString(object, "body");

    const MailServerConfiguration& configuration = ServiceRegistry::Configurator().Configuration();
    Smtp::SmtpClient smtp_client(configuration.smtp.host, configuration.smtp.port);
    Smtp::Mail mail = Smtp::MailBuilder()
                        .SetSender(from)
                        .AddRecipients(recipients)
                        .SetSubject(subject)
                        .SetBody(body)
                        .Build();

    const Smtp::SmtpResult result = smtp_client.SendMailMessage(mail);

    if (!result.ok())
    {
      ServiceRegistry::Logger().Log(LogLevel::Warning, "SMTP delivery failed: " + result.message);
      return MakeJsonResponse(request,
                              http::status::bad_gateway,
                              json::object{{"status", "failed"},
                                           {"message", result.message},
                                           {"smtp_code", result.smtpCode}});
    }

    ServiceRegistry::Logger().Log(LogLevel::Info, "Mail submitted to SMTP server from " + from);
    return MakeJsonResponse(request,
                            http::status::accepted,
                            json::object{{"status", "accepted"},
                                         {"message", result.message},
                                         {"smtp_code", result.smtpCode}});
  }
  catch (const std::exception& exception)
  {
    return MakeError(request, http::status::bad_request, exception.what());
  }
}

} // namespace ISXMailServer
