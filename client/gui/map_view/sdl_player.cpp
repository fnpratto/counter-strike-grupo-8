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


    const float RADIUS = 64.0f;  // distancia deseada

    float angle_rad = angle * M_PI / 180.0f;  // convertir de grados a radianes
    float tan_alpha = std::tan(angle_rad);

    // calcular x e y a partir de la fórmula
    float denominator = 1 + tan_alpha * tan_alpha;
    float x_offset = std::sqrt((RADIUS * RADIUS) / denominator);
    float y_offset = tan_alpha * x_offset;

    // mantener el signo correcto según la dirección
    if (std::cos(angle_rad) < 0)
        x_offset = -x_offset;
    if (std::sin(angle_rad) < 0)
        y_offset = -y_offset;

    // crear la posición final del arma
    Area dest(position_from_cam.get_x() + static_cast<int>(x_offset),
              position_from_cam.get_y() + static_cast<int>(y_offset), WIDTH, HEIGHT);
    ItemSlot item = game_state.get_players().at(playerName).get_equipped_item();
    weapon.render(item, dest, angle);
}
