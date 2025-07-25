#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/map/map.h"
#include "common/message.h"
#include "common/updates/game_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/physics/physics_system.h"
#include "server/player/player.h"
#include "server/player_message.h"
#include "server/shop/shop.h"
#include "server/states/game_state.h"

#include "game_phase.h"

class Game: public Logic<GameState, GameUpdate> {
private:
    std::string name;
    TimePoint last_tick;

    Shop shop;
    PhysicsSystem physics_system;

    std::vector<PlayerMessage> output_messages;

public:
    Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map,
         GameConfig&& config);

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    bool is_full() const;

    void join_player(const std::string& player_name);

    std::vector<PlayerMessage> tick(const std::vector<PlayerMessage>& msgs);

    const std::string& get_name() const;
    std::string get_map_name() const;
    int get_player_count() const;
    PhaseType get_phase();

private:
    float get_tick_duration();
    void give_bomb_to_random_tt(Bomb&& bomb);
    void prepare_new_round();
    void move_player_to_spawn(const std::unique_ptr<Player>& player);

    void broadcast(const Message& msg);
    void send_msg(const std::string& player_name, const Message& msg);

    void handle_msg(const Message& msg, const std::string& player_name);

    template <typename T>
    void handle(const std::string& player_name, const T& msg);

    void advance_round_logic();
    void advance_players_movement();
    void advance_bomb_logic();
    void perform_attacks();
    bool apply_attack_effect(const std::unique_ptr<Player>& attacker, const Effect& effect,
                             const Target& target);
};
