#pragma once

#include "Common.h"
#include "Router.h"

namespace ISXMailServer {

class Server
{
  public:
  Server(net::io_context& io_context, tcp::endpoint endpoint, std::shared_ptr<Router const> router);

  void Run();

  private:
  void DoAccept();

  tcp::acceptor m_acceptor;

  std::shared_ptr<Router const> m_router;
};

} // namespace ISXMailServer
