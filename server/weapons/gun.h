#pragma once

#include <memory>

#include "common/models.h"

class Gun {
protected:
    GunType gun;
    int bullets_per_magazine;
    int magazine_ammo;
    int reserve_ammo;

public:
    Gun(GunType gun, int bullets_per_magazine, int magazine_ammo, int reserve_ammo) : 
            gun(gun), 
            bullets_per_magazine(bullets_per_magazine),
            magazine_ammo(magazine_ammo), 
            reserve_ammo(reserve_ammo) {}

    virtual std::unique_ptr<Gun> clone() const = 0;

    GunType get_type() const { return gun; }
    int get_bullets_per_magazine() const { return bullets_per_magazine; }
    int get_magazine_ammo() const { return magazine_ammo; }
    int get_reserve_ammo() const { return reserve_ammo; }

    void add_magazines(int num_mags) {
        reserve_ammo += bullets_per_magazine * num_mags;
    }

    virtual ~Gun() = default;
};
