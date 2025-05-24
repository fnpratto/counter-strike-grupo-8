#pragma once

#include <utility>
#include <vector>

#include "common/models.h"

#include "gun.h"
#include "weapons_config.h"

class Glock: public Gun {
public:
    Glock():
            Gun(GunType::Glock, GlockConfig::bullets_per_mag, GlockConfig::init_mag_ammo,
                GlockConfig::init_reserve_ammo) {}

    std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest,
                              TimePoint now) override {
        std::vector<Bullet> bullets;
        if (!can_shoot(GlockConfig::fire_rate, now))
            return bullets;

        int damage =
                get_random_damage(GlockBulletConfig::min_damage, GlockBulletConfig::max_damage);
        Vector2D dir = get_bullet_dir(origin, dest);
        Bullet bullet(origin, dir, damage, GlockBulletConfig::precision, 0);

        bullets.push_back(std::move(bullet));

        mag_ammo -= 1;

        return bullets;
    }
};
