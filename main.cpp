#include <iostream>
#include <string>

// Boost includes
#include <boost/asio.hpp>

// Crypto++ includes
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

int main() {
    std::cout << "--- Mail Server Component Quick Test ---\n" << std::endl;

    try {
        // 1. Use Boost.Asio to parse/validate an IP address string
        std::string ip_string = "127.0.0.1";
        boost::system::error_code ec;
        boost::asio::ip::address ip_address = boost::asio::ip::make_address(ip_string, ec);

        if (ec) {
            std::cerr << "Boost Error: Invalid IP address: " << ec.message() << std::endl;
            return 1;
        }
        std::cout << "[Boost Asio] Successfully validated IP: " << ip_address.to_string() << std::endl;

        // 2. Use Crypto++ to compute a SHA-256 hash of the IP address
        std::string hash_input = "Server-IP:" + ip_address.to_string();
        std::string hash_output;

        CryptoPP::SHA256 hash;
        CryptoPP::StringSource ss(
            hash_input,
            true,
            new CryptoPP::HashFilter(
                hash,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hash_output),
                    true // UpperCase output
                )
            )
        );

        std::cout << "[Crypto++] Input string:  " << hash_input << std::endl;
        std::cout << "[Crypto++] SHA-256 Hash: " << hash_output << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Standard Exception caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}