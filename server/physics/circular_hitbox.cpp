#include "circular_hitbox.h"

#include "common/physics/physics_config.h"

CircularHitbox::CircularHitbox(const Vector2D& center, int radius): Circle(center, radius) {}

CircularHitbox::CircularHitbox(const Circle& circle): Circle(circle.center, circle.radius) {}

CircularHitbox CircularHitbox::player_hitbox(const Vector2D& center) {
    return CircularHitbox(center, PhysicsConfig::meter_size / 3);
}

Circle CircularHitbox::get_bounds() const { return Circle{center, radius}; }

bool CircularHitbox::collides_with_rectangle(const RectHitbox& rect) const {
    return rect.collides_with_circle(center, radius);
}

bool CircularHitbox::collides_with_circle(const CircularHitbox& circle) const {
    float distance = (center - circle.center).length();
    return distance <= (radius + circle.radius);
}

bool CircularHitbox::is_hit(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    Vector2D dir = ray_dir.normalized(PhysicsConfig::meter_size);
    Vector2D distance = center - ray_start;
    float orthogonal_distance = std::abs(distance.cross(dir)) / PhysicsConfig::meter_size;
    return orthogonal_distance <= radius;
}

bool CircularHitbox::is_in_same_cuadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    Vector2D dir = ray_dir.normalized(PhysicsConfig::meter_size);
    Vector2D distance = center - ray_start;
    float projected_length = distance.dot(dir) / PhysicsConfig::meter_size;
    return projected_length + radius > 0.0f;
}
