#include "camera.h"
const int tile_size = 32;  // todo change

Camera::Camera(int screen_width, int screen_height):
        screen_width(screen_width), screen_height(screen_height), offset(0, 0) {}


void Camera::center(const Vector2D& target_pos) {
    int x = target_pos.get_x() - screen_width / 2;
    int y = target_pos.get_y() - screen_height / 2;

    offset = Vector2D(x, y);
}

Vector2D Camera::get_offset() const { return offset; }
Camera::~Camera() = default;
