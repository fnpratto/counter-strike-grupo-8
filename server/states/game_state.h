#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/game/world_item.h"
#include "common/models.h"
#include "common/updates/game_update.h"
#include "server/clock/clock.h"
#include "server/game/game_phase.h"
#include "server/player/player.h"

#include "state.h"

class GameState: public State<GameUpdate> {
    GamePhase phase;
    int num_rounds = 0;
    int max_players;
    std::map<std::string, std::unique_ptr<Player>> players;

    std::vector<WorldItem<std::unique_ptr<Gun>>> dropped_guns;
    std::optional<WorldItem<Bomb>> bomb;

    bool is_tts_win_condition() const;

public:
    GameState(std::shared_ptr<Clock>&& game_clock, int max_players,
              const std::vector<std::pair<GunType, Vector2D>>& guns);

    bool player_is_in_game(const std::string& player_name) const;
    bool all_players_ready() const;
    bool game_is_full() const;
    bool team_is_full(const Team& team) const;
    bool is_round_end_condition() const;
    std::map<std::string, ScoreboardEntry> get_scoreboard() const;

    int get_num_rounds() const;
    int get_num_tts() const;
    int get_num_cts() const;
    GamePhase& get_phase();
    const std::map<std::string, std::unique_ptr<Player>>& get_players() const;
    const std::unique_ptr<Player>& get_player(const std::string& player_name) const;
    std::vector<WorldItem<std::unique_ptr<Gun>>>& get_dropped_guns();
    std::optional<WorldItem<Bomb>>& get_bomb();

    void advance_round();

    void swap_players_teams();

    void add_player(const std::string& player_name, Team team, const Vector2D& pos);
    void add_dropped_gun(std::unique_ptr<Gun>&& gun, const Vector2D& pos);
    std::unique_ptr<Gun> remove_dropped_gun_at_pos(const Vector2D& pos);
    void add_bomb(Bomb&& bomb, const Vector2D& pos);
    Bomb remove_bomb();

    std::vector<CharacterType> get_characters_tt() const;
    std::vector<CharacterType> get_characters_ct() const;
    CharacterType get_default_character(Team team) const;
    Team get_winning_team() const;
    void give_rewards_to_players(Team winning_team);

    GameUpdate get_updates() const override;

    GameUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]

    void clear_updates() override;
};
