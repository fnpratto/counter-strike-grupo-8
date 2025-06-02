#pragma once

#include <string>
#include <vector>

/**
 * @class ListGamesResponse
 * @brief Response containing a list of the existing games' names.
 */
class ListGamesResponse {
    std::vector<std::string> game_names;

public:
    explicit ListGamesResponse(const std::vector<std::string>& names): game_names(names) {}
    std::vector<std::string> get_game_names() const { return game_names; }
};
