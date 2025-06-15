#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>

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

    bool is_tts_win_condition() const;

public:
    GameState(std::shared_ptr<Clock>&& game_clock, int max_players);

    bool player_is_in_game(const std::string& player_name) const;
    bool all_players_ready() const;
    bool game_is_full() const;
    bool team_is_full(const Team& team) const;

    int get_num_rounds() const;
    int get_num_tts() const;
    int get_num_cts() const;
    GamePhase& get_phase();
    const std::map<std::string, std::unique_ptr<Player>>& get_players() const;
    const std::unique_ptr<Player>& get_player(const std::string& player_name) const;

    void advance_round();

    void swap_players_teams();

    void add_player(const std::string& player_name, std::unique_ptr<Player> player);

    Team get_winning_team() const;

    GameUpdate get_updates() const override;

    GameUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]

    void clear_updates() override;
};
