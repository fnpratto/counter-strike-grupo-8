#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "client/gui/window_elements/sdl_animation.h"
#include "common/updates/game_update.h"

#include "sdl_camera.h"

class SdlPlayer {
    SdlWindow& window;
    const SdlCamera& camera;

    static constexpr const char* CHARACTER_PATH = "../assets/gfx/player/ct2.xcf";
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 32;

    SdlAnimation walk_animation;

public:
    explicit SdlPlayer(SdlWindow& w, const SdlCamera& camera):
            window(w),
            camera(camera),
            walk_animation(w, CHARACTER_PATH,
                           // TODO This could be changed with a strip of sprites instead of this
                           // mosaic and simply iterated over x
                           std::vector<SDL_Rect>({{0, 0, WIDTH, HEIGHT},
                                                  {32, 0, WIDTH, HEIGHT},
                                                  {0, 32, WIDTH, HEIGHT},
                                                  {32, 32, WIDTH, HEIGHT},
                                                  {0, 64, WIDTH, HEIGHT},
                                                  {32, 64, WIDTH, HEIGHT}})) {}

    void render(const PlayerUpdate& state) {
        if (state.get_velocity() == Vector2D(0, 0)) {
            walk_animation.reset();
        }

        auto position_from_cam = camera.get_screen_pos(state.get_pos());
        auto aim_direction = state.get_aim_direction();
        float angle;
        if (aim_direction != Vector2D(0, 0)) {
            // Calculate angle based on aim direction
            angle = std::atan2(aim_direction.get_y(), aim_direction.get_x()) * 180 / M_PI;
            std::cout << "Aim direction: " << aim_direction.get_x() << ", " << aim_direction.get_y()
                      << " -> angle: " << angle << std::endl;
            angle = angle + 90.0f;  // Adjust so that 0 degrees is right
        } else {
            // If no aim direction, just render facing right
            // (or any default direction, here we assume 0 degrees is right)
            angle = 0.0f;  // Default angle if no aim direction is provided
        }

        // TODO Replace this with the animation
        SDL_Rect clip{32, 32, WIDTH, HEIGHT};
        SdlTexture texture(CHARACTER_PATH, window, WIDTH, HEIGHT);
        texture.render(position_from_cam.get_x(), position_from_cam.get_y(), &clip, angle, nullptr,
                       SDL_FLIP_NONE);
        // walk_animation.render(position_from_cam.get_x(), position_from_cam.get_y(), angle);
    }
};
