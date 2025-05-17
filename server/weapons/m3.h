#pragma once

#include "common/models.h"
#include "gun.h"

class M3 : public Gun {
public:
    M3() : Gun(GunType::M3, bullets_per_magazine=1, magazine_ammo=8, reserve_ammo=32) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<M3>(*this); }
};
