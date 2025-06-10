#pragma once

#include "common/models.h"
#include "common/updates/gun_update.h"
#include "server/weapons/weapons_config.h"

#include "state.h"

class GunState: public State<GunUpdate> {
    GunType gun;
    GunConfig gun_config;
    int mag_ammo;
    int reserve_ammo;
    bool is_attacking;

public:
    GunState(GunType gun_type, GunConfig gun_config);

    GunType get_gun() const;
    GunConfig get_gun_config() const;
    int get_mag_ammo() const;
    int get_reserve_ammo() const;
    bool get_is_attacking() const;

    void set_gun(GunType new_gun);
    void set_bullets_per_mag(int new_bullets_per_mag);
    void set_mag_ammo(int new_mag_ammo);
    void set_reserve_ammo(int new_reserve_ammo);
    void set_is_attacking(bool is_attacking);

    GunUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};
