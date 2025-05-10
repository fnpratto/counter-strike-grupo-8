#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "errors.h"
#include "game_monitor.h"

class LobbyMonitor {
private:
    std::mutex mtx;
    std::map<std::string, std::shared_ptr<GameMonitor>> games;

public:
    LobbyMonitor() = default;

    // Create a new game with the given name
    // throws GameExistsError if the game already exists
    std::shared_ptr<GameMonitor> create_game(const std::string& name);

    // Join an existing game
    // throws GameNotFoundError if the game doesn't exist or is full
    std::shared_ptr<GameMonitor> join_game(const std::string& name);

    // Get a list of available games
    std::vector<std::string> get_games_names();

    void reap();

    LobbyMonitor(const LobbyMonitor&) = delete;
    LobbyMonitor& operator=(const LobbyMonitor&) = delete;
};
