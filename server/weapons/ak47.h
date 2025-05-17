#pragma once

#include "common/models.h"
#include "gun.h"

class Ak47 : public Gun {
public:
    Ak47() : Gun(GunType::AK47, bullets_per_magazine=30, magazine_ammo=30, reserve_ammo=90) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Ak47>(*this); }
};
