#include "idle_status.h"

#include <memory>

#include "server/player/player.h"

#include "defusing_bomb_status.h"
#include "moving_status.h"
#include "planting_bomb_status.h"

void IdleStatus::handle_start_moving(Player& player, const Vector2D& velocity) {
    player.set_status(std::make_unique<MovingStatus>());
    player.start_moving(velocity);
}

void IdleStatus::handle_start_attacking(Player& player, TimePoint now) {
    player.start_attacking_with_equipped_weapon(now);
}

void IdleStatus::handle_switch_item(Player& player, ItemSlot slot) { player.equip_item(slot); }

void IdleStatus::handle_start_planting(Player& player, TimePoint now) {
    player.set_status(std::make_unique<PlantingBombStatus>());
    player.start_planting_bomb(now);
}

void IdleStatus::handle_start_defusing(Player& player, Bomb& bomb, TimePoint now) {
    player.set_status(std::make_unique<DefusingBombStatus>());
    player.start_defusing_bomb(bomb, now);
}
