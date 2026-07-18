#include <iostream>

#include <mail_server/Handlers.h>
#include <mail_server/Router.h>
#include <mail_server/Server.h>

namespace ISXMailServer {

Router MakeRouter()
{
  Router router;

  router.Post("/hello", HelloHandler);

  return router;
}

} // namespace ISXMailServer

int main()
{
  net::io_context io_context;

  auto router = std::make_shared<ISXMailServer::Router>(ISXMailServer::MakeRouter());

  ISXMailServer::Server server(io_context, ISXMailServer::tcp::endpoint{ISXMailServer::tcp::v4(), 8080}, router);

  server.Run();
  io_context.run();
}
