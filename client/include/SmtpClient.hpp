#pragma once

#include <string>
#include <cstdint>

#include <boost/asio.hpp>

#include "MailBuilder.hpp"

namespace ISXSmtpClient
{

    class SmtpClient
    {
    public:
        SmtpClient(const std::string& server_address, std::uint16_t port);

        void set_server_address(const std::string& server_address);
        void set_port(std::uint16_t port);

        bool CheckConnection();
        bool SendMailMessage(const Mail& mail);

    private:
        std::string ReadResponse();
        bool SendCommand(const std::string& command);
        bool ExpectCode(const std::string& expected_code);

        std::string m_server_address;
        std::uint16_t m_port;
        boost::asio::io_context m_io_context;
        boost::asio::ip::tcp::socket m_socket;
    };

}