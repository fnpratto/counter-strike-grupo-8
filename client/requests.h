#pragma once

#include <string>
#include <utility>

// @brief Represents a request to connect to a server.
class ConnectionRequest {
    std::string ip;
    std::string port;

public:
    inline explicit ConnectionRequest(std::string ip, std::string port):
            ip(std::move(ip)), port(std::move(port)) {}
    const std::string& get_ip() const { return ip; }
    const std::string& get_port() const { return port; }
};
