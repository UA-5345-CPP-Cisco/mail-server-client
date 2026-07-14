#include <iostream>
#include <mail_server/router.h>
#include <mail_server/handlers.h>
#include <mail_server/server.h>

Router make_router()
{
  Router router;

  router.post("/hello", hello_handler);

  return router;
}


int main()
{
  net::io_context io_context;

  auto router = std::make_shared<Router>(make_router());

  Server server(
    io_context,
    tcp::endpoint{tcp::v4(), 8080},
    router);

  server.run();
  io_context.run();
}
