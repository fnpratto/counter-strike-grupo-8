#include "lobby_monitor.h"

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "game/game.h"

#include "errors.h"
#include "game_thread.h"


pipe_t LobbyMonitor::create_game(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);

    if (games.find(name) != games.end())
        throw GameExistsError();

    auto game = std::make_shared<GameThread>(name);
    game->start();
    games[name] = game;

    return game->join_game("player_name");  // TODO: take the player name from the message
}

pipe_t LobbyMonitor::join_game(const std::string& name) {
    std::lock_guard<std::mutex> lock(mtx);

    auto game = games[name];

    if (!game->is_alive())
        throw JoinGameError();

    return game->join_game("player_name");  // TODO: take the player name from the message
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
        if (!game->is_alive()) {
            game->join();
            games.erase(name);
        }
}

LobbyMonitor::~LobbyMonitor() {
    for (const auto& [name, game]: games) {
        game->stop();
        game->join();
    }
}
