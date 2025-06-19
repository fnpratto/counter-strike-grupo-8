#include "moving_status.h"

#include "server/player/player.h"

#include "idle_status.h"
#include "planting_bomb_status.h"

void MovingStatus::handle_start_moving(Player& player, const Vector2D& velocity) {
    player.start_moving(velocity);
}

void MovingStatus::handle_stop_moving(Player& player) {
    player.set_status(IdleStatus());
    player.stop_moving();
}

void MovingStatus::handle_start_attacking(Player& player) {
    player.start_attacking_with_equipped_weapon();
}

void MovingStatus::handle_switch_item(Player& player, ItemSlot slot) { player.equip_item(slot); }

void MovingStatus::handle_start_planting(Player& player, TimePoint now) {
    if (!player.get_inventory().get_bomb().has_value())
        return;
    player.set_status(PlantingBombStatus());
    player.start_planting_bomb(now);
}
