#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <system_error>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class Client
{
  private:
  std::string ip_addr;
  int client_socket;
  int port;
  bool is_connected;

  public:
  Client(const std::string& ip, int port);

  ~Client();

  bool ConnectToServer();

  void Disconnect();

  bool SendCommand(const std::string& command);

  std::string ReceiveResponse();
};