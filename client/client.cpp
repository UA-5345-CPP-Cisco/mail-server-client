// smtp_client.cpp
// Cross-platform: POSIX sockets (Linux) / Winsock2 (Windows)

#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using SocketType = SOCKET;
constexpr SocketType INVALID_SOCK = INVALID_SOCKET;
inline int closeSocket(SocketType s) { return closesocket(s); }
#else
#include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    using SocketType = int;
    constexpr SocketType INVALID_SOCK = -1;
    inline int closeSocket(SocketType s) { return close(s); }
#endif

constexpr int PORT = 25000;
constexpr int BUFFER_SIZE = 4096;

std::string recvLine(SocketType sock) {
    char buf[BUFFER_SIZE];
    int n = recv(sock, buf, sizeof(buf) - 1, 0);
    if (n <= 0) return "";
    buf[n] = '\0';
    return std::string(buf);
}

std::string sendCmd(SocketType sock, const std::string& cmd) {
    std::string full = cmd + "\r\n";
    send(sock, full.c_str(), static_cast<int>(full.size()), 0);
    std::cout << "C: " << cmd << "\n";

    std::string resp = recvLine(sock);
    std::cout << "S: " << resp;
    return resp;
}

void sendRaw(SocketType sock, const std::string& data) {
    send(sock, data.c_str(), static_cast<int>(data.size()), 0);
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "[Client] WSAStartup failed\n";
        return 1;
    }
#endif

    SocketType sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCK) {
        std::cerr << "[Client] socket() failed\n";
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        std::cerr << "[Client] Failed to connect to 127.0.0.1:" << PORT << "\n";
        closeSocket(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Read greeting
    std::string greeting = recvLine(sock);
    std::cout << "S: " << greeting;

    // SMTP dialogue
    sendCmd(sock, "HELO localhost");
    sendCmd(sock, "MAIL FROM:<vlad@test.com>");
    sendCmd(sock, "RCPT TO:<colleague@test.com>");
    sendCmd(sock, "DATA");

    // Send body + terminator
    std::string body =
            "From: vlad@test.com\r\n"
            "To: colleague@test.com\r\n"
            "Subject: Hello from SMTP client\r\n"
            "\r\n"
            "This is a test email.\r\n"
            "Second line of body.\r\n"
            ".\r\n";

    sendRaw(sock, body);
    std::cout << "C: (message body)\nC: .\n";

    std::string dataResp = recvLine(sock);
    std::cout << "S: " << dataResp;

    sendCmd(sock, "QUIT");

    closeSocket(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}