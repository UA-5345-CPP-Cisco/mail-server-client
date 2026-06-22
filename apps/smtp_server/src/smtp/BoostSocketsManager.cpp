#include "smtp/SocketsManager.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <chrono>
#include <deque>
#include <istream>
#include <memory>
#include <queue>
#include <string>
#include <system_error>
#include <unordered_map>
#include <utility>

namespace smtp {

namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;

struct BoostSocketsManager::Implementation {
    struct ConnectionState {
        ConnectionId id{0};
        std::shared_ptr<ssl::stream<tcp::socket>> stream;
        boost::asio::streambuf readBuffer;
        std::deque<std::string> pendingWrites;
        bool tlsEnabled{false};
        bool tlsHandshakePending{false};
        bool writeInProgress{false};
        bool readInProgress{false};
        bool closeRequested{false};
        bool closed{false};
    };

    asio::io_context ioContext;
    ssl::context sslContext{ssl::context::tls_server};
    std::unique_ptr<tcp::acceptor> acceptor;
    ServerConfig config;
    bool isRunning{false};
    ConnectionId nextConnectionId{1};
    std::queue<SmtpEvent> readyEvents;
    std::unordered_map<ConnectionId, std::shared_ptr<ConnectionState>> connections;

    void Start(const ServerConfig& serverConfig)
    {
        config = serverConfig;
        ioContext.restart();

        if (config.tls.enabled) {
            sslContext.set_options(
                ssl::context::default_workarounds |
                ssl::context::no_sslv2 |
                ssl::context::no_sslv3 |
                ssl::context::single_dh_use);
            sslContext.use_certificate_chain_file(config.tls.certificatePath);
            sslContext.use_private_key_file(config.tls.privateKeyPath, ssl::context::pem);
        }

        const asio::ip::address address = asio::ip::make_address(config.host);
        const tcp::endpoint endpoint(address, config.port);

        acceptor = std::make_unique<tcp::acceptor>(ioContext);
        acceptor->open(endpoint.protocol());
        acceptor->set_option(tcp::acceptor::reuse_address(true));
        acceptor->bind(endpoint);
        acceptor->listen(static_cast<int>(config.maxConnections));

        isRunning = true;
        StartAccept();
    }

    void StartAccept()
    {
        if (!isRunning || !acceptor || !acceptor->is_open()) {
            return;
        }

        auto stream = std::make_shared<ssl::stream<tcp::socket>>(ioContext, sslContext);
        acceptor->async_accept(stream->next_layer(), [this, stream](const boost::system::error_code& error) {
            if (!isRunning) {
                return;
            }

            if (!error) {
                auto connection = std::make_shared<ConnectionState>();
                connection->id = nextConnectionId++;
                connection->stream = stream;
                connections.emplace(connection->id, connection);
                QueueEvent(SmtpEvent{SmtpEventType::Connected, connection->id, {}});
            }

            StartAccept();
        });
    }

    void Continue(ConnectionId connectionId)
    {
        auto connection = FindConnection(connectionId);
        if (!connection || connection->closeRequested || connection->readInProgress) {
            return;
        }

        if (connection->tlsHandshakePending) {
            if (!connection->writeInProgress && connection->pendingWrites.empty()) {
                StartTlsHandshake(connection);
            }
            return;
        }

        StartRead(connection);
    }

    void StartRead(const std::shared_ptr<ConnectionState>& connection)
    {
        if (!connection || connection->closeRequested || connection->readInProgress) {
            return;
        }

        connection->readInProgress = true;

        auto handler = [this, connection](const boost::system::error_code& error, std::size_t) {
            connection->readInProgress = false;

            if (error) {
                FinishClose(connection, error != asio::error::operation_aborted);
                return;
            }

            std::istream input(&connection->readBuffer);
            std::string line;
            std::getline(input, line);
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            QueueEvent(SmtpEvent{SmtpEventType::MessageReceived, connection->id, std::move(line)});
        };

        if (connection->tlsEnabled) {
            asio::async_read_until(*connection->stream, connection->readBuffer, '\n', std::move(handler));
        } else {
            asio::async_read_until(connection->stream->next_layer(), connection->readBuffer, '\n',
                                   std::move(handler));
        }
    }

    void Send(ConnectionId connectionId, std::string message)
    {
        asio::post(ioContext, [this, connectionId, message = std::move(message)]() mutable {
            auto connection = FindConnection(connectionId);
            if (!connection || connection->closeRequested) {
                return;
            }

            connection->pendingWrites.push_back(std::move(message));
            if (!connection->writeInProgress) {
                StartWrite(connection);
            }
        });
    }

