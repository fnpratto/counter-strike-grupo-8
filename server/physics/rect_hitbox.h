#pragma once

#include "common/utils/rectangle.h"
#include "common/utils/vector_2d.h"

class RectHitbox: public Rectangle {
private:
    RectHitbox(const Vector2D& pos, int width, int height, int rotation);

    Vector2D to_aabb_space(const Vector2D& global_point, int rotation_deg) const;

    Vector2D closest_pos_in_aabb(const Vector2D& pos) const;

public:
    explicit RectHitbox(const Rectangle& bounds);

    static RectHitbox gun_hitbox(const Vector2D& pos);

    static RectHitbox bomb_hitbox(const Vector2D& pos);

    static RectHitbox tile_hitbox(const Vector2D& pos);

    Rectangle get_bounds() const;

    bool collides_with_circle(const Vector2D& circle_pos, int radius) const;

    bool is_in_same_quadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const;
    bool is_hit(const Vector2D& ray_start, const Vector2D& ray_dir) const;
};
