#pragma once

#include <memory>
#include <vector>

#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/clock/clock.h"

class Player;

class Weapon {
protected:
    TimePoint time_last_attack;

public:
    Weapon() {}

    // TODO: If the weapon is attacking, being able to attack depends
    //       on burst_frec
    bool can_attack(const float attack_rate, TimePoint now) {
        std::chrono::duration<float> time_since_last_attack = now - time_last_attack;
        float min_time_between_attacks = 1.0f / attack_rate;
        return time_since_last_attack.count() >= min_time_between_attacks;
    }

    virtual std::vector<std::unique_ptr<AttackEffect>> attack(Player& player_origin,
                                                              const Vector2D& dir,
                                                              TimePoint now) = 0;

    virtual ~Weapon() = default;
};
