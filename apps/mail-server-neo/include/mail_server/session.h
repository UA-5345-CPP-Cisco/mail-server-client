#pragma once

#include "common.h"
#include "router.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(
    tcp::socket socket,
    std::shared_ptr<Router const> router);

  void run();

private:
  void do_read();
  void on_read(beast::error_code error, std::size_t bytes_transferred);

  void do_write(Response response);
  void on_write(
    bool close,
    beast::error_code error,
    std::size_t bytes_transferred);

  void do_close();

  tcp::socket m_socket;
  beast::flat_buffer m_buffer;
  Request m_request;
  std::shared_ptr<Response> m_response;
  std::shared_ptr<Router const> m_router;
};