#pragma once

#include "common.h"
#include "router.h"

class Server
{
  public:
  Server(net::io_context& io_context, tcp::endpoint endpoint, std::shared_ptr<Router const> router);

  void run();

  private:
  void do_accept();

  tcp::acceptor m_acceptor;

  std::shared_ptr<Router const> m_router;
};
