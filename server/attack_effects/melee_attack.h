#pragma once

#include "attack_effect.h"

class MeleeAttack: public AttackEffect {
private:
    int radius;

public:
    MeleeAttack(int damage, int radius):
            // TODO: Should it receive a radius or
            AttackEffect(damage),
            radius(radius) {}

    int compute_damage(int distance) const override {
        (void)distance;
        return damage;
    }

    bool is_target_hit() const override {
        // TODO: Return true if target is in radius.
        return true;
    }
};
