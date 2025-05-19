#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "server/map/map.h"
#include "server/clock/clock.h"
#include "server/utils/vector_2d.h"
#include "server/utils/random_float_generator.h"
#include "bullet.h"

class Gun {
protected:
    GunType gun;
    int bullets_per_mag;
    int mag_ammo;
    int reserve_ammo;
    TimePoint time_last_shoot = TimePoint{};

public:
    Gun(GunType gun, int bullets_per_mag, int mag_ammo, int reserve_ammo) : 
            gun(gun), 
            bullets_per_mag(bullets_per_mag),
            mag_ammo(mag_ammo), 
            reserve_ammo(reserve_ammo) {}

    virtual std::unique_ptr<Gun> clone() const = 0;

    bool can_shoot(const float fire_rate, TimePoint now) {
        if (mag_ammo == 0)
            return false;
        std::chrono::duration<float> secs_btw_shoots = now - time_last_shoot;
        if (secs_btw_shoots.count() < (1.0f / fire_rate))
            return false;
        return true;
    }

    GunType get_type() const { return gun; }
    int get_bullets_per_mag() const { return bullets_per_mag; }
    int get_mag_ammo() const { return mag_ammo; }
    int get_reserve_ammo() const { return reserve_ammo; }

    void add_mags(int num_mags) {
        reserve_ammo += bullets_per_mag * num_mags;
    }

    int get_random_damage(const int min_dam, const int max_dam) {
        RandomFloatGenerator rfg(min_dam, max_dam);
        return rfg.generate();
    }

    Vector2D get_bullet_dir(const Vector2D& origin, const Vector2D& dest) {
        Vector2D dir(origin, dest);
        return dir.normalized();
    }

    virtual std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest, TimePoint now) = 0;

    void reload() {
        int bullets_to_reload = bullets_per_mag - mag_ammo;
        mag_ammo += bullets_to_reload;
        reserve_ammo -= bullets_to_reload;
    }

    virtual ~Gun() = default;
};
