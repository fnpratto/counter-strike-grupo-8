#include "sdl_bullet.h"

SdlBullet::SdlBullet(SdlWindow& w):
        window(w),
        laser("../assets/gfx/guns/laser.xcf", window, 170, 180),
        hit_effect("../assets/gfx/player/blood.xcf", window, 120, 120) {}  // Initialize texture

void SdlBullet::render(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir,
                       bool is_hit) {
    float angle = std::atan2(get_hit_dir.get_y(), get_hit_dir.get_x()) * 180 / M_PI;

    // Render bullet at origin
    SDL_Rect clip{0, 0, 170, 180};  // Clip size for bullet texture
    laser.render(get_origin.get_x(), get_origin.get_y(), &clip, angle, nullptr, SDL_FLIP_NONE);

    // If the bullet hits something, render the hit position
    if (is_hit) {
        SDL_Rect hit_clip{0, 0, 120, 120};  // Clip size for hit effect
        laser.render(get_hit_pos.get_x(), get_hit_pos.get_y(), &hit_clip, angle, nullptr,
                     SDL_FLIP_NONE);
    }
}
