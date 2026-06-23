#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <system_error>

class Client
{
private:
    std::string ip_addr;
    int client_socket;
    int port;
    bool is_connected;

public: 
    Client(const std::string& ip,int port);

    ~Client();

    bool ConnectToServer();

    void Disconnect();

    bool SendCommand(const std::string& command);

    std::string ReceiveResponse();
};