#include "planting_bomb_status.h"

#include <memory>

#include "server/player/player.h"

#include "idle_status.h"

void PlantingBombStatus::handle_stop_planting(Player& player, TimePoint now) {
    player.set_status(std::make_unique<IdleStatus>());
    player.stop_planting_bomb(now);
}
