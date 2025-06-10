#include "sdl_camera.h"

SdlCamera::SdlCamera(int screen_width, int screen_height):
        screen_width(screen_width), screen_height(screen_height), position(0, 0) {}


void SdlCamera::center(const Vector2D& target_pos) { position = target_pos; }

Vector2D SdlCamera::get_offset(Vector2D pos) const {
    auto delta = pos - position;
    // Convert from game units to pixels
    delta = delta * u2px_rate;

    // Adjust for screen center
    static Vector2D screen_center(screen_width / 2, screen_height / 2);
    Vector2D position = screen_center + delta;

    return position;
}

bool SdlCamera::can_see(const Vector2D& target_pos) const {
    Vector2D offset = get_offset(target_pos);
    return offset.get_x() >= 0 && offset.get_x() < screen_width && offset.get_y() >= 0 &&
           offset.get_y() < screen_height;
}
