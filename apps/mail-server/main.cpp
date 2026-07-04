#include <iostream>

#include "mail_server_sdk/Server.h"

int main()
{
  Server server(4);
  server.StartServer();

  return 0;
}