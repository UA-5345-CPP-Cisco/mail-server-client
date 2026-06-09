// smtp_server.cpp
// Cross-platform: POSIX sockets (Linux) / Winsock2 (Windows)

#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

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

bool startsWithCI(const std::string& str, const std::string& prefix) {
    if (str.size() < prefix.size()) return false;
    for (size_t i = 0; i < prefix.size(); ++i) {
        if (std::toupper(static_cast<unsigned char>(str[i])) !=
            std::toupper(static_cast<unsigned char>(prefix[i])))
            return false;
    }
    return true;
}

std::string extractBrackets(const std::string& s) {
    auto a = s.find('<'), b = s.find('>');
    if (a != std::string::npos && b != std::string::npos && b > a)
        return s.substr(a + 1, b - a - 1);
    return "";
}

std::string trim(const std::string& s) {
    auto a = s.find_first_not_of(" \t\r\n");
    auto b = s.find_last_not_of(" \t\r\n");
    return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
}

void sendStr(SocketType sock, const std::string& msg) {
    send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "[Server] WSAStartup failed\n";
        return 1;
    }
#endif

    SocketType listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCK) {
        std::cerr << "[Server] socket() failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(listenSock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        std::cerr << "[Server] bind() failed\n";
        return 1;
    }

    listen(listenSock, 5);
    std::cout << "[Server] Listening on 127.0.0.1:" << PORT << "\n";

    while (true) {
        SocketType client = accept(listenSock, nullptr, nullptr);
        if (client == INVALID_SOCK) continue;

        std::cout << "[Server] Client connected.\n";
        sendStr(client, "220 TestSMTP Ready\r\n");

        int state = 0;
        bool inData = false;
        std::string body;
        char buf[BUFFER_SIZE];
        std::string lineBuf;
        bool quit = false;

        while (!quit) {
            int n = recv(client, buf, sizeof(buf) - 1, 0);
            if (n <= 0) break;
            buf[n] = '\0';
            lineBuf += buf;

            size_t pos;
            while ((pos = lineBuf.find('\n')) != std::string::npos) {
                std::string line = lineBuf.substr(0, pos + 1);
                lineBuf.erase(0, pos + 1);

                std::string resp;

                if (inData) {
                    if (trim(line) == ".") {
                        inData = false;
                        state = 1;
                        resp = "250 OK message accepted\r\n";
                        std::cout << "[Server] Message (" << body.size() << " bytes) received.\n";
                        body.clear();
                    } else {
                        body += line;
                    }
                }
                else if (startsWithCI(line, "HELO") || startsWithCI(line, "EHLO")) {
                    state = 1;
                    resp = "250 Hello " + trim(line.substr(4)) + "\r\n";
                }
                else if (startsWithCI(line, "MAIL FROM:")) {
                    if (state < 1) { resp = "503 Need HELO first\r\n"; }
                    else { state = 2; resp = "250 OK sender " + extractBrackets(line) + "\r\n"; }
                }
                else if (startsWithCI(line, "RCPT TO:")) {
                    if (state < 2) { resp = "503 Need MAIL FROM first\r\n"; }
                    else { state = 3; resp = "250 OK recipient " + extractBrackets(line) + "\r\n"; }
                }
                else if (startsWithCI(line, "DATA")) {
                    if (state < 3) { resp = "503 Need RCPT TO first\r\n"; }
                    else { inData = true; state = 4; resp = "354 Start input, end with <CRLF>.<CRLF>\r\n"; }
                }
                else if (startsWithCI(line, "QUIT")) {
                    resp = "221 Bye\r\n";
                    sendStr(client, resp);
                    std::cout << "[Server] " << resp;
                    quit = true;
                    break;
                }
                else if (startsWithCI(line, "NOOP")) { resp = "250 OK\r\n"; }
                else if (startsWithCI(line, "RSET")) { state = 1; resp = "250 OK reset\r\n"; }
                else { resp = "500 Unknown command\r\n"; }

                if (!resp.empty()) {
                    sendStr(client, resp);
                    std::cout << "[Server] " << resp;
                }
            }
        }

        closeSocket(client);
        std::cout << "[Server] Client disconnected.\n\n";
    }

    closeSocket(listenSock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}