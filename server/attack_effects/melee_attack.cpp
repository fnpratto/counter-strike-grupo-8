#include "melee_attack.h"

#include "server/player/player.h"

MeleeAttack::MeleeAttack(Player& player_origin, int damage, const Vector2D& dir, int max_range):
        AttackEffect(player_origin, damage, dir, max_range) {}

int MeleeAttack::compute_damage(int distance) const {
    (void)distance;
    return damage;
}

bool MeleeAttack::is_target_hit(int distance) const {
    (void)distance;
    return true;
}
