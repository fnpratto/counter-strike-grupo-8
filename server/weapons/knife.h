#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "server/attack_effects/attack_effect.h"
#include "server/attack_effects/melee_attack.h"
#include "server/logic.h"

#include "weapon.h"
#include "weapons_config.h"

class Knife: public Weapon {
public:
    Knife() {}

    std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir, TimePoint now) override {
        (void)dir;       // TODO: MeleeAttack does not actually need this
        int radius = 2;  // TODO: This should be received instead
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!can_attack(KnifeConfig::attack_rate, now))
            return effects;
        effects.push_back(std::make_unique<MeleeAttack>(KnifeConfig::damage, radius));
        time_last_attack = now;
        return effects;
    }
};
