#pragma once

#include "common/utils/circle.h"
#include "common/utils/vector_2d.h"

#include "rect_hitbox.h"

class CircularHitbox: public Circle {
    CircularHitbox(const Vector2D& center, int radius);

public:
    explicit CircularHitbox(const Circle& circle);

    static CircularHitbox player_hitbox(const Vector2D& center);

    Circle get_bounds() const;

    bool collides_with_rectangle(const RectHitbox& rect) const;
    bool collides_with_circle(const CircularHitbox& circle) const;

    bool is_hit(const Vector2D& ray_start, const Vector2D& ray_dir) const;
    bool is_in_same_cuadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const;
};
