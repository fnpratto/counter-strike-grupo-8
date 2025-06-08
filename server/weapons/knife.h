#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "server/attack_effects/melee_attack.h"
#include "server/logic.h"
#include "server/states/knife_state.h"

#include "weapon.h"
#include "weapons_config.h"

class Knife: public Logic<KnifeState, KnifeUpdate>, public Weapon {
public:
    Knife(): Logic<KnifeState, KnifeUpdate>(KnifeState()) {}

    void start_attacking() { state.set_is_attacking(true); }

    std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir, TimePoint now) override {
        (void)dir;
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!can_attack(KnifeConfig::attack_rate, now))
            return effects;
        effects.push_back(std::make_unique<MeleeAttack>(
                KnifeConfig::damage, KnifeConfig::attack_radius, KnifeConfig::cone_max_angle));
        time_last_attack = now;
        return effects;
    }
};
