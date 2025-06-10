#pragma once

#include "attack_effect.h"

class MeleeAttack: public AttackEffect {
public:
    MeleeAttack(Player& player_origin, int damage, const Vector2D& dir, int radius);

    int compute_damage(int distance) const override;

    bool is_target_hit(int distance) const override;
};
