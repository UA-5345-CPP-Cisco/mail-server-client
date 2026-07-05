#include "smtp_server_sdk/SmtpClient.h"

#include <string>

namespace Smtp {

SmtpClient::SmtpClient(const std::string& server_address, std::uint16_t port) :
  m_server_address(server_address),
  m_port(port),
  m_socket(m_io_context)
{
}

void SmtpClient::set_server_address(const std::string& server_address)
{
  m_server_address = server_address;
}

void SmtpClient::set_port(std::uint16_t port)
{
  m_port = port;
}

std::string SmtpClient::ReadResponse()
{
  std::string last_line;

  while (true)
  {
    boost::asio::streambuf buffer;
    boost::asio::read_until(m_socket, buffer, "\r\n");
    const std::string line(boost::asio::buffers_begin(buffer.data()),
                           boost::asio::buffers_end(buffer.data()));
    last_line = line;

    if (line.size() >= 4 && line[3] == ' ')
    {
      break;
    }
  }

  return last_line;
}

void SmtpClient::SendCommand(const std::string& command)
{
  boost::asio::write(m_socket, boost::asio::buffer(command + "\r\n"));
}

bool SmtpClient::ExpectCode(const std::string& expected_code)
{
  const std::string response = ReadResponse();
  return response.substr(0, expected_code.size()) == expected_code;
}

bool SmtpClient::CheckConnection()
{
  try
  {
    m_socket = boost::asio::ip::tcp::socket(m_io_context);

    boost::asio::ip::tcp::resolver resolver(m_io_context);
    const auto endpoints = resolver.resolve(m_server_address, std::to_string(m_port));

    boost::asio::connect(m_socket, endpoints);

    const bool greeting_ok = ExpectCode("220");

    SendCommand("QUIT");
    m_socket.close();

    return greeting_ok;
  }
  catch (const std::exception&)
  {
    if (m_socket.is_open())
    {
      m_socket.close();
    }

    return false;
  }
}

bool SmtpClient::SendMailMessage(const Mail& mail)
{
  try
  {
    m_socket = boost::asio::ip::tcp::socket(m_io_context);

    boost::asio::ip::tcp::resolver resolver(m_io_context);
    const auto endpoints = resolver.resolve(m_server_address, std::to_string(m_port));

    boost::asio::connect(m_socket, endpoints);

    if (!ExpectCode("220"))
    {
      m_socket.close();
      return false;
    }

    SendCommand("EHLO client");
    if (!ExpectCode("250"))
    {
      m_socket.close();
      return false;
    }

    SendCommand("MAIL FROM:<" + mail.sender + ">");
    if (!ExpectCode("250"))
    {
      m_socket.close();
      return false;
    }

    for (const std::string& recipient : mail.recipients)
    {
      SendCommand("RCPT TO:<" + recipient + ">");
      if (!ExpectCode("250"))
      {
        m_socket.close();
        return false;
      }
    }

    SendCommand("DATA");
    if (!ExpectCode("354"))
    {
      m_socket.close();
      return false;
    }

    const std::string message = "From: " + mail.sender + "\r\n" + "Subject: " + mail.subject +
                                "\r\n" + "\r\n" + mail.body + "\r\n" + ".";

    SendCommand(message);
    if (!ExpectCode("250"))
    {
      m_socket.close();
      return false;
    }

    SendCommand("QUIT");
    m_socket.close();

    return true;
  }
  catch (const std::exception&)
  {
    if (m_socket.is_open())
    {
      m_socket.close();
    }

    return false;
  }
}

} // namespace Smtp
