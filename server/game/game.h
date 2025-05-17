#pragma once

#include <string>

#include "shop.h"
#include "common/message.h"
#include "game_state.h"

class Game {
private:
    std::string name;
    GameState state;
    
    void handle_msg(Message msg, const std::string& player_name);

    void chk_valid_player_name_or_fail(const std::string& player_name);

public:
    Game(const std::string& name, const Clock& clock, const Shop& shop);

    void tick(Message msg, const std::string& player_name);

    ~Game();
};
