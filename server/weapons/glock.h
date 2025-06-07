#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "common/models.h"
#include "server/attack_effects/gun_attack.h"

#include "gun.h"
#include "weapons_config.h"

class Glock: public Gun {
public:
    Glock():
            Gun(GunType::Glock, GlockConfig::bullets_per_mag, GlockConfig::init_mag_ammo,
                GlockConfig::init_reserve_ammo) {}

    std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir, TimePoint now) override {
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!has_ammo() || !can_attack(GlockConfig::attack_rate, now))
            return effects;

        int damage =
                get_random_damage(GlockBulletConfig::min_damage, GlockBulletConfig::max_damage);

        auto effect = std::make_unique<GunAttack>(damage, dir, GlockBulletConfig::precision);
        effects.push_back(std::move(effect));

        decrease_mag_ammo();
        time_last_attack = now;

        return effects;
    }
};
