#pragma once

#include <vector>

#include "common/utils/vector_2d.h"
#include "effects/attack_effect.h"
#include "server/clock/clock.h"

class Weapon {
protected:
    TimePoint time_last_attack;

    bool can_attack(const float attack_rate, TimePoint now);

    int get_random_damage(const int min_dam, const int max_dam);

public:
    Weapon() = default;

    virtual std::vector<AttackEffect> attack(const Vector2D& origin, const Vector2D& dir,
                                             TimePoint now) = 0;

    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;

    Weapon(Weapon&&) = default;
    Weapon& operator=(Weapon&&) = default;

    virtual ~Weapon() = default;
};
