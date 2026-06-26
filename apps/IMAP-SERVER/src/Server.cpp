#include "Server.hpp"

void Server::SetNonBlock(int fd)
{
    int current_flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, current_flags | O_NONBLOCK);
}

void Server::CheakTimeOut()
{
    std::unique_lock<std::shared_mutex> lock(this->map_mutex);
    time_t now = time(NULL);
    for (auto it = this->clinets.begin(); it != this->clinets.end();)
    {
        if ((now - it->second->last_activity) > TIMEOUT)
        {
            it = this->clinets.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Server::SetServerSocket(int port)
{
    int opt = 1;
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_fd == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to create socket");
    }
    if ((setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to setsockopt");
    }
    SetNonBlock(this->server_fd);
    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if ((bind(this->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to bind");
    }
    if ((listen(this->server_fd, SOMAXCONN)) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to listen");
    }
}

void Server::SetEpoll()
{
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to create epoll");
    }
    struct epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = this->server_fd;
    if ((epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->server_fd, &event)) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to epoll_ctl");
    }
}

void Server::HandleNewConnection()
{
    while (true)
    {
        int current_fd = accept(this->server_fd, NULL, NULL);
        if (current_fd == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else
            {
                std::cerr << "Accept error: " << errno << "\n";
                break;
            }
        }
        SetNonBlock(current_fd);
        {
            std::unique_lock<std::shared_mutex> lock(this->map_mutex);
            this->clinets.emplace(
                current_fd,
                std::make_shared<Client>(current_fd));
        }
        std::string msg = "Client connected. FD: " + std::to_string(current_fd);
        this->logger_thread.Enqueue([msg]()
                                    { Logger::GetInstance().LogDebug("Server::HandleNewConnection", msg); });
        struct epoll_event clinet_events{};
        clinet_events.events = EPOLLIN | EPOLLET,
        clinet_events.data.fd = current_fd;
        if ((epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, current_fd, &clinet_events)) == -1)
        {
            throw std::system_error(errno, std::generic_category(), "Failed to epoll_ctl");
        }
        std::cout << "New connection\n";
    }
}

void Server::HandleClientData(int client_fd)
{
    std::shared_ptr<Client> client_ptr = nullptr;
    {
        std::shared_lock<std::shared_mutex> lock(this->map_mutex);
        auto it = this->clinets.find(client_fd);
        if (it != this->clinets.end())
        {
            client_ptr = (it->second);
        }
    }
    if (!client_ptr)
    {
        return;
    }
    std::unique_lock<std::mutex> client_lock(client_ptr->client_lock);
    while (true)
    {
        char temp_buffer[BUF_SIZE];
        int bytes = recv(client_fd, temp_buffer, sizeof(temp_buffer), 0);
        if (bytes == 0)
        {
            client_lock.unlock();
            std::string msg = "Client disconnected gracefully. FD: " + std::to_string(client_fd);
            this->logger_thread.Enqueue([msg]()
                                        { Logger::GetInstance().LogDebug("Server::HandleClientData", msg); });
            std::unique_lock<std::shared_mutex> lock(this->map_mutex);
            this->clinets.erase(client_fd);
            return;
        }
        else if (bytes == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else
            {
                std::unique_lock<std::shared_mutex> lock(this->map_mutex);
                this->clinets.erase(client_fd);
                return;
            }
        }
        client_ptr->read_byffer.append(temp_buffer, bytes);
        client_ptr->last_activity = time(NULL);
    }
    auto &buffer = client_ptr->read_byffer;
    size_t pos = 0;
    while ((pos = buffer.find('\n')) != std::string::npos)
    {
        std::string json_message = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
        this->thread_pool.Enqueue([this, client_ptr, json_message]()
                                  { this->ProcessRequest(client_ptr->socket_fd, json_message); });
    }
}

void Server::ProcessRequest(int client_fd, const std::string &raw_json)
{
    std::string msg = "Start function for FD: " + std::to_string(client_fd);
    this->logger_thread.Enqueue([msg]()
                                { Logger::GetInstance().LogDebug("Server::ProcessRequest", msg); });

    try
    {
        boost::json::value parsed_value = boost::json::parse(raw_json);
        if (!parsed_value.is_object())
        {
            SendResponse(client_fd, R"({"status": "ERROR", "message": "Invalid JSON structure"})" + std::string("\n"));
            return;
        }
        boost::json::object &request = parsed_value.as_object();
        if (!request.contains("action"))
        {
            SendResponse(client_fd, R"({"status": "ERROR", "message": "Missing 'action' field"})" + std::string("\n"));
            return;
        }
        std::string action = request.at("action").as_string().c_str();
        if (action == "LOGIN")
        {
            this->logger_thread.Enqueue([raw_json]()
                                        { Logger::GetInstance().LogTrace("Server::ProcessRequest", raw_json, "Action: LOGIN"); });
            std::string email = request.contains("email") ? request.at("email").as_string().c_str() : "";
            std::string password = request.contains("password") ? request.at("password").as_string().c_str() : "";

            HandleLogin(client_fd, email, password);
        }
        else if (action == "LIST_EMAILS")
        {
            this->logger_thread.Enqueue([raw_json]()
                                        { Logger::GetInstance().LogTrace("Server::ProcessRequest", raw_json, "Action: LIST_EMAILS"); });
            HandleListEmails(client_fd);
        }
        else if (action == "READ_EMAIL")
        {
            this->logger_thread.Enqueue([raw_json]()
                                        { Logger::GetInstance().LogTrace("Server::ProcessRequest", raw_json, "Action: READ_EMAIL"); });
            int email_id = request.contains("email_id") ? request.at("email_id").as_int64() : -1;
            HandleReadEmail(client_fd, email_id);
        }
        else if (action == "QUIT")
        {
            this->logger_thread.Enqueue([raw_json]()
                                        { Logger::GetInstance().LogTrace("Server::ProcessRequest", raw_json, "Action: QUIT"); });
            HandleQuit(client_fd);
        }
        else
        {
            this->logger_thread.Enqueue([raw_json]()
                                        { Logger::GetInstance().LogTrace("Server::ProcessRequest", raw_json, "Action: UNKNOWN"); });
            SendResponse(client_fd, R"({"status": "ERROR", "message": "Unknown action"})" + std::string("\n"));
        }
    }
    catch (const std::exception &e)
    {
        std::string msg = "ERROR!!! Invalid JSON format from client " + std::to_string(client_fd) + ": " + e.what();
        this->logger_thread.Enqueue([msg]()
                                    { Logger::GetInstance().LogProd("Server::ProcessRequest", msg); });
        SendResponse(client_fd, R"({"status": "ERROR", "message": "Invalid JSON format"})" + std::string("\n"));
    }
}

void Server::SendResponse(int client_fd, const std::string &raw_json)
{
    std::string msg = "Sending to FD: " + std::to_string(client_fd);
    this->logger_thread.Enqueue([msg, raw_json]()
                                { Logger::GetInstance().LogTrace("Server::SendResponse", raw_json, msg); });
    size_t total_bytes = 0;
    while (total_bytes < raw_json.size())
    {
        ssize_t bytes = send(client_fd, raw_json.c_str() + total_bytes, raw_json.size() - total_bytes, MSG_NOSIGNAL);
        if (bytes == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return;
            }
            else
            {
                return;
            }
        }
        total_bytes += bytes;
    }
}

void Server::StartEpollLoop()
{
    while (true)
    {
        int n = epoll_wait(this->epoll_fd, this->events.data(), MAX_EVENTS, 1000);
        if (n == -1)
        {
            throw std::system_error(errno, std::generic_category(), "Failed to epoll_wait");
        }
        for (int i = 0; i < n; i++)
        {
            int temp_fd = events[i].data.fd;
            if (temp_fd == this->server_fd)
            {
                HandleNewConnection();
            }
            else
            {
                HandleClientData(temp_fd);
            }
        }
        CheakTimeOut();
    }
}

void Server::StartServer()
{
    this->logger_thread.Enqueue([]()
                                { Logger::GetInstance().LogProd("Server::StartServer", "*********Starting Server**********"); });
    SetServerSocket(PORT);
    SetEpoll();
    StartEpollLoop();
}

void Server::HandleLogin(int client_fd, const std::string &email, const std::string &password)
{
    std::string msg = "Email: " + email;
    this->logger_thread.Enqueue([msg]()
                                { Logger::GetInstance().LogTrace("Server::HandleLogin", msg, "Status: OK"); });
    std::cout << "[Thread " << std::this_thread::get_id() << "] Login attempt from: " << email << "\n";
    SendResponse(client_fd, R"({"status": "OK", "message": "Logged in successfully"})" + std::string("\n"));
}

void Server::HandleListEmails(int client_fd)
{
    SendResponse(client_fd, R"({"status": "OK", "emails": [{"id": 1, "subject": "Welcome!"}]})" + std::string("\n"));
}

void Server::HandleReadEmail(int client_fd, int email_id)
{
    SendResponse(client_fd, R"({"status": "OK", "body": "This is a test email body."})" + std::string("\n"));
}

void Server::HandleQuit(int client_fd)
{
    SendResponse(client_fd, R"({"status": "OK", "message": "Goodbye"})" + std::string("\n"));
}
