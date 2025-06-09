#pragma once

#include "attack_effect.h"

class GunAttack: public AttackEffect {
protected:
    float precision;

public:
    GunAttack(Player& player_origin, int damage, const Vector2D& dir, float precision,
              int max_range);

    int compute_damage(int distance) const override;

    bool is_target_hit() const override;
};
