#pragma once

#include "common/models.h"
#include "weapons_cons.h"
#include "gun.h"

class M3 : public Gun {
public:
    M3() : Gun(GunType::M3, M3Config::bullets_per_mag, 
                            M3Config::init_mag_ammo, 
                            M3Config::init_reserve_ammo) {}

    std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest, TimePoint now) override {
        std::vector<Bullet> bullets;
        if (!can_shoot(M3Config::fire_rate, now))
            return bullets;
        
        for (int i = 0; i < M3Config::burst_bullets; i++) {
            int damage = get_random_damage(M3BulletConfig::min_damage, M3BulletConfig::max_damage);
            Vector2D dir = get_bullet_dir(origin, dest);
            dir.vary_dir_in_cone(M3Config::cone_max_angle);
            float delay = i * M3Config::burst_frec;
            Bullet bullet(origin, dir, damage, M3BulletConfig::precision, delay);
            
            bullets.push_back(std::move(bullet));
            
            mag_ammo -= 1;
        }

        return bullets;
    }
};
