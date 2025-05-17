#pragma once

#include "common/models.h"
#include "guns_cons.h"
#include "gun.h"

class Glock : public Gun {
public:
    Glock() : Gun(GunType::Glock, GlockConfig::bullets_per_mag, 
                                  GlockConfig::init_mag_ammo, 
                                  GlockConfig::init_reserve_ammo) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Glock>(*this); }
};
