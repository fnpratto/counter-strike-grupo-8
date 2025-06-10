#pragma once

#include "attack_effect.h"

class MeleeAttack: public AttackEffect {
private:
    int radius;
    float cone_max_angle;

public:
    MeleeAttack(int damage, int radius, float cone_max_angle):
            AttackEffect(damage), radius(radius), cone_max_angle(cone_max_angle) {}

    int compute_damage(int distance) const override {
        (void)distance;
        return damage;
    }

    bool is_target_hit() const override {
        // TODO: Return true if target is in cone limited by radius and cone_max_angle.
        return true;
    }
};
