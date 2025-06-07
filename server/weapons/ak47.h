#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "common/models.h"
#include "server/attack_effects/gun_attack.h"

#include "gun.h"
#include "weapons_config.h"

class Ak47: public Gun {
public:
    Ak47():
            Gun(GunType::AK47, Ak47Config::bullets_per_mag, Ak47Config::init_mag_ammo,
                Ak47Config::init_reserve_ammo) {}

    std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir, TimePoint now) override {
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!has_ammo() || !can_attack(Ak47Config::attack_rate, now))
            return effects;

        for (int i = 0; i < Ak47Config::burst_bullets; i++) {
            int damage =
                    get_random_damage(Ak47BulletConfig::min_damage, Ak47BulletConfig::max_damage);
            // TODO: This is not needed now. We have to handle burst_frec differently
            // float delay = i * Ak47Config::burst_frec;

            auto effect = std::make_unique<GunAttack>(damage, dir, Ak47BulletConfig::precision);
            effects.push_back(std::move(effect));

            decrease_mag_ammo();
            time_last_attack = now;
        }

        return effects;
    }
};
