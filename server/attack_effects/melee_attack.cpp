#include "melee_attack.h"

#include "server/player/player.h"

MeleeAttack::MeleeAttack(Player& player_origin, int damage, const Vector2D& dir, int radius):
        AttackEffect(player_origin, damage, dir), radius(radius) {}

int MeleeAttack::compute_damage(int distance) const {
    (void)distance;
    return damage;
}

bool MeleeAttack::is_target_hit(const Vector2D& target_pos) const {
    return (target_pos - player_origin.get_pos()).length() <= radius;
}
