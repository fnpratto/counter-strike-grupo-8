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
    int num_rounds;
    unsigned int num_terrorists;
    unsigned int num_counter_terrorists;

    bool is_invalid_player_name(const std::string& player_name);

public:
    Game(const std::string& name, const Clock& clock, const GameConfig& config, const Shop& shop);

    RoundPhaseType get_phase_type() const;
    int get_num_players() const;
    int get_num_rounds_played() const;
    Team get_player_team(const std::string& player_name) const;
    Inventory get_player_inventory(const std::string& player_name) const;

    bool is_full() const;
    bool team_is_full(Team team) const;
    bool is_started() const;
    
    void join(const std::string& player_name, Team team);
    void start();
    void player_buy_weapon(const std::string& player_name, WeaponType weapon);

    void tick();
};
