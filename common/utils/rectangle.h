#pragma once

#include "vector_2d.h"

struct Rectangle {
    Vector2D pos;
    int width;
    int height;
    float rotation_deg = 0.0f;

    Rectangle(const Vector2D& pos, int width, int height): pos(pos), width(width), height(height) {}

    void rotate(float angle) {
        rotation_deg += angle;
        if (rotation_deg >= 360.0f)
            rotation_deg -= 360.0f;
        else if (rotation_deg < 0.0f)
            rotation_deg += 360.0f;
    }
};
