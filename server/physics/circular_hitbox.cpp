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

bool CircularHitbox::is_in_same_quadrant(const Vector2D& ray_start, const Vector2D& ray_dir) const {
    Vector2D dir = ray_dir.normalized(PhysicsConfig::meter_size);
    Vector2D distance = center - ray_start;
    float projected_length = distance.dot(dir) / PhysicsConfig::meter_size;
    return projected_length + radius > 0.0f;
}

std::optional<Vector2D> CircularHitbox::get_hit_pos(const Vector2D& ray_start,
                                                    const Vector2D& ray_dir) const {
    Vector2D dir = ray_dir.normalized(PhysicsConfig::meter_size);
    Vector2D distance = ray_start - center;

    float a = dir.dot(dir);
    float b = 2 * distance.dot(dir);
    float c = distance.dot(distance) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return std::optional<Vector2D>();

    float sqrt_discriminant = std::sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);

    float t = (t1 >= 0) ? t1 : ((t2 >= 0) ? t2 : -1);

    if (t >= 0)
        return ray_start + dir * t;

    return std::optional<Vector2D>();
}
