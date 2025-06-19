#include "sdl_bullet.h"

SdlBullet::SdlBullet(SdlWindow& w):
        window(w),
        laser("../assets/gfx/map/laser.xcf", window),
        hit_effect("../assets/gfx/map/blood.xcf", window, 10, 10) {}  // Initialize texture

void SdlBullet::render(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir,
                       bool is_hit) {
    std::cout << "Rendering bullet with origin: (" << get_origin.get_x() << ", "
              << get_origin.get_y() << "), hit position: (" << get_hit_pos.get_x() << ", "
              << get_hit_pos.get_y() << "), hit direction: (" << get_hit_dir.get_x() << ", "
              << get_hit_dir.get_y() << "), is hit: " << (is_hit ? "true" : "false") << std::endl;
    float angle;
    if (get_origin != Vector2D(0, 0)) {
        // Calculate angle based on aim get_origin
        angle = std::atan2(get_hit_dir.get_y(), get_hit_dir.get_x()) * 180 / M_PI;
        angle += 90.0f;  // Adjust so that 0 degrees is right
        std::cout << "Calculated angle: " << angle << std::endl;
    } else {
        angle = 0.0f;  // Default angle if no aim direction is provided
        std::cout << "No origin provided, using default angle: " << angle << std::endl;
    }

    if (is_hit) {

        float dx = get_hit_pos.get_x() - get_origin.get_x();
        float dy = get_hit_pos.get_y() - get_origin.get_y();
        float length = std::sqrt(dx * dx + dy * dy);

        angle = std::atan2(dy, dx) * 180 / M_PI + 90.0f;

        SDL_Rect clip{0, 0, 200, 5};
        laser.render(get_origin.get_x(), get_origin.get_y(), length, 5, &clip, angle, nullptr,
                     SDL_FLIP_NONE);

        std::cout << "Rendering bullet at origin: (" << get_origin.get_x() << ", "
                  << get_origin.get_y() << ")" << std::endl;

        SDL_Rect hit_clip{0, 0, 60, 60};  // Clip size for hit effect
        hit_effect.render(get_hit_pos.get_x(), get_hit_pos.get_y(), &hit_clip, angle, nullptr,
                          SDL_FLIP_NONE);
        std::cout << "Rendering hit effect at position: (" << get_hit_pos.get_x() << ", "
                  << get_hit_pos.get_y() << ")" << std::endl;
    } else {
        SDL_Rect clip{0, 0, 200, 5};
        laser.render(get_origin.get_x(), get_origin.get_y(), 400, 5, &clip, angle, nullptr,
                     SDL_FLIP_NONE);  // If not hit position fix laser to be 400

        std::cout << "Rendering bullet at origin: (" << get_origin.get_x() << ", "
                  << get_origin.get_y() << ")" << std::endl;
    }
}
