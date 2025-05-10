#include "lobby_monitor.h"

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "errors.h"
#include "game.h"

std::shared_ptr<GameMonitor> LobbyMonitor::create_game(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);

    if (games.find(name) != games.end())
        throw GameExistsError();

    auto game = std::make_shared<GameMonitor>(name);
    games[name] = game;

    return game;
}

std::shared_ptr<GameMonitor> LobbyMonitor::join_game(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = games.find(name);
    if (it == games.end())
        throw JoinGameError();

    auto game = it->second;

    if (game->is_full())
        throw JoinGameError();

    // game->join_player2(); // Join the game

    return game;
}

std::vector<std::string> LobbyMonitor::get_games_names() {
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> games_names;
    for (const auto& [name, _]: games) {
        games_names.push_back(name);
    }

    return games_names;
}

void LobbyMonitor::reap() {
    std::lock_guard<std::mutex> lock(mtx);

    for (const auto& [name, game]: games)
        if (game->is_finished())
            games.erase(name);
}
