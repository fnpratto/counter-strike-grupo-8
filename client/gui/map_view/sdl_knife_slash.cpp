#include "sdl_knife_slash.h"

#include <cmath>
#include <iostream>

#include "common/utils/vector_2d.h"

SdlKnifeSlash::SdlKnifeSlash(const SdlWindow& w, const SdlCamera& camera, Vector2D origin,
                             const HitResponse& hit_response):
        camera(camera),
        origin(origin),
        hit_response(hit_response),
        slash_animation(w, "../assets/gfx/knife_slash.xcf",
                        std::vector<SDL_Rect>({{0, 0, WIDTH, HEIGHT},
                                               {32, 0, WIDTH, HEIGHT},
                                               {64, 0, WIDTH, HEIGHT},
                                               {96, 0, WIDTH, HEIGHT}}),
                        std::chrono::milliseconds(200), false, 1.3f) {}

void SdlKnifeSlash::render() {
    // Only render if the player origin is visible
    if (!camera.can_see(origin))
        return;

    auto screen_origin = camera.get_screen_pos(origin);

    // Calculate angle to face the hit position
    Vector2D direction = hit_response.get_hit_pos() - origin;
    float angle = (std::atan2(direction.get_y(), direction.get_x()) * 180.0f / M_PI) - 45.0f;

    slash_animation.render(screen_origin.get_x() + x_offset, screen_origin.get_y() + y_offset,
                           angle);
}

bool SdlKnifeSlash::is_finished() const { return slash_animation.is_finished(); }
