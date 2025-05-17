#pragma once

#include "common/models.h"
#include "gun.h"

class Awp : public Gun {
public:
    Awp() : Gun(GunType::AWP, bullets_per_magazine=10, magazine_ammo=10, reserve_ammo=30) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Awp>(*this); }
};
