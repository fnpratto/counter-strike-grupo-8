#pragma once

#include "common/models.h"
#include "weapons_cons.h"
#include "gun.h"

class Awp : public Gun {
public:
    Awp() : Gun(GunType::AWP, AwpConfig::bullets_per_mag, 
                              AwpConfig::init_mag_ammo, 
                              AwpConfig::init_reserve_ammo) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Awp>(*this); }

    std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest, TimePoint now) override {
        std::vector<Bullet> bullets;
        if (!can_shoot(AwpConfig::fire_rate, now))
            return bullets;
        
        Vector2D dir = get_bullet_dir(origin, dest);
        Bullet bullet(origin, dir, AwpBulletConfig::speed, AwpBulletConfig::damage, AwpBulletConfig::precision, 0);
        
        bullets.push_back(std::move(bullet));
        
        mag_ammo -= 1;

        return bullets;
    }
};
