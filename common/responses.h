#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "game/scoreboard_entry.h"

struct GameInfo {
    std::string name;
    std::string map_name;
    int players_count;
    PhaseType phase;

    GameInfo(const std::string& game_name, const std::string& map_name, int count,
             PhaseType game_phase):
            name(game_name), map_name(map_name), players_count(count), phase(game_phase) {}
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

/**
 * @class ListMapsResponse
 * @brief Response containing a list of the existing maps' names and IDs.
 */
class ListMapsResponse {
    std::vector<std::string> map_names;

public:
    explicit ListMapsResponse(const std::vector<std::string>& maps): map_names(maps) {}

    std::vector<std::string> get_map_names() const { return map_names; }
};

/**
 * @class CharactersResponse
 * @brief Response containing the characters information.
 */
class CharactersResponse {
    std::vector<CharacterType> characters;

public:
    explicit CharactersResponse(std::vector<CharacterType> chars): characters(std::move(chars)) {}

    std::vector<CharacterType> get_characters() const { return characters; }
};

/**
 * @class ShopPricesResponse
 * @brief Response containing the prices of guns and ammo in the shop.
 */
class ShopPricesResponse {
    std::map<GunType, int> gun_prices;
    std::map<GunType, int> ammo_prices;

public:
    ShopPricesResponse(std::map<GunType, int> gun_prices, std::map<GunType, int> ammo_prices):
            gun_prices(std::move(gun_prices)), ammo_prices(std::move(ammo_prices)) {}

    std::map<GunType, int> get_gun_prices() const { return gun_prices; }
    std::map<GunType, int> get_ammo_prices() const { return ammo_prices; }
};

/**
 * @class HitResponse
 * @brief Response containing the origin and hit position and direction of a hit.
 */
class HitResponse {
    Vector2D origin;
    Vector2D hit_pos;
    Vector2D hit_dir;
    bool hit;

public:
    HitResponse(const Vector2D& origin, const Vector2D& hit_pos, const Vector2D& dir, bool hit):
            origin(origin), hit_pos(hit_pos), hit_dir(dir), hit(hit) {}

    Vector2D get_origin() const { return origin; }
    Vector2D get_hit_pos() const { return hit_pos; }
    Vector2D get_hit_dir() const { return hit_dir; }
    bool is_hit() const { return hit; }
};

/**
 * @class ScoreboardResponse
 * @brief Response containing the scoreboard information.
 */
class ScoreboardResponse {
    std::map<std::string, ScoreboardEntry> scoreboard;

public:
    explicit ScoreboardResponse(std::map<std::string, ScoreboardEntry>&& scoreboard):
            scoreboard(std::move(scoreboard)) {}

    std::map<std::string, ScoreboardEntry> get_scoreboard() const { return scoreboard; }
};

/**
 * @class ErrorResponse
 * @brief Sent when a command results in an error.
 */
class ErrorResponse {};

/**
 * @class RoundEndResponse
 * @brief Response sent when a round ends.
 */
class RoundEndResponse {
    Team winning_team;

public:
    explicit RoundEndResponse(Team winning_team): winning_team(winning_team) {}

    Team get_winning_team() const { return winning_team; }
};

/**
 * @class BombExplodedResponse
 * @brief Response sent when a bomb explodes.
 */
class BombExplodedResponse {
    Vector2D explosion_center;
    int explosion_radius;

public:
    explicit BombExplodedResponse(const Vector2D& center, int radius):
            explosion_center(center), explosion_radius(radius) {}

    Vector2D get_explosion_center() const { return explosion_center; }
    int get_explosion_radius() const { return explosion_radius; }
};

/**
 * @class SwapTeamsResponse
 * @brief Response sent when players swap teams.
 */
class SwapTeamsResponse {};
