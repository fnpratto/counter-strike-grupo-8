#include "sdl_item.h"


SdlItem::SdlItem(const SdlWindow& window, const GameUpdate& game_state, const SdlCamera& camera):
        window(window),
        awp_t(std::string(GameConfig::Paths::AWP_ITEM_PATH), window),
        m3_t(std::string(GameConfig::Paths::M3_ITEM_PATH), window),
        ak_t(std::string(GameConfig::Paths::AK_ITEM_PATH), window),
        bomb_t(std::string(GameConfig::Paths::BOMB_ITEM_PATH), window),
        game_state(game_state),
        camera(camera) {}

void SdlItem::render() {
    if (game_state.get_bomb().has_value()) {
        const auto& bomb = game_state.get_bomb().value();
        Vector2D pos = bomb.hitbox.get_pos();
        Area origin(0, 0, 24, 24);
        Vector2D pos_cam = camera.get_screen_pos(pos);
        Area dest(pos_cam.get_x(), pos_cam.get_y(), 32, 32);
        if (camera.can_see(pos_cam))
            bomb_t.render(origin, dest);
    }
    for (const auto& [gun_type, gun_item]:  // cppcheck-suppress[unassignedVariable]
         game_state.get_dropped_guns()) {
        Vector2D pos = gun_item.get_pos();
        if (!camera.can_see(pos))
            return;
        Vector2D pos_cam = camera.get_screen_pos(pos);
        Area dest(pos_cam.get_x(), pos_cam.get_y(), 32, 32);
        Area origin(10, 0, 54, 32);
        if (gun_type == GunType::AWP) {
            awp_t.render(origin, dest);
        } else if (gun_type == GunType::M3) {
            m3_t.render(origin, dest);
        } else if (gun_type == GunType::AK47) {
            ak_t.render(origin, dest);
        }
    }
}
