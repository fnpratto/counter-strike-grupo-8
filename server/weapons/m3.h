#pragma once

#include "common/models.h"
#include "guns_cons.h"
#include "gun.h"

class M3 : public Gun {
public:
    M3() : Gun(GunType::M3, M3Config::bullets_per_mag, 
                            M3Config::init_mag_ammo, 
                            M3Config::init_reserve_ammo) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<M3>(*this); }
};
