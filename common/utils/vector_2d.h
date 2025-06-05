#pragma once

#include <cmath>

#include "random_float_generator.h"

class Vector2D {
    static constexpr int meter_size = 32;  // 1 meter = 32 game world units

private:
    int x;
    int y;

public:
    Vector2D(int x, int y): x(x), y(y) {}
    Vector2D(const Vector2D& v1, const Vector2D& v2):
            x(v2.get_x() - v1.get_x()), y(v2.get_y() - v1.get_y()) {}

    int get_x() const { return x; }
    int get_y() const { return y; }

    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }

    Vector2D operator*(float n) const { return Vector2D(x * n, y * n); }

    bool operator==(const Vector2D& other) const { return x == other.x && y == other.y; }

    bool operator!=(const Vector2D& other) const { return x != other.x || y != other.y; }

    // Normalize the vector to 1 meter_size
    Vector2D normalized() const {
        float norm = std::sqrt(x * x + y * y);
        if (norm == 0)
            return Vector2D(0, 0);
        return Vector2D((x / norm) * meter_size, (y / norm) * meter_size);
    }

    // TODO esto no va acá
    void vary_dir_in_cone(float cone_max_angle_deg) {
        float base_angle = std::atan2(y, x);

        float cone_max_angle_rad = cone_max_angle_deg * M_PI / 180.0f;
        RandomFloatGenerator rfg(-cone_max_angle_rad, cone_max_angle_rad);
        float angle_offset = rfg.generate();

        float varied_angle = base_angle + angle_offset;

        x = std::cos(varied_angle);
        y = std::sin(varied_angle);
    }

    ~Vector2D() {}
};
