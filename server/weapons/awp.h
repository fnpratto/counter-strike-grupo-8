#pragma once

#include "common/models.h"
#include "guns_cons.h"
#include "gun.h"

class Awp : public Gun {
public:
    Awp() : Gun(GunType::AWP, AwpConfig::bullets_per_mag, 
                              AwpConfig::init_mag_ammo, 
                              AwpConfig::init_reserve_ammo) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Awp>(*this); }
};
