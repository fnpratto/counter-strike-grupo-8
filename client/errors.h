#pragma once

#include <exception>
#include <string>

class ClientError: public std::exception {
    std::string message;

public:
    explicit ClientError(const std::string& message): message(message) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ServerDisconnectError: public ClientError {
public:
    ServerDisconnectError(): ClientError("client disconnected") {}
};
