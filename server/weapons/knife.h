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

    std::vector<std::unique_ptr<AttackEffect>> attack(Player& player_origin, const Vector2D& dir,
                                                      TimePoint now) override {
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!can_attack(KnifeConfig::attack_rate, now))
            return effects;
        int damage = get_random_damage(KnifeConfig::min_damage, KnifeConfig::max_damage);
        effects.push_back(
                std::make_unique<MeleeAttack>(player_origin, damage, dir, KnifeConfig::max_range));
        time_last_attack = now;
        return effects;
    }
};
