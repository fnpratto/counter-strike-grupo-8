#include "rect_hitbox.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "common/physics/physics_config.h"
#include "common/utils/random_float_generator.h"

RectHitbox::RectHitbox(const Vector2D& pos, int width, int height, int rotation = 0):
        Rectangle(pos, width, height) {
    this->rotate(rotation);
}

RectHitbox::RectHitbox(const Rectangle& bounds):
        RectHitbox(bounds.get_pos(), bounds.get_width(), bounds.get_height(),
                   bounds.get_rotation_deg()) {}

RectHitbox RectHitbox::gun_hitbox(const Vector2D& pos) {
    int rotation = RandomFloatGenerator(-180, 180).generate();
    return RectHitbox(pos, PhysicsConfig::meter_size / 2, PhysicsConfig::meter_size / 4, rotation);
}

RectHitbox RectHitbox::bomb_hitbox(const Vector2D& pos) {
    int rotation = RandomFloatGenerator(-180, 180).generate();
    return RectHitbox(pos, PhysicsConfig::meter_size / 2, PhysicsConfig::meter_size / 2, rotation);
}

RectHitbox RectHitbox::tile_hitbox(const Vector2D& pos) {
    return RectHitbox(pos, PhysicsConfig::meter_size, PhysicsConfig::meter_size);
}

Rectangle RectHitbox::get_bounds() const { return Rectangle(pos, width, height); }

Vector2D RectHitbox::to_aabb_space(const Vector2D& global_pos, int rotation_deg) const {
    Vector2D relative = global_pos - pos;
    return relative.rotated(rotation_deg);
}

Vector2D RectHitbox::closest_pos_in_aabb(const Vector2D& another_pos) const {
    int another_x = another_pos.get_x();
    int another_y = another_pos.get_y();

    int closest_x = std::max(0, std::min(another_x, width));
    int closest_y = std::max(0, std::min(another_y, height));

    return Vector2D(closest_x, closest_y);
}

bool RectHitbox::collides_with_circle(const Vector2D& circle_pos, int radius) const {
    Vector2D circle_relative_pos = to_aabb_space(circle_pos, rotation_deg);
    Vector2D closest = closest_pos_in_aabb(circle_relative_pos);
    float distance = (circle_relative_pos - closest).length();
    return distance <= radius;
}

bool RectHitbox::is_in_same_quadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    Vector2D top_left = Vector2D(pos.get_x(), pos.get_y());
    Vector2D top_right = Vector2D(pos.get_x() + width, pos.get_y());
    Vector2D bottom_left = Vector2D(pos.get_x(), pos.get_y() + height);
    Vector2D bottom_right = Vector2D(pos.get_x() + width, pos.get_y() + height);

    std::vector<Vector2D> corners = {
            top_left.rotated(rotation_deg), top_right.rotated(rotation_deg),
            bottom_right.rotated(rotation_deg), bottom_left.rotated(rotation_deg)};

    return std::any_of(corners.begin(), corners.end(), [&](const Vector2D& corner) {
        Vector2D dist_to_corner = corner - ray_start;
        return dist_to_corner.dot(ray_dir) >= 0;
    });
}

bool RectHitbox::is_hit(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    Vector2D relative_ray_start = (ray_start - pos).rotated(rotation_deg);
    Vector2D relative_ray_dir = ray_dir.rotated(rotation_deg);

    float dir_x = (relative_ray_dir.get_x() == 0) ? 1e-8f : relative_ray_dir.get_x();
    float dir_y = (relative_ray_dir.get_y() == 0) ? 1e-8f : relative_ray_dir.get_y();

    float t1 = -relative_ray_start.get_x() / dir_x;
    float t2 = (width - relative_ray_start.get_x()) / dir_x;
    float t3 = -relative_ray_start.get_y() / dir_y;
    float t4 = (height - relative_ray_start.get_y()) / dir_y;

    float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
    float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

    return (tmax >= 0 && tmin <= tmax);
}
