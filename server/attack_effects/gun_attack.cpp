#include "gun_attack.h"

#include "server/player/player.h"

GunAttack::GunAttack(Player& player_origin, int damage, const Vector2D& dir, float precision,
                     int max_range):
        AttackEffect(player_origin, damage, dir, max_range), precision(precision) {}

int GunAttack::compute_damage(int distance) const {
    // TODO: Implement falloff for GunAttack
    (void)distance;
    return damage;
}

bool GunAttack::is_target_hit() const {
    return RandomFloatGenerator(0.0f, 1.0f).generate() <= precision;
}
