#pragma once

#include <cmath>

class Vector2D {
private:
    int x;
    int y;

public:
    Vector2D(int x, int y): x(x), y(y) {}
    Vector2D(const Vector2D& v1, const Vector2D& v2):
            x(v2.get_x() - v1.get_x()), y(v2.get_y() - v1.get_y()) {}

    void set_x(int new_x) { x = new_x; }
    void set_y(int new_y) { y = new_y; }

    int get_x() const { return x; }
    int get_y() const { return y; }

    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }

    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }

    Vector2D operator+(float n) const { return Vector2D(x + n, y + n); }

    Vector2D operator-(float n) const { return Vector2D(x - n, y - n); }

    Vector2D operator*(float n) const { return Vector2D(x * n, y * n); }

    Vector2D operator/(float n) const { return Vector2D(x / n, y / n); }

    bool operator==(const Vector2D& other) const { return x == other.x && y == other.y; }

    bool operator!=(const Vector2D& other) const { return x != other.x || y != other.y; }

    float cross(const Vector2D& other) const { return x * other.y - y * other.x; }

    float dot(const Vector2D& other) const { return x * other.x + y * other.y; }

    float length() const { return std::sqrt(x * x + y * y); }

    Vector2D normalized(int n) const {
        float norm = std::sqrt(x * x + y * y);
        if (norm == 0)
            return Vector2D(0, 0);
        return Vector2D((x / norm) * n, (y / norm) * n);
    }

    Vector2D rotated(float angle_deg) const {
        float angle_rad = angle_deg * M_PI / 180.0f;
        float base_angle = std::atan2(y, x);
        float rotated_angle = base_angle + angle_rad;
        float len = this->length();
        return Vector2D(std::cos(rotated_angle) * len, std::sin(rotated_angle) * len);
    }

    ~Vector2D() {}
};
