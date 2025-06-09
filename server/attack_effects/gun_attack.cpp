#include "gun_attack.h"

#include "server/player/player.h"

GunAttack::GunAttack(Player& player_origin, int damage, const Vector2D& dir, int max_range,
                     float precision, float falloff):
        AttackEffect(player_origin, damage, dir, max_range),
        precision(precision),
        falloff(falloff) {}

int GunAttack::compute_damage(int distance) const {
    if (distance > max_range)
        return 0;
    float falloff_factor = 1.0f - (static_cast<float>(distance) / max_range) * falloff;
    return static_cast<int>(damage * falloff_factor);
}

bool GunAttack::is_target_hit() const {
    return RandomFloatGenerator(0.0f, 1.0f).generate() <= precision;
}
