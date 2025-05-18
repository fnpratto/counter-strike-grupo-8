#pragma once

#include "common/models.h"
#include "weapons_cons.h"
#include "gun.h"

class Ak47 : public Gun {
public:
    Ak47() : Gun(GunType::AK47, Ak47Config::bullets_per_mag, 
                                Ak47Config::init_mag_ammo, 
                                Ak47Config::init_reserve_ammo) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Ak47>(*this); }

    std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest, TimePoint now) override {
        std::vector<Bullet> bullets;
        if (!can_shoot(Ak47Config::fire_rate, now))
            return bullets;
        
        for (int i = 0; i < Ak47Config::burst_bullets; i++) {
            int damage = get_random_damage(Ak47BulletConfig::min_damage, Ak47BulletConfig::max_damage);
            Vector2D dir = get_bullet_dir(origin, dest);
            float delay = i * Ak47Config::burst_frec;
            Bullet bullet(origin, dir, Ak47BulletConfig::speed, damage, Ak47BulletConfig::precision, delay);
            
            bullets.push_back(std::move(bullet));
        
            mag_ammo -= 1;
        }

        return bullets;
    }
};
