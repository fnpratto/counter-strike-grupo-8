#pragma once

#include "common/models.h"
#include "gun.h"

class Glock : public Gun {
public:
    Glock() : Gun(GunType::Glock, bullets_per_magazine=20, magazine_ammo=20, reserve_ammo=120) {}

    std::unique_ptr<Gun> clone() const override { return std::make_unique<Glock>(*this); }
};
