#pragma once

#include "vector_2d.h"

class Rectangle {
protected:
    Vector2D pos;  // Position of the top-left corner
    int width;
    int height;
    float rotation_deg = 0.0f;  // Rotation in degrees (counterclockwise), around pos

public:
    Rectangle(const Vector2D& pos, int width, int height): pos(pos), width(width), height(height) {}

    Vector2D get_pos() const { return pos; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    float get_rotation_deg() const { return rotation_deg; }

    Vector2D get_center() const {
        return Vector2D(pos.get_x() + width / 2.0f, pos.get_y() + height / 2.0f);
    }

    void rotate(float angle) {
        rotation_deg += angle;
        if (rotation_deg >= 360.0f)
            rotation_deg -= 360.0f;
        else if (rotation_deg < 0.0f)
            rotation_deg += 360.0f;
    }
};
