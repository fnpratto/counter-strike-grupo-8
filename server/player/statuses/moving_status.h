#pragma once

#include "server/clock/clock.h"
#include "server/player/player.h"

#include "player_status.h"

class MovingStatus: public PlayerStatus {
public:
    void handle_start_moving(Player& player, const Vector2D& velocity) override;
    void handle_stop_moving(Player& player) override;
    void handle_start_attacking(Player& player) override;
    void handle_switch_item(Player& player, ItemSlot slot) override;
    void handle_start_planting(Player& player, TimePoint now) override;
    void handle_start_defusing(Player& player, Bomb& bomb, TimePoint now) override;
};
