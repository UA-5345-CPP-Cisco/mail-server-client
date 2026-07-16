//
// Created by  Pavlo Kilko on 14.07.2026.
//

#include <mail_server/server.h>
#include <mail_server/session.h>

Server::Server(net::io_context& io_context,
               tcp::endpoint endpoint,
               std::shared_ptr<Router const> router) :
  m_acceptor(io_context),
  m_router(std::move(router))
{
  beast::error_code error;

  m_acceptor.open(endpoint.protocol(), error);
  if (error)
  {
    throw beast::system_error(error);
  }

  m_acceptor.set_option(net::socket_base::reuse_address(true), error);
  if (error)
  {
    throw beast::system_error(error);
  }

  m_acceptor.bind(endpoint, error);
  if (error)
  {
    throw beast::system_error(error);
  }

  m_acceptor.listen(net::socket_base::max_listen_connections, error);
  if (error)
  {
    throw beast::system_error(error);
  }
}

void Server::run()
{
  do_accept();
}

void Server::do_accept()
{
  m_acceptor.async_accept(
    [this](const beast::error_code& error, tcp::socket socket)
    {
      if (!error)
      {
        std::make_shared<Session>(std::move(socket), m_router)->run();
      }

      do_accept();
    });
}
