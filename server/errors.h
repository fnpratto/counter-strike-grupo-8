#pragma once

#include <exception>
#include <string>

#include "common/errors.h"

class GameError: public Error {
public:
    explicit GameError(const std::string& message): Error(message) {}
    const char* what() const noexcept override {
        static std::string full_message = "Game Error: " + std::string(Error::what());
        return full_message.c_str();
    }
};

class GameExistsError: public GameError {
public:
    GameExistsError(): GameError("game already exists") {}
};

class JoinGameError: public GameError {
public:
    JoinGameError(): GameError("could not join game") {}
};

class StartGameError: public GameError {
public:
    StartGameError(): GameError("try to start an already started game") {}
};

class SelectTeamError: public GameError {
public:
    SelectTeamError(): GameError("error at select team") {}
};

class PlayerError: public Error {
public:
    explicit PlayerError(const std::string& message): Error(message) {}
    const char* what() const noexcept override {
        static std::string full_message = "Player Error: " + std::string(Error::what());
        return full_message.c_str();
    }
};

class MapError: public Error {
public:
    explicit MapError(const std::string& message): Error(message) {}
    const char* what() const noexcept override {
        static std::string full_message = "Map Error: " + std::string(Error::what());
        return full_message.c_str();
    }
};
