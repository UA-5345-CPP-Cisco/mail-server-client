#pragma once

#include "smtp/Event.hpp"
#include "smtp/ServerConfig.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace smtp
{

    // Socket boundary for the server. This component owns network I/O:
    // accepting clients, polling complete socket events, sending responses,
    // closing connections, and performing TLS upgrades.
    class ISocketsManager
    {
    public:
        virtual ~ISocketsManager() = default;

        virtual void Start(const ServerConfig &config) = 0;
        virtual std::vector<SmtpEvent> PollEvents() = 0;
        virtual void Send(ConnectionId connectionId, std::string_view message) = 0;
        virtual void Close(ConnectionId connectionId) = 0;
        virtual void StartTls(ConnectionId connectionId) = 0;
        virtual void Continue(ConnectionId connectionId) = 0;
        virtual void Stop() = 0;
    };

    // Boost.Asio-based sockets manager planned for this project.
    // It should not parse SMTP commands; it only moves complete events and bytes.
    //
    // Responsibilities:
    // - open and close the listening socket;
    // - accept incoming TCP clients;
    // - assign each client a ConnectionId;
    // - collect completed socket events for SmtpServer::RunOnce();
    // - send bytes produced by the SMTP session handler;
    // - close individual connections when requested;
    // - upgrade an existing connection to TLS after STARTTLS is accepted.
    class BoostSocketsManager : public ISocketsManager
    {
    public:
        BoostSocketsManager();
        ~BoostSocketsManager() override;

        void Start(const ServerConfig &config) override;
        std::vector<SmtpEvent> PollEvents() override;
        void Send(ConnectionId connectionId, std::string_view message) override;
        void Close(ConnectionId connectionId) override;
        void StartTls(ConnectionId connectionId) override;
        void Continue(ConnectionId connectionId) override;
        void Stop() override;

    private:
        struct Implementation;

        // Keep Boost.Asio details in the .cpp so the interface remains small and
        // other components do not need to include Boost headers.
        std::unique_ptr<Implementation> implementation_;
    };

}
