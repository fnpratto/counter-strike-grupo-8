#include "sdl_bullet.h"

#include <cmath>
#include <iostream>

#include "common/utils/vector_2d.h"

// SdlBullet::SdlBullet(SdlWindow& w):
//         window(w), laser(, window), hit_effect("../assets/gfx/map/hitmark.xcf", window, 30, 30)
//         {}

SdlBullet::SdlBullet(const SdlWindow& w, const SdlCamera& camera, Vector2D origin,
                     const HitResponse& hit_response):
        camera(camera),
        origin(origin),
        hit_response(hit_response),
        bullet_animation(w, "../assets/gfx/gunshot.xcf", 6, 15, std::chrono::milliseconds(500)) {
    Vector2D direction = hit_response.get_hit_pos() - origin;
    angle = std::atan2(direction.get_y(), direction.get_x()) * 180.0f / M_PI;
}

void SdlBullet::render() {
    // Good enough heuristic
    if (!camera.can_see(origin) && !camera.can_see(hit_response.get_hit_pos()))
        return;

    auto screen_origin = camera.get_screen_pos(origin);
    auto screen_hit_pos = camera.get_screen_pos(hit_response.get_hit_pos());

    bullet_animation.render(screen_origin, screen_hit_pos, angle);
}

bool SdlBullet::is_finished() const { return bullet_animation.is_finished(); }

// void SdlBullet::render_hit_effect(const Vector2D& pos, float angle) {
//     SDL_Rect hit_clip{0, 0, 15, 15};
//     hit_effect.render(pos.get_x(), pos.get_y(), &hit_clip, angle, nullptr, SDL_FLIP_NONE);
//     std::cout << "Rendering hit effect at position: (" << pos.get_x() << ", " << pos.get_y() <<
//     ")"
//               << std::endl;
// }
