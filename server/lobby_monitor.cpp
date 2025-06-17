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


pipe_t LobbyMonitor::create_game(const std::string& game_name, const int& map_id,
                                 const std::string& player_name) {
    std::lock_guard<std::mutex> lock(mtx);

    if (games.find(game_name) != games.end())
        throw GameExistsError();

    auto game = std::make_shared<GameThread>(game_name, map_store.get_map(map_id));
    game->start();
    games[game_name] = game;

    return game->join_game(player_name);
}

pipe_t LobbyMonitor::join_game(const std::string& game_name, const std::string& player_name) {
    std::lock_guard<std::mutex> lock(mtx);

    auto game = games[game_name];

    if (!game || !game->is_alive())
        throw JoinGameError();

    return game->join_game(player_name);
}

std::vector<GameInfo> LobbyMonitor::get_games_info() {
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<GameInfo> games_info;
    for (const auto& [_, game]: games) {  // cppcheck-suppress[unusedVariable]
        if (!game)
            continue;

        games_info.push_back(game->get_game_info());
    }

    return games_info;
}

std::map<std::string, int> LobbyMonitor::get_maps_info() const { return map_store.get_maps_info(); }

void LobbyMonitor::reap() {
    std::lock_guard<std::mutex> lock(mtx);

    for (const auto& [name, game]: games)
        if (game && !game->is_alive()) {
            game->join();
            games.erase(name);
        }
}

LobbyMonitor::~LobbyMonitor() {
    for (const auto& [_, game]: games) {  // cppcheck-suppress[unusedVariable]
        game->stop();
        game->join();
    }
}
