#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "common/models.h"
#include "server/attack_effects/gun_attack.h"

#include "gun.h"
#include "weapons_config.h"

class Awp: public Gun {
public:
    Awp():
            Gun(GunType::AWP, AwpConfig::bullets_per_mag, AwpConfig::init_mag_ammo,
                AwpConfig::init_reserve_ammo) {}

    std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir, TimePoint now) override {
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!has_ammo() || !can_attack(AwpConfig::attack_rate, now))
            return effects;

        auto effect = std::make_unique<GunAttack>(AwpBulletConfig::damage, dir,
                                                  AwpBulletConfig::precision);
        effects.push_back(std::move(effect));

        decrease_mag_ammo();
        time_last_attack = now;

        return effects;
    }
};
