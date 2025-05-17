#pragma once

#include <exception>
#include <string>

class ServerError: public std::exception {
    std::string message;

public:
    explicit ServerError(const std::string& message): message(message) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ServerDisconnectError: public ServerError {
public:
    ServerDisconnectError(): ServerError("client disconnected") {}
};

class GameError: public ServerError {
public:
    explicit GameError(const std::string& message): ServerError(message) {}
    const char* what() const noexcept override {
        static std::string full_message = "Game Error: " + std::string(ServerError::what());
        return full_message.c_str();
    }
};

class GameExistsError: public GameError {
public:
    GameExistsError(): GameError("game already exists") {}
};

class JoinGameError: public GameError {
public:
    JoinGameError(): GameError("game not found") {}
};

class StartGameError: public GameError {
public:
    StartGameError(): GameError("try to start an already started game") {}
};

class SelectTeamError : public GameError {
public:
    SelectTeamError(): GameError("error at select team") {}
};

class InvalidPlayerNameError : public GameError {
public:
    InvalidPlayerNameError(): GameError("invalid player name") {}
};

class PlayerError: public ServerError {
public:
    explicit PlayerError(const std::string& message): ServerError(message) {}
    const char* what() const noexcept override {
        static std::string full_message = "Player Error: " + std::string(ServerError::what());
        return full_message.c_str();
    }
};

class BuyGunError: public PlayerError {
public:
    BuyGunError(): PlayerError("not enough money to buy gun") {}
};
