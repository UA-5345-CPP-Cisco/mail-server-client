#include "smtp_server_sdk/SmtpClient.h"

#include <string>

namespace Smtp {

SmtpClient::SmtpClient(
  const std::string& server_address,
  std::uint16_t port
  ) :
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
    const std::string line(
      boost::asio::buffers_begin(buffer.data()),
      boost::asio::buffers_end(buffer.data())
    );
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

SmtpReply SmtpClient::ReadReply()
{
  const std::string line = ReadResponse();
  SmtpReply reply;

  if (line.size() >= 3)
  {
    reply.code = std::stoi(line.substr(0, 3));
    reply.text = (line.size() > 4) ? line.substr(4) : "";
  }

  return reply;
}

bool SmtpClient::CheckConnection()
{
  try
  {
    m_socket = boost::asio::ip::tcp::socket(m_io_context);

    boost::asio::ip::tcp::resolver resolver(m_io_context);
    const auto endpoints = resolver.resolve(
      m_server_address,
      std::to_string(m_port)
    );

    boost::asio::connect(m_socket, endpoints);

    const auto reply = ReadReply();

    SendCommand("QUIT");
    m_socket.close();

    return reply.code == 220;
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

SmtpResult SmtpClient::SendMailMessage(const Mail& mail)
{
  try
  {
    m_socket = boost::asio::ip::tcp::socket(m_io_context);

    boost::asio::ip::tcp::resolver resolver(m_io_context);
    const auto endpoints = resolver.resolve(
      m_server_address,
      std::to_string(m_port)
    );

    boost::asio::connect(m_socket, endpoints);

    auto reply = ReadReply();
    if (reply.code != 220)
    {
      m_socket.close();
      return SmtpResult::Fail(SmtpError::ConnectionFailed,
                              "Server greeting failed: " + reply.text, reply.code);
    }

    SendCommand("EHLO client");
    reply = ReadReply();
    if (reply.code != 250)
    {
      m_socket.close();
      return SmtpResult::Fail(SmtpError::ServerError,
                              "EHLO rejected: " + reply.text, reply.code);
    }

    SendCommand("MAIL FROM:<" + mail.sender + ">");
    reply = ReadReply();
    if (reply.code != 250)
    {
      m_socket.close();
      return SmtpResult::Fail(SmtpError::ServerError,
                              "MAIL FROM rejected: " + reply.text, reply.code);
    }

    for (const std::string& recipient : mail.recipients)
    {
      SendCommand("RCPT TO:<" + recipient + ">");
      reply = ReadReply();
      if (reply.code != 250)
      {
        m_socket.close();
        return SmtpResult::Fail(SmtpError::RecipientRejected,
                                "Recipient <" + recipient + "> rejected: " + reply.text,
                                reply.code);
      }
    }

    SendCommand("DATA");
    reply = ReadReply();
    if (reply.code != 354)
    {
      m_socket.close();
      return SmtpResult::Fail(SmtpError::ServerError,
                              "DATA command rejected: " + reply.text, reply.code);
    }

    const std::string message =
      "From: " + mail.sender + "\r\n" +
      "Subject: " + mail.subject + "\r\n" +
      "\r\n" +
      mail.body + "\r\n" +
      ".";

    SendCommand(message);
    reply = ReadReply();
    if (reply.code != 250)
    {
      m_socket.close();
      return SmtpResult::Fail(SmtpError::ServerError,
                              "Message not accepted: " + reply.text, reply.code);
    }

    SendCommand("QUIT");
    m_socket.close();

    return SmtpResult::Success();
  }
  catch (const boost::system::system_error& e)
  {
    if (m_socket.is_open())
    {
      m_socket.close();
    }

    if (e.code() == boost::asio::error::host_not_found)
    {
      return SmtpResult::Fail(SmtpError::DnsResolutionFailed,
                              "Host not found: " + m_server_address);
    }

    if (e.code() == boost::asio::error::connection_refused)
    {
      return SmtpResult::Fail(SmtpError::ConnectionFailed,
                              "Connection refused on " + m_server_address + ":"
                                + std::to_string(m_port));
    }

    if (e.code() == boost::asio::error::timed_out)
    {
      return SmtpResult::Fail(SmtpError::Timeout,
                              "Connection timed out to " + m_server_address + ":"
                                + std::to_string(m_port));
    }

    return SmtpResult::Fail(SmtpError::Unknown, e.what());
  }
  catch (const std::exception& e)
  {
    if (m_socket.is_open())
    {
      m_socket.close();
    }

    return SmtpResult::Fail(SmtpError::Unknown, e.what());
  }
}

}