#include "sdl_camera.h"

SdlCamera::SdlCamera(int screen_width, int screen_height):
        screen_width(screen_width), screen_height(screen_height), position(0, 0) {}


void SdlCamera::center(const Vector2D& target_pos) { position = target_pos; }

/**
 * @brief Converts a game position to a screen position.
 *
 * @param pos
 * @return Vector2D
 */
Vector2D SdlCamera::get_screen_pos(Vector2D pos) const {
    auto delta = pos - position;
    // Convert from game units to pixels
    delta = delta * u2px_rate;

    // Adjust for screen center
    static Vector2D screen_center(screen_width / 2, screen_height / 2);
    Vector2D screen_position = screen_center + delta;

    return screen_position;
}

template <typename T>
bool SdlCamera::can_see(const T& obj) const {
    Vector2D offset = get_screen_pos(obj.get_pos());
    return can_see(offset);
}

bool SdlCamera::can_see(const Tile& obj) const {
    Vector2D offset = get_screen_pos(obj.pos);
    return can_see(offset);
}

bool SdlCamera::can_see(const Vector2D& pos) const {
    Vector2D offset = get_screen_pos(pos);
    return offset.get_x() >= 0 && offset.get_x() < screen_width && offset.get_y() >= 0 &&
           offset.get_y() < screen_height;
}
