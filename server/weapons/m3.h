#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "common/models.h"
#include "server/attack_effects/gun_attack.h"

#include "gun.h"
#include "weapons_config.h"

class M3: public Gun {
public:
    M3():
            Gun(GunType::M3, M3Config::bullets_per_mag, M3Config::init_mag_ammo,
                M3Config::init_reserve_ammo) {}

    std::vector<std::unique_ptr<AttackEffect>> attack(const Vector2D& dir, TimePoint now) override {
        std::vector<std::unique_ptr<AttackEffect>> effects;
        if (!has_ammo() || !can_attack(M3Config::attack_rate, now))
            return effects;

        for (int i = 0; i < M3Config::bullets_per_shoot; i++) {
            int damage = get_random_damage(M3BulletConfig::min_damage, M3BulletConfig::max_damage);
            Vector2D varied_dir = dir.varied_dir_in_cone(M3Config::cone_max_angle);

            auto effect =
                    std::make_unique<GunAttack>(damage, varied_dir, M3BulletConfig::precision);
            effects.push_back(std::move(effect));

            decrease_mag_ammo();
            time_last_attack = now;
        }

        return effects;
    }
};
