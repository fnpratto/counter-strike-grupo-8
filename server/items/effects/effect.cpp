#include "effect.h"

#include "common/utils/random_float_generator.h"
#include "server/player/player.h"

Effect::Effect(const Vector2D& origin, int damage, int max_range, float precision, float falloff):
        origin(origin),
        damage(damage),
        max_range(max_range),
        precision(precision),
        falloff(falloff) {}

Vector2D Effect::get_origin() const { return origin; }

int Effect::get_max_range() const { return max_range; }

bool Effect::apply(const std::unique_ptr<Player>& player) const {
    int distance = (player->get_hitbox().center - origin).length();
    if (!is_target_hit(distance))
        return false;
    int final_damage = compute_damage(distance);
    player->take_damage(final_damage);
    return true;
}

bool Effect::is_target_hit(int distance) const {
    float adjusted_precision = apply_falloff(precision, distance);
    return RandomFloatGenerator(0.0f, 1.0f).generate() <= adjusted_precision;
}

int Effect::compute_damage(int distance) const { return apply_falloff(damage, distance); }

float Effect::apply_falloff(float value, int distance) const {
    if (distance > max_range)
        return 0;
    float falloff_factor = 1.0f - (static_cast<float>(distance) / max_range) * falloff;
    return static_cast<float>(value * falloff_factor);
}
