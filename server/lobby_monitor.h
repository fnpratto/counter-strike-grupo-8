#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "errors.h"
#include "game_thread.h"
#include "pipe.h"


class LobbyMonitor {
private:
    std::mutex mtx;

    // Using shared_ptr is not strictly necessary here,
    // but it allows for easier management of a GameThread
    // because unique_ptr cannot be referenced
    std::map<std::string, std::shared_ptr<GameThread>> games;

public:
    LobbyMonitor() = default;

    // @brief Create a new game with the given name
    // @throws GameExistsError if the game already exists
    pipe_t create_game(const std::string& name);

    // @brief Join an existing game
    // @throws GameNotFoundError if the game doesn't exist or is full
    // @note The mutex lock is necessary in case the game finishes and is reaped while we're joining
    pipe_t join_game(const std::string& name);

    // Get a list of available games
    std::vector<std::string> get_games_names();

    void reap();

    LobbyMonitor(const LobbyMonitor&) = delete;
    LobbyMonitor& operator=(const LobbyMonitor&) = delete;
};
