#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <map>

#include "round_phase.h"
#include "game_config.h"
#include "shop.h"
#include "player.h"
#include "server/errors.h"
#include "common/models.h"
#include "common/message.h"

class Game {
private:
    std::string name;
    RoundPhase phase;
    const GameConfig& config;
    const Shop& shop;
    std::map<std::string, Player> players;
    int num_rounds = 0;
    unsigned int num_terrorists = 0;
    unsigned int num_counter_terrorists = 0;
    
    void handle_msg(Message msg, const std::string& player_name);
    
    void join(const std::string& player_name);
    Team choose_player_team();

    void select_player_team(const std::string& player_name, Team team);
    
    void start();
    void give_bomb_to_terrorists();
    
    void player_buy_weapon(const std::string& player_name, WeaponType weapon);

public:
    Game(const std::string& name, const Clock& clock, const GameConfig& config, const Shop& shop);

    // TODO: encapsulate Game state into class GameState.
    RoundPhaseType get_phase_type() const;
    int get_num_players() const;
    int get_num_rounds_played() const;
    Inventory get_player_inventory(const std::string& player_name) const;
    bool is_player_terrorist(const std::string& player_name) const;
    bool is_player_counter_terrorist(const std::string& player_name) const;
    bool is_full() const;
    bool team_is_full(Team team) const;
    bool is_started() const;
    
    void tick(Message msg, const std::string& player_name);
};
