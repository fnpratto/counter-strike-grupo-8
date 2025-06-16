#pragma once

#include "common/utils/rect_bounds.h"
#include "common/utils/vector_2d.h"

class RectHitbox: public RectBounds {
    RectHitbox(const Vector2D& pos, int width, int height, float rotation);

public:
    explicit RectHitbox(const RectBounds& bounds);

    static RectHitbox gun_hitbox(const Vector2D& pos);

    static RectHitbox bomb_hitbox(const Vector2D& pos);

    static RectHitbox tile_hitbox(const Vector2D& pos);

    RectBounds get_bounds() const;

    bool collides_with_circle(const Vector2D& circle_pos, float radius) const;

    bool is_in_same_cuadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const;
    bool is_hit(const Vector2D& ray_start, const Vector2D& ray_dir) const;
};
