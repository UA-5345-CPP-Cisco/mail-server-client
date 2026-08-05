#include <mail_server/Session.h>

namespace ISXMailServer {

Session::Session(tcp::socket socket, std::shared_ptr<Router const> router) :
  m_socket(std::move(socket)),
  m_router(std::move(router))
{
}

void Session::Run()
{
  DoRead();
}

void Session::DoRead()
{
  m_request = {};

  http::async_read(m_socket, m_buffer, m_request, beast::bind_front_handler(&Session::OnRead, shared_from_this()));
}

void Session::OnRead(beast::error_code error, std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (error == http::error::end_of_stream)
  {
    DoClose();
    return;
  }

  if (error)
  {
    return;
  }

  DoWrite(m_router->Dispatch(m_request));
}

void Session::DoWrite(Response response)
{
  const bool close = response.need_eof();

  m_response = std::make_shared<Response>(std::move(response));

  http::async_write(m_socket, *m_response, beast::bind_front_handler(&Session::OnWrite, shared_from_this(), close));
}

void Session::OnWrite(bool close, beast::error_code error, std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (error)
  {
    return;
  }

  m_response = nullptr;

  if (close)
  {
    DoClose();
    return;
  }

  DoRead();
}

void Session::DoClose()
{
  beast::error_code error;
  m_socket.shutdown(tcp::socket::shutdown_send, error);
}

} // namespace ISXMailServer
