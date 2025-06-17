#include "sdl_player.h"

#include <cmath>

#include <SDL2/SDL.h>


SdlPlayer::SdlPlayer(SdlWindow& w, const SdlCamera& cam, const GameUpdate& game_state_param,
                     const std::string& player_name_param):
        window(w),
        camera(cam),
        player_texture(CHARACTER_PATH, window),
        walk_animation(
                w, WALKING_ANIMATION,
                std::vector<SDL_Rect>(
                        {{0, 0, WIDTH, HEIGHT}, {32, 0, WIDTH, HEIGHT}, {64, 0, WIDTH, HEIGHT}})),
        game_state(game_state_param),
        playerName(player_name_param),
        weapon(window) {}

void SdlPlayer::render(const PlayerUpdate& state) {
    if (state.get_health() <= 0) {
        return;
    }
    if (state.get_velocity() == Vector2D(0, 0)) {
        walk_animation.reset();
    }

    auto position_from_cam = camera.get_screen_pos(state.get_pos());
    auto aim_direction = state.get_aim_direction();
    float angle;
    if (aim_direction != Vector2D(0, 0)) {
        // Calculate angle based on aim direction
        angle = std::atan2(aim_direction.get_y(), aim_direction.get_x()) * 180 / M_PI;
        angle += 90.0f;  // Adjust so that 0 degrees is right
    } else {
        angle = 0.0f;  // Default angle if no aim direction is provided
    }
    // Render feet
    walk_animation.render(position_from_cam.get_x(), position_from_cam.get_y(), angle);


    SDL_Rect clip{32, 32, WIDTH, HEIGHT};
    player_texture.render(position_from_cam.get_x(), position_from_cam.get_y(), &clip, angle,
                          nullptr, SDL_FLIP_NONE);

    Vector2D adjusted_position(state.get_pos().get_x() + 5, state.get_pos().get_y());
    Vector2D position_from_cam_weapon = camera.get_screen_pos(adjusted_position);
    // crear la posición final del arma
    Area dest(position_from_cam_weapon.get_x(), position_from_cam_weapon.get_y(), WIDTH, HEIGHT);
    ItemSlot item = game_state.get_players().at(playerName).get_equipped_item();
    weapon.render(item, dest, angle);
}
