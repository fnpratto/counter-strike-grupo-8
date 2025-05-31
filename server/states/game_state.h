#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "common/updates/game_update.h"
#include "server/clock/clock.h"
#include "server/game/game_phase.h"
#include "server/player/player.h"

#include "state.h"

class GameState: public State<GameUpdate> {
    GamePhase phase;
    int num_rounds = 0;
    std::map<std::string, std::unique_ptr<Player>> players;

public:
    explicit GameState(std::unique_ptr<Clock>&& game_clock);

    // Delete copy constructor and copy assignment operator
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    // Enable move constructor and move assignment operator
    GameState(GameState&&) = default;
    GameState& operator=(GameState&&) = default;

    void advance_round() { set_num_rounds(num_rounds + 1); }
    void set_num_rounds(int rounds) {
        if (num_rounds == rounds)
            return;
        num_rounds = rounds;
        updates.set_num_rounds(rounds);
    }

    int get_num_rounds() const { return num_rounds; }

    int get_num_tts() const {
        int num_tts = 0;
        for (const auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
            if (player->is_tt())
                num_tts++;

        return num_tts;
    }

    int get_num_cts() const {
        int num_cts = 0;
        for (const auto& [_, player]: players)  // cppcheck-suppress[unusedVariable]
            if (player->is_ct())
                num_cts++;

        return num_cts;
    }

    GamePhase& get_phase() { return phase; }

    void add_player(const std::string& player_name, std::unique_ptr<Player> player) {
        if (players.find(player_name) != players.end())
            throw std::runtime_error("Player already exists");
        players[player_name] = std::move(player);
        updates.add_players_change(player_name, players.at(player_name)->get_updates());
    }
    const std::map<std::string, std::unique_ptr<Player>>& get_players() const { return players; }

    GameUpdate get_full_update() const override {
        GameUpdate update;
        update.set_phase(phase.get_full_update());
        update.set_num_rounds(num_rounds);
        for (const auto& [name, player]: players) {
            update.add_players_change(name, player->get_full_update());
        }
        return update;
    }
};
