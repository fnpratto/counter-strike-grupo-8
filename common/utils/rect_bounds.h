#pragma once

#include "vector_2d.h"

class RectBounds {
protected:
    Vector2D pos;
    int width;
    int height;
    float rotation_deg = 0.0f;

public:
    RectBounds(const Vector2D& pos, int width, int height):
            pos(pos), width(width), height(height) {}

    Vector2D get_pos() const { return pos; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    float get_rotation() const { return rotation_deg; }

    void rotate(float angle) {
        rotation_deg += angle;
        if (rotation_deg >= 360.0f)
            rotation_deg -= 360.0f;
        else if (rotation_deg < 0.0f)
            rotation_deg += 360.0f;
    }
};
