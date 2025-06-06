#include "camera.h"

Camera::Camera(int screen_width, int screen_height, int world_width, int world_height):
        screen_width(screen_width),
        screen_height(screen_height),
        world_width(world_width),
        world_height(world_height),
        offset(0, 0) {}


void Camera::center(const Vector2D& target_pos) {
    int x = target_pos.get_x() - screen_width / 2;
    int y = target_pos.get_y() - screen_height / 2;

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x > world_width - screen_width)
        x = world_width - screen_width;
    if (y > world_height - screen_height)
        y = world_height - screen_height;

    offset = Vector2D(x, y);
}

Vector2D Camera::get_offset() const { return offset; }
Camera::~Camera() = default;
