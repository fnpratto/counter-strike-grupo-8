#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/message.h"
#include "common/updates/game_update.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/map/map.h"
#include "server/player/player.h"
#include "server/player_message.h"
#include "server/states/game_state.h"

#include "game_config.h"
#include "game_phase.h"
#include "physics_system.h"
#include "shop.h"

class Game: public Logic<GameState, GameUpdate> {
private:
    std::string name;

    Shop shop;
    PhysicsSystem physics_system;

    std::vector<PlayerMessage> output_messages;

public:
    Game(const std::string& name, std::shared_ptr<Clock>&& game_clock, Map&& map);

    bool is_full() const;

    void join_player(const std::string& player_name);

    std::vector<PlayerMessage> tick(const std::vector<PlayerMessage>& msgs);

    std::string get_name() const;
    int get_player_count() const;
    PhaseType get_phase();

    ~Game();

private:
    void give_bomb_to_random_tt();
    void move_player_to_spawn(const std::string& player_name);

    void handle_msg(const Message& msg, const std::string& player_name);

    template <typename T>
    void handle(const std::string& player_name, const T& msg);

    void advance_round_logic();
    void advance_players_movement();
    void perform_attacks();
};
