#pragma once

#include <exception>
#include <string>

class Error: public std::exception {
    std::string message;

public:
    explicit Error(const std::string& message): message(message) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ServerDisconnectError: public Error {
public:
    ServerDisconnectError(): Error("client disconnected") {}
};
