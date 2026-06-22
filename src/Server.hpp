#pragma once
#define MAX_EVENTS 10000
#define TIMEOUT 30
#define BUF_SIZE 4096
#define PORT 2500

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include "json.hpp"
#include "Thread_pool.hpp"
#include "Logger.hpp"

enum class ClientState
{
    NotAuthenticated,
    Authenticated
};

class Client
{
private:
    friend class Server;
    int socket_fd;
    std::string read_byffer;
    ClientState state;
    time_t last_activity;

public:
    Client() : socket_fd(-1), last_activity(time(NULL)) {}

    Client(int socket_fd)
    {
        this->socket_fd = socket_fd;
        this->read_byffer = "";
        this->state = ClientState::NotAuthenticated;
        last_activity = time(NULL);
    }
};

class Server
{
private:
    int server_fd;
    int epoll_fd;
    std::unordered_map<int, Client> clinets;
    std::vector<struct epoll_event> events;
    ThreadPool thread_pool;
    ThreadPool logger_thread;
    std::mutex client_mutex;

    void SetNonBlock(int fd);
    void CheakTimeOut();
    void SetServerSocket(int port);
    void SetEpoll();
    void StartEpollLoop();

    void HandleNewConnection();
    void HandleClientData(int client_fd);
    void SendResponse(int client_fd, const std::string &json_response);
    void ProcessRequest(int client_fd, const std::string &raw_json);

    void HandleLogin(int client_fd, const std::string &email, const std::string &password);
    void HandleListEmails(int client_fd);
    void HandleReadEmail(int client_fd, int email_id);
    void HandleQuit(int client_fd);
public:
    Server(const unsigned int max_threads)
    {
        events.resize(MAX_EVENTS);
        this->thread_pool.Initialize(max_threads);
        this->logger_thread.Initialize(1);
    }
    Server(const Server &other) = delete;
    Server operator=(const Server &other) = delete;

    void StartServer();
};
