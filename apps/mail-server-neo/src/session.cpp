#include <mail_server/session.h>

Session::Session(
  tcp::socket socket,
  std::shared_ptr<Router const> router
  ) : m_socket(std::move(socket)), m_router(std::move(router))
{
}

void Session::run()
{
  do_read();
}

void Session::do_read()
{
  m_request = {};

  http::async_read(
    m_socket,
    m_buffer,
    m_request,
    beast::bind_front_handler(
      &Session::on_read,
      shared_from_this()));
}

void Session::on_read(beast::error_code error, std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (error == http::error::end_of_stream)
  {
    do_close();
    return;
  }

  if (error)
  {
    return;
  }

  do_write(m_router->dispatch(m_request));
}

void Session::do_write(Response response)
{
  const bool close = response.need_eof();

  m_response = std::make_shared<Response>(std::move(response));

  http::async_write(
    m_socket,
    *m_response,
    beast::bind_front_handler(
      &Session::on_write,
      shared_from_this(),
      close));
}

void Session::on_write(
  bool close,
  beast::error_code error,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (error)
  {
    return;
  }

  m_response = nullptr;

  if (close)
  {
    do_close();
    return;
  }

  do_read();
}

void Session::do_close()
{
  beast::error_code error;
  m_socket.shutdown(tcp::socket::shutdown_send, error);
}