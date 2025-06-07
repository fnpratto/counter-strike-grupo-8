#pragma once

#include <memory>
#include <vector>

#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/clock/clock.h"

class Weapon {
protected:
    TimePoint time_last_attack = TimePoint{};

public:
    // TODO: If the weapon is shooting, being able to attack depends
    //       on burst_frec
    bool can_attack(const float fire_rate, TimePoint now) {
        std::chrono::duration<float> secs_btw_attacks = now - time_last_attack;
        if (secs_btw_attacks.count() < (1.0f / fire_rate))
            return false;
        return true;
    }

    virtual std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir,
                                                              TimePoint now) = 0;

    virtual ~Weapon() = default;
};
