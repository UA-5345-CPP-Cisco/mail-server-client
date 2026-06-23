#include"Server.hpp"

int main(void)
{
    auto& logger=Logger::GetInstance();
    logger.Initialize("app.log", LogLevels::Trace, true);
    const unsigned int MAX_THREADS=std::thread::hardware_concurrency();
    Server server(MAX_THREADS);
    std::cout<<"Server started\n";
    server.StartServer();
    return 0;
    //./mail_server
} 