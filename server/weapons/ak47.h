#pragma once

#include "common/models.h"
#include "guns_cons.h"
#include "gun.h"

class Ak47 : public Gun {
public:
    Ak47() : Gun(GunType::AK47, Ak47Config::bullets_per_mag, 
                                Ak47Config::init_mag_ammo, 
                                Ak47Config::init_reserve_ammo) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Ak47>(*this); }
};
