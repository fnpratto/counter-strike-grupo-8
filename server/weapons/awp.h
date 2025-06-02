#pragma once

#include <utility>
#include <vector>

#include "common/models.h"

#include "gun.h"
#include "weapons_config.h"

class Awp: public Gun {
public:
    Awp():
            Gun(GunType::AWP, AwpConfig::bullets_per_mag, AwpConfig::init_mag_ammo,
                AwpConfig::init_reserve_ammo) {}

    std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest,
                              TimePoint now) override {
        std::vector<Bullet> bullets;
        if (!can_shoot(AwpConfig::fire_rate, now))
            return bullets;

        Vector2D dir = get_bullet_dir(origin, dest);
        Bullet bullet(origin, dir, AwpBulletConfig::damage, AwpBulletConfig::precision, 0);

        bullets.push_back(std::move(bullet));

        decrease_mag_ammo();

        return bullets;
    }
};
