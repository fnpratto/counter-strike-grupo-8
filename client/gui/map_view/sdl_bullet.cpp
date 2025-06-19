#include "sdl_bullet.h"

#include <cmath>
#include <iostream>

SdlBullet::SdlBullet(SdlWindow& w):
        window(w),
        laser("../assets/gfx/map/laser.xcf", window),
        hit_effect("../assets/gfx/map/hitmark.xcf", window, 30, 30) {}

void SdlBullet::render(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir, bool is_hit,
                       bool is_knife) {
    (void)get_hit_dir;

    float dx = get_hit_pos.get_x() - get_origin.get_x();
    float dy = get_hit_pos.get_y() - get_origin.get_y();
    float length = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx) * 180 / M_PI;

    std::cout << "Angle in degrees: " << angle << std::endl;

    if (is_hit) {
        if (!is_knife) {
            render_laser(get_origin, length, angle);
        }
        render_hit_effect(get_hit_pos, angle);
    } else {
        if (!is_knife) {
            render_laser(get_origin, length, angle);
        }
    }
}


void SdlBullet::render_laser(const Vector2D& origin, float length, float angle) {
    SDL_Rect clip{0, 0, 200, 5};
    laser.render(origin.get_x(), origin.get_y(), length, 5, &clip, angle, nullptr, SDL_FLIP_NONE);
    std::cout << "Rendering bullet at origin: (" << origin.get_x() << ", " << origin.get_y() << ")"
              << std::endl;
}

void SdlBullet::render_hit_effect(const Vector2D& pos, float angle) {
    SDL_Rect hit_clip{0, 0, 15, 15};
    hit_effect.render(pos.get_x(), pos.get_y(), &hit_clip, angle, nullptr, SDL_FLIP_NONE);
    std::cout << "Rendering hit effect at position: (" << pos.get_x() << ", " << pos.get_y() << ")"
              << std::endl;
}
