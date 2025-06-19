#include "defusing_bomb_status.h"

#include <memory>

#include "server/player/player.h"

#include "idle_status.h"

void DefusingBombStatus::handle_stop_defusing(Player& player, Bomb& bomb) {
    player.set_status(std::make_unique<IdleStatus>());
    player.stop_defusing_bomb(bomb);
}
