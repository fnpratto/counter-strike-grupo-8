#pragma once

#include "server/player/player.h"

#include "player_status.h"

class PlantingBombStatus: public PlayerStatus {
public:
    void handle_stop_planting(Player& player, TimePoint now) override;
};
