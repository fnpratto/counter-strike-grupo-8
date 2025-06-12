#pragma once

#include "common/models.h"
#include "common/updates/gun_update.h"

#include "state.h"

class GunState: public State<GunUpdate> {
    GunType gun;
    int mag_ammo;
    int reserve_ammo;
    bool is_attacking;

public:
    GunState(GunType gun_type, int init_mag_ammo, int init_reserve_ammo);

    GunType get_gun() const;
    int get_mag_ammo() const;
    int get_reserve_ammo() const;
    bool get_is_attacking() const;

    void set_mag_ammo(int new_mag_ammo);
    void set_reserve_ammo(int new_reserve_ammo);
    void set_is_attacking(bool is_attacking);

    GunUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};
