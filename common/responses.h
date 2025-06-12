#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "common/models.h"
#include "common/utils/vector_2d.h"
#include "scoreboard/scoreboard_entry.h"

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
 * @class TriedToJoinFullTeamErrorResponse
 * @brief Response sent when a player attempts to join a team that is already full.
 */
class TriedToJoinFullTeamErrorResponse {};

/**
 * @class CannotBuyErrorResponse
 * @brief Response sent when a player attempts to make a purchase but is not allowed to.
 */
class CannotBuyErrorResponse {};
