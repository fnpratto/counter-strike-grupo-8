#pragma once

#include <string>

#include "shop.h"
#include "common/message.h"
#include "game_state.h"

class Game {
private:
    std::string name;
    GameState state;
    const Shop& shop;
    
    void handle_msg(Message msg, const std::string& player_name);

    void chk_valid_player_name_or_fail(const std::string& player_name);

public:
    Game(const std::string& name, const Clock& clock, const GameConfig& config, const Shop& shop);

    GameConfig get_config();
    PhaseType get_phase();

    void tick(Message msg, const std::string& player_name);

    ~Game();
};
