#pragma once

#include "common/models.h"
#include "common/updates/gun_update.h"

#include "state.h"

class GunState: public State<GunUpdate> {
    GunType gun;
    int bullets_per_mag;
    int mag_ammo;
    int reserve_ammo;

public:
    GunState(GunType gun_type, int bullets_per_mag, int mag_ammo, int reserve_ammo);

    GunType get_gun() const;
    int get_bullets_per_mag() const;
    int get_mag_ammo() const;
    int get_reserve_ammo() const;

    void set_gun(GunType new_gun);
    void set_bullets_per_mag(int new_bullets_per_mag);
    void set_mag_ammo(int new_mag_ammo);
    void set_reserve_ammo(int new_reserve_ammo);

    GunUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};
