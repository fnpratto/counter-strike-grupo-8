#include "sdl_bullet.h"

SdlBullet::SdlBullet(SdlWindow& w):
        window(w),
        laser("../assets/gfx/map/laser.xcf", window),
        hit_effect("../assets/gfx/map/hitmark.xcf", window, 15, 15) {}  // Initialize texture

void SdlBullet::render(Vector2D get_origin, Vector2D get_hit_pos, Vector2D get_hit_dir, bool is_hit,
                       bool is_knife) {
    (void)get_hit_dir;  // Unused parameter, can be removed if not needed
    float dx = get_hit_pos.get_x() - get_origin.get_x();
    float dy = get_hit_pos.get_y() - get_origin.get_y();
    float angle = std::atan2(dy, dx) * 180 / M_PI;
    std::cout << "Angle in radians: "
              << ", Angle in degrees: " << angle << std::endl;

    if (is_hit) {

        if (!is_knife) {
            float length = std::sqrt(dx * dx + dy * dy);
            SDL_Rect clip{0, 0, 200, 5};
            laser.render(get_origin.get_x(), get_origin.get_y(), length, 5, &clip, angle, nullptr,
                         SDL_FLIP_NONE);

            std::cout << "Rendering bullet at origin: (" << get_origin.get_x() << ", "
                      << get_origin.get_y() << ")" << std::endl;
        }


        SDL_Rect hit_clip{0, 0, 15, 15};  // Clip size for hit effect
        hit_effect.render(get_hit_pos.get_x(), get_hit_pos.get_y(), &hit_clip, angle, nullptr,
                          SDL_FLIP_NONE);
        std::cout << "Rendering hit effect at position: (" << get_hit_pos.get_x() << ", "
                  << get_hit_pos.get_y() << ")" << std::endl;
    } else {
        if (!is_knife) {
            SDL_Rect clip{0, 0, 200, 5};
            laser.render(get_origin.get_x(), get_origin.get_y(), 200, 5, &clip, angle, nullptr,
                         SDL_FLIP_NONE);  // If not hit position fix laser to be 400

            std::cout << "Rendering bullet at origin: (" << get_origin.get_x() << ", "
                      << get_origin.get_y() << ")" << std::endl;
        }
    }
}