    void StartWrite(const std::shared_ptr<ConnectionState>& connection)
    {
        if (!connection || connection->writeInProgress || connection->pendingWrites.empty()) {
            return;
        }

        connection->writeInProgress = true;
        const auto buffer = asio::buffer(connection->pendingWrites.front());

        auto handler = [this, connection](const boost::system::error_code& error, std::size_t) {
            connection->writeInProgress = false;

            if (error) {
                FinishClose(connection, error != asio::error::operation_aborted);
                return;
            }

            connection->pendingWrites.pop_front();

            if (!connection->pendingWrites.empty()) {
                StartWrite(connection);
                return;
            }

            if (connection->tlsHandshakePending) {
                StartTlsHandshake(connection);
                return;
            }

            if (connection->closeRequested) {
                FinishClose(connection, true);
            }
        };

        if (connection->tlsEnabled) {
            asio::async_write(*connection->stream, buffer, std::move(handler));
        } else {
            asio::async_write(connection->stream->next_layer(), buffer, std::move(handler));
        }
    }

    void StartTls(ConnectionId connectionId)
    {
        asio::post(ioContext, [this, connectionId] {
            auto connection = FindConnection(connectionId);
            if (!connection || connection->closeRequested || connection->tlsEnabled) {
                return;
            }

            connection->tlsHandshakePending = true;

            if (!connection->writeInProgress && connection->pendingWrites.empty()) {
                StartTlsHandshake(connection);
            }
        });
    }

    void StartTlsHandshake(const std::shared_ptr<ConnectionState>& connection)
    {
        if (!connection || connection->closeRequested) {
            return;
        }

        connection->tlsHandshakePending = false;
        connection->stream->async_handshake(ssl::stream_base::server,
                                            [this, connection](const boost::system::error_code& error) {
            if (error) {
                QueueEvent(SmtpEvent{SmtpEventType::TlsFailed, connection->id, error.message()});
                FinishClose(connection, true);
                return;
            }

            connection->tlsEnabled = true;
            QueueEvent(SmtpEvent{SmtpEventType::TlsSucceeded, connection->id, {}});
        });
    }

    void Close(ConnectionId connectionId)
    {
        asio::post(ioContext, [this, connectionId] {
            auto connection = FindConnection(connectionId);
            if (!connection) {
                return;
            }

            connection->closeRequested = true;
            if (!connection->writeInProgress && connection->pendingWrites.empty()) {
                FinishClose(connection, true);
            }
        });
    }

    void FinishClose(const std::shared_ptr<ConnectionState>& connection, bool emitDisconnected)
    {
        if (!connection || connection->closed) {
            return;
        }

        connection->closeRequested = true;
        connection->closed = true;

        boost::system::error_code ignored;
        if (connection->tlsEnabled) {
            connection->stream->shutdown(ignored);
        }

        connection->stream->next_layer().shutdown(tcp::socket::shutdown_both, ignored);
        connection->stream->next_layer().close(ignored);

        connections.erase(connection->id);

        if (emitDisconnected) {
            QueueEvent(SmtpEvent{SmtpEventType::Disconnected, connection->id, {}});
        }
    }

    std::shared_ptr<ConnectionState> FindConnection(ConnectionId connectionId)
    {
        const auto connection = connections.find(connectionId);
        if (connection == connections.end()) {
            return {};
        }

        return connection->second;
    }

    void QueueEvent(SmtpEvent event)
    {
        readyEvents.push(std::move(event));
    }

    std::vector<SmtpEvent> PollEvents()
    {
        if (isRunning) {
            ioContext.restart();
            ioContext.run_for(std::chrono::milliseconds(10));
        }

        std::vector<SmtpEvent> events;
        while (!readyEvents.empty()) {
            events.push_back(std::move(readyEvents.front()));
            readyEvents.pop();
        }

        return events;
    }

    void Stop()
    {
        if (!isRunning) {
            return;
        }

        isRunning = false;

        boost::system::error_code ignored;
        if (acceptor) {
            acceptor->close(ignored);
        }

        for (auto& entry : connections) {
            auto& connection = entry.second;
            connection->closeRequested = true;
            connection->stream->next_layer().shutdown(tcp::socket::shutdown_both, ignored);
            connection->stream->next_layer().close(ignored);
        }

        connections.clear();
        ioContext.stop();
    }
};

BoostSocketsManager::BoostSocketsManager()
    : implementation_(std::make_unique<Implementation>())
{
}

BoostSocketsManager::~BoostSocketsManager() = default;

void BoostSocketsManager::Start(const ServerConfig& config)
{
    implementation_->Start(config);
}

std::vector<SmtpEvent> BoostSocketsManager::PollEvents()
{
    return implementation_->PollEvents();
}

void BoostSocketsManager::Send(ConnectionId connectionId, std::string_view message)
{
    implementation_->Send(connectionId, std::string(message));
}

void BoostSocketsManager::Close(ConnectionId connectionId)
{
    implementation_->Close(connectionId);
}

void BoostSocketsManager::StartTls(ConnectionId connectionId)
{
    implementation_->StartTls(connectionId);
}

void BoostSocketsManager::Continue(ConnectionId connectionId)
{
    implementation_->Continue(connectionId);
}

void BoostSocketsManager::Stop()
{
    implementation_->Stop();
}

}
