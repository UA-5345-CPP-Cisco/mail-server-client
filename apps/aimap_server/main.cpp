#include <iostream>

#include "aimap/Server.h"

int main()
{
  Server server(4);
  server.StartServer();

  return 0;
}