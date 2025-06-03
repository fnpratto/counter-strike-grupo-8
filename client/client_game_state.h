#pragma once

#include <map>
#include <string>

#include "common/models.h"
#include "common/updates/game_update.h"
#include "common/utils/vector_2d.h"

struct PlayerState {
    int health = 100;
    Team team;
    Vector2D position;
    // add other fields you need like weapon, money, etc.
};

struct GameState {
    int num_rounds = 0;
    PhaseType phase;
    TimePoint time;
    std::map<std::string, PlayerState> players;

    void apply_update(const GameUpdate& update) {
        if (update.has_num_rounds_changed()) {
            num_rounds = update.get_num_rounds();
        }
        if (update.has_phase_changed()) {
            const auto& phase_update = update.get_phase();
            if (phase_update.has_phase_changed())
                phase = phase_update.get_phase();
            if (phase_update.has_time_changed())
                time = phase_update.get_time();
        }
        if (update.has_players_changed()) {
            for (const auto& [id, player_update]: update.get_players()) {
                PlayerState& player = players[id];  // Creates if not exists
                if (player_update.has_health_changed())
                    player.health = player_update.get_health();
                if (player_update.has_team_changed())
                    player.team = player_update.get_team();
                if (player_update.has_pos_changed())
                    player.position = player_update.get_pos();
                // Handle other fields...
            }
        }
    }
};
