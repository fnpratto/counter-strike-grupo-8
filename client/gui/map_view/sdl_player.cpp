#include "sdl_player.h"

#include <cmath>

#include <SDL2/SDL.h>

SdlPlayer::SdlPlayer(SdlWindow& w, const SdlCamera& camera):
        window(w),
        camera(camera),
        walk_animation(w, CHARACTER_PATH,
                       std::vector<SDL_Rect>({{0, 0, WIDTH, HEIGHT},
                                              {32, 0, WIDTH, HEIGHT},
                                              {0, 32, WIDTH, HEIGHT},
                                              {32, 32, WIDTH, HEIGHT},
                                              {0, 64, WIDTH, HEIGHT},
                                              {32, 64, WIDTH, HEIGHT}})) {}

void SdlPlayer::render(const PlayerUpdate& state) {
    if (state.get_velocity() == Vector2D(0, 0)) {
        walk_animation.reset();
    }

    auto position_from_cam = camera.get_screen_pos(state.get_pos());
    std::cout << "Player position from camera: (" << position_from_cam.get_x() << ", "
              << position_from_cam.get_y() << ")" << std::endl;
    auto aim_direction = state.get_aim_direction();
    float angle;
    if (aim_direction != Vector2D(0, 0)) {
        // Calculate angle based on aim direction
        angle = std::atan2(aim_direction.get_y(), aim_direction.get_x()) * 180 / M_PI;
        angle += 90.0f;  // Adjust so that 0 degrees is right
    } else {
        angle = 0.0f;  // Default angle if no aim direction is provided
    }

    // Render the player texture
    SDL_Rect clip{32, 32, WIDTH, HEIGHT};
    SdlTexture texture(CHARACTER_PATH, window, WIDTH, HEIGHT);
    texture.render(position_from_cam.get_x(), position_from_cam.get_y(), &clip, angle, nullptr,
                   SDL_FLIP_NONE);

    // Render the walking animation (if applicable)
    // walk_animation.render(position_from_cam.get_x(), position_from_cam.get_y(), angle);
}
