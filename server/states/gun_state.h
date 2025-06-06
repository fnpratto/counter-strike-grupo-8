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
    GunState(GunType gun_type, int bullets_per_mag, int mag_ammo, int reserve_ammo):
            gun(gun_type),
            bullets_per_mag(bullets_per_mag),
            mag_ammo(mag_ammo),
            reserve_ammo(reserve_ammo) {
        updates = get_full_update();
    }

    GunType get_gun() const { return gun; }
    int get_bullets_per_mag() const { return bullets_per_mag; }
    int get_mag_ammo() const { return mag_ammo; }
    int get_reserve_ammo() const { return reserve_ammo; }

    void set_gun(GunType new_gun) {
        gun = new_gun;
        updates.set_gun(new_gun);
    }
    void set_bullets_per_mag(int new_bullets_per_mag) {
        bullets_per_mag = new_bullets_per_mag;
        updates.set_bullets_per_mag(new_bullets_per_mag);
    }
    void set_mag_ammo(int new_mag_ammo) {
        mag_ammo = new_mag_ammo;
        updates.set_mag_ammo(new_mag_ammo);
    }
    void set_reserve_ammo(int new_reserve_ammo) {
        reserve_ammo = new_reserve_ammo;
        updates.set_reserve_ammo(new_reserve_ammo);
    }

    GunUpdate get_full_update() const override {
        GunUpdate full_update;
        full_update.set_gun(gun);
        full_update.set_bullets_per_mag(bullets_per_mag);
        full_update.set_mag_ammo(mag_ammo);
        full_update.set_reserve_ammo(reserve_ammo);
        return full_update;
    }
};
