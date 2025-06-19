#pragma once

#include "server/clock/clock.h"
#include "server/player/player.h"

#include "player_status.h"

class DefusingBombStatus: public PlayerStatus {
public:
    void handle_stop_defusing(Player& player, Bomb& bomb, TimePoint now) override;
};
