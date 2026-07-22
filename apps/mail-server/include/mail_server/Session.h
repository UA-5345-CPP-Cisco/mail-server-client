#pragma once

#include "Common.h"
#include "Router.h"

namespace ISXMailServer {

class Session : public std::enable_shared_from_this<Session>
{
  public:
  Session(tcp::socket socket, std::shared_ptr<Router const> router);

  void Run();

  private:
  void DoRead();
  void OnRead(beast::error_code error, std::size_t bytes_transferred);

  void DoWrite(Response response);
  void OnWrite(bool close, beast::error_code error, std::size_t bytes_transferred);

  void DoClose();

  tcp::socket m_socket;
  beast::flat_buffer m_buffer;
  Request m_request;
  std::shared_ptr<Response> m_response;
  std::shared_ptr<Router const> m_router;
};

} // namespace ISXMailServer
