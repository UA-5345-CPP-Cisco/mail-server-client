#include "Client.hpp"

Client::Client(const std::string &ip, int port)
{
    this->ip_addr = ip;
    this->port = port;
    this->client_socket = -1;
    this->is_connected = false;
}

Client::~Client()
{
    this->Disconnect();
}

bool Client::ConnectToServer()
{
    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->client_socket == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to create socket");
    }
    struct sockaddr_in server_addr{};
    server_addr.sin_port = htons(this->port);
    server_addr.sin_family = AF_INET;
    if ((inet_pton(AF_INET, this->ip_addr.c_str(), &server_addr.sin_addr)) <= 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to set addr");
    }
    if ((connect(this->client_socket,(struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to connect");
    }
    this->is_connected = true;
    return true;
}

bool Client::SendCommand(const std::string &command)
{
    if (!this->is_connected)
    {
        return false;
    }
    std::string formatted_command = command + "\r\n";
    ssize_t bytesSent = send(this->client_socket, formatted_command.c_str(), formatted_command.length(), 0);
    if (bytesSent < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to send");
    }
    return true;
}

std::string Client::ReceiveResponse()
{
    if (!this->is_connected)
    {
        return "";
    }
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(this->client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to recv");
    }
    return std::string(buffer);
}

void Client::Disconnect()
{
    if (this->is_connected && this->client_socket != -1)
    {
        close(this->client_socket);
        this->client_socket = -1;
        this->is_connected = false;
    }
}

void Example()
{
    try {
        Client myClient("127.0.0.1", 8080); 
        std::cout << "Attempting to connect...\n";
        if (myClient.ConnectToServer()) {
            std::cout << "[Success] Connected to the server!\n";
            std::cout << "Sending command...\n";
            myClient.SendCommand("LOGIN roman mypassword123");
            std::string response = myClient.ReceiveResponse();
            std::cout << "[Server response]: " << response << "\n";
            myClient.Disconnect();
        }
    } catch (const std::system_error& e) {
        std::cerr << "[Exception] " << e.what() << " (Error code: " << e.code() << ")\n";
    }
}