#include "sdl_world_item.h"

SdlWorldItem::SdlWorldItem(const SdlWindow& window, const GameUpdate& game_state,
                           const SdlCamera& camera):
        window(window),
        awp_t(std::string(GameConfig::Paths::AWP_ITEM_PATH), window),
        m3_t(std::string(GameConfig::Paths::M3_ITEM_PATH), window),
        ak_t(std::string(GameConfig::Paths::AK_ITEM_PATH), window),
        bomb_t(std::string(GameConfig::Paths::BOMB_ITEM_PATH), window),
        bomb_plated(std::string(GameConfig::Paths::BOMB_PLANTED_ITEM_PATH), window),
        game_state(game_state),
        camera(camera) {}

void SdlWorldItem::render() {
    render_bomb();
    render_dropped_guns();
}

void SdlWorldItem::render_bomb() {
    if (!game_state.get_bomb().has_value())
        return;

    const auto& bomb = game_state.get_bomb().value();
    Vector2D pos = bomb.hitbox.get_pos();
    Vector2D pos_cam = camera.get_screen_pos(pos);
    Area origin(0, 0, 24, 24);
    Area dest(pos_cam.get_x(), pos_cam.get_y(), 32, 32);

    if (!camera.can_see(pos_cam))
        return;

    if (game_state.get_phase().get_type() == PhaseType::BombPlanted) {
        bomb_plated.render(origin, dest);
    } else {
        bomb_t.render(origin, dest);
    }
}

void SdlWorldItem::render_dropped_guns() {
    for (const auto& [gun_type, gun_item]: game_state.get_dropped_guns()) {
        Vector2D pos = gun_item.get_pos();
        float angle = gun_item.get_rotation_deg();
        if (!camera.can_see(pos))
            continue;

        Vector2D pos_cam = camera.get_screen_pos(pos);
        Area dest(pos_cam.get_x(), pos_cam.get_y(), 32, 32);
        Area origin(10, 0, 54, 32);
        switch (gun_type) {
            case GunType::AWP:
                awp_t.render(origin, dest, angle);
                break;
            case GunType::M3:
                m3_t.render(origin, dest, angle);
                break;
            case GunType::AK47:
                ak_t.render(origin, dest, angle);
                break;
            default:
                break;
        }
    }
}
