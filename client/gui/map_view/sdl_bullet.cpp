#include "sdl_bullet.h"

SdlBullet::SdlBullet(SdlWindow& w):
        window(w),
        laser("../assets/gfx/map/laser.xcf", window, 200, 5),
        hit_effect("../assets/gfx/map/blood.xcf", window, 120, 120) {}  // Initialize texture

void SdlBullet::render(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir,
                       bool is_hit) {

    float angle;
    if (get_origin != Vector2D(0, 0)) {
        // Calculate angle based on aim get_origin
        angle = std::atan2(get_hit_dir.get_y(), get_hit_dir.get_x()) * 180 / M_PI;
        angle += 180.0f;  // Adjust so that 0 degrees is right
    } else {
        angle = 90.0f;  // Default angle if no aim direction is provided
    }

    // Render bullet at origin
    SDL_Rect clip{0, 0, 170, 180};  // Clip size for bullet texture
    laser.render(get_origin.get_x(), get_origin.get_y(), &clip, angle, nullptr, SDL_FLIP_NONE);
    std::cout << "Rendering bullet at origin: (" << get_origin.get_x() << ", " << get_origin.get_y()
              << ")" << std::endl;
    // If the bullet hits something, render the hit position
    if (is_hit) {
        SDL_Rect hit_clip{0, 0, 120, 120};  // Clip size for hit effect
        hit_effect.render(get_hit_pos.get_x(), get_hit_pos.get_y(), &hit_clip, angle, nullptr,
                          SDL_FLIP_NONE);
        std::cout << "Rendering hit effect at position: (" << get_hit_pos.get_x() << ", "
                  << get_hit_pos.get_y() << ")" << std::endl;
    }
}
