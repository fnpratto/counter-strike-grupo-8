#pragma once

#include "vector_2d.h"

struct Circle {
    Vector2D center;
    int radius;

    Circle(const Vector2D& center, int radius): center(center), radius(radius) {}
};
