#pragma once

#include <string>
#include <vector>

#include "common/models.h"

struct GameInfo {
    std::string name;
    int players_count;
    PhaseType phase;

    GameInfo(const std::string& game_name, int count, PhaseType game_phase):
            name(game_name), players_count(count), phase(game_phase) {}
};

/**
 * @class ListGamesResponse
 * @brief Response containing a list of the existing games' names.
 */
class ListGamesResponse {
    std::vector<GameInfo> games_info;

public:
    explicit ListGamesResponse(const std::vector<GameInfo>& games): games_info(games) {}

    std::vector<GameInfo> get_games_info() const { return games_info; }
};
