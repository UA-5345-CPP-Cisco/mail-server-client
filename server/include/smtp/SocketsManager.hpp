#pragma once

#include "smtp/Event.hpp"
#include "smtp/ServerConfig.hpp"

#include <string_view>
#include <vector>

namespace smtp {

    //Boost-implemented for our project, but you can go with just linux if you'd like
class ISocketsManager {
public:
    virtual ~ISocketsManager() = default;

    virtual void Start(const ServerConfig& config) = 0;
    virtual std::vector<SmtpEvent> PollEvents() = 0;
    virtual void Send(ConnectionId connectionId, std::string_view message) = 0;
    virtual void Close(ConnectionId connectionId) = 0;
    virtual void StartTls(ConnectionId connectionId) = 0;
    virtual void Stop() = 0;
};

}
