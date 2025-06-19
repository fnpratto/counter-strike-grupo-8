#include "defusing_bomb_status.h"

#include "server/player/player.h"

#include "idle_status.h"

void DefusingBombStatus::handle_stop_defusing(Player& player, Bomb& bomb, TimePoint now) {
    player.set_status(IdleStatus());
    player.stop_defusing_bomb(bomb, now);
}
