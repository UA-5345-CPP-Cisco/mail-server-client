#include <iostream>

#include "aimap/Server.h"
#include "logger/Logger.h"

int main()
{
  auto& logger = Logger::GetInstance();
  if (!logger.Initialize("aimap_server.log", LogLevels::Trace, true))
  {
    std::cerr << "Failed to initialize logger" << std::endl;
    return 1;
  }

  Server server(4);
  server.StartServer();

  return 0;
}