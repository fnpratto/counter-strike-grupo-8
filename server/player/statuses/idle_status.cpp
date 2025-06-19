#include "idle_status.h"

#include "server/player/player.h"

#include "defusing_bomb_status.h"
#include "moving_status.h"
#include "planting_bomb_status.h"

void IdleStatus::handle_start_moving(Player& player, const Vector2D& velocity) {
    player.set_status(MovingStatus());
    player.start_moving(velocity);
}

void IdleStatus::handle_start_attacking(Player& player) {
    player.start_attacking_with_equipped_weapon();
}

void IdleStatus::handle_switch_item(Player& player, ItemSlot slot) { player.equip_item(slot); }

void IdleStatus::handle_start_planting(Player& player, TimePoint now) {
    player.set_status(PlantingBombStatus());
    player.start_planting_bomb(now);
}

void IdleStatus::handle_start_defusing(Player& player, Bomb& bomb, TimePoint now) {
    player.set_status(DefusingBombStatus());
    player.start_defusing_bomb(bomb, now);
}
