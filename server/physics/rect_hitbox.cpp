#include "rect_hitbox.h"

#include <algorithm>

#include "common/physics/physics_config.h"
#include "common/utils/random_float_generator.h"

RectHitbox::RectHitbox(const Vector2D& pos, int width, int height, float rotation = 0.0f):
        RectBounds(pos, width, height) {
    this->rotate(rotation);
}

RectHitbox::RectHitbox(const RectBounds& bounds):
        RectHitbox(bounds.get_pos(), bounds.get_width(), bounds.get_height(),
                   bounds.get_rotation()) {}

RectHitbox RectHitbox::gun_hitbox(const Vector2D& pos) {
    float rotation = RandomFloatGenerator(-180.0f, 180.0f).generate();
    return RectHitbox(pos, PhysicsConfig::meter_size / 2, PhysicsConfig::meter_size / 4, rotation);
}

RectHitbox RectHitbox::bomb_hitbox(const Vector2D& pos) {
    float rotation = RandomFloatGenerator(-180.0f, 180.0f).generate();
    return RectHitbox(pos, PhysicsConfig::meter_size / 2, PhysicsConfig::meter_size / 2, rotation);
}

RectHitbox RectHitbox::tile_hitbox(const Vector2D& pos) {
    return RectHitbox(pos, PhysicsConfig::meter_size, PhysicsConfig::meter_size);
}

RectBounds RectHitbox::get_bounds() const {
    return RectBounds(get_pos(), get_width(), get_height());
}

bool RectHitbox::collides_with_circle(const Vector2D& circle_pos, float radius) const {
    float minX = get_pos().get_x();
    float maxX = get_pos().get_x() + get_width();
    float minY = get_pos().get_y();
    float maxY = get_pos().get_y() + get_height();

    float closestX = std::max(minX, std::min(static_cast<float>(circle_pos.get_x()), maxX));
    float closestY = std::max(minY, std::min(static_cast<float>(circle_pos.get_y()), maxY));

    float distance =
            Vector2D(circle_pos.get_x() - closestX, circle_pos.get_y() - closestY).length();

    if (distance <= radius)
        return true;

    return false;
}

bool RectHitbox::is_in_same_cuadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    Vector2D distance = get_pos() - ray_start;
    return distance.dot(ray_dir) > 0;
}

bool RectHitbox::is_hit(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    float minX = get_pos().get_x();
    float maxX = get_pos().get_x() + get_width();
    float minY = get_pos().get_y();
    float maxY = get_pos().get_y() + get_height();

    float aim_x = ray_dir.get_x();
    float aim_y = ray_dir.get_y();

    if (aim_x == 0)
        aim_x = 1e-8;
    if (aim_y == 0)
        aim_y = 1e-8;

    float t1 = (minX - ray_start.get_x()) / aim_x;
    float t2 = (maxX - ray_start.get_x()) / aim_x;
    float t3 = (minY - ray_start.get_y()) / aim_y;
    float t4 = (maxY - ray_start.get_y()) / aim_y;

    float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
    float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

    if (tmax < 0)
        return false;

    if (tmin > tmax)
        return false;

    return true;
}
