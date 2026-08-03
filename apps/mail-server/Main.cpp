#include <iostream>

#include <mail_server/Handlers.h>
#include <mail_server/Router.h>
#include <mail_server/Server.h>
#include <mail_server/services/ServiceRegistry.h>

namespace ISXMailServer {

Router MakeRouter()
{
  Router router;

  router.Get("/mails", GetMailsHandler);
  router.Post("/hello", HelloHandler);
  router.Post("/login", LoginHandler);
  router.Post("/register", RegisterHandler);
  router.Post("/send-mail", SendMailHandler);

  return router;
}

} // namespace ISXMailServer

int main(int argument_count, char* arguments[])
{
  const std::string configuration_path = argument_count > 1 ? arguments[1] : "config/mail_server.json";

  ISXMailServer::ServiceRegistry::Initialize(configuration_path);
  const ISXMailServer::MailServerConfiguration& configuration =
    ISXMailServer::ServiceRegistry::Configurator().Configuration();

  net::io_context io_context;

  auto router = std::make_shared<ISXMailServer::Router>(ISXMailServer::MakeRouter());

  ISXMailServer::Server server(
    io_context, ISXMailServer::tcp::endpoint{ISXMailServer::tcp::v4(), configuration.server.port}, router);

  server.Run();
  ISXMailServer::ServiceRegistry::Logger().Log(ISXMailServer::LogLevel::Info, "Mail server started");
  io_context.run();
}
