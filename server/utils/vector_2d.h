#pragma once

#include <cmath>

class Vector2D{
private:
    float x;
    float y;

public:
    Vector2D() : x(0), y(0) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    float get_x() const { return x; }
    float get_y() const { return y; }

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator*(float n) const {
        return Vector2D(x * n, y * n);
    }

    Vector2D normalize() const {
        float norm = std::sqrt(x * x + y * y);
        if (norm == 0) 
            return Vector2D(0, 0);
        return Vector2D(x / norm, y / norm);
    }

    ~Vector2D() {}
};
