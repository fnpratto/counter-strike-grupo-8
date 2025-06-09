#include "gun_state.h"

GunState::GunState(GunType gun_type, GunConfig gun_config):
        gun(gun_type),
        gun_config(gun_config),
        mag_ammo(gun_config.init_mag_ammo),
        reserve_ammo(gun_config.init_reserve_ammo),
        is_attacking(false) {
    updates = get_full_update();
}

GunType GunState::get_gun() const { return gun; }
GunConfig GunState::get_gun_config() const { return gun_config; }
int GunState::get_mag_ammo() const { return mag_ammo; }
int GunState::get_reserve_ammo() const { return reserve_ammo; }
bool GunState::get_is_attacking() const { return is_attacking; }

void GunState::set_mag_ammo(int new_mag_ammo) {
    if (mag_ammo == new_mag_ammo)
        return;
    mag_ammo = new_mag_ammo;
    updates.set_mag_ammo(new_mag_ammo);
}

void GunState::set_reserve_ammo(int new_reserve_ammo) {
    if (reserve_ammo == new_reserve_ammo)
        return;
    reserve_ammo = new_reserve_ammo;
    updates.set_reserve_ammo(new_reserve_ammo);
}

void GunState::set_is_attacking(bool is_attacking) {
    if (this->is_attacking == is_attacking)
        return;
    this->is_attacking = is_attacking;
    updates.set_is_attacking(this->is_attacking);
}

GunUpdate GunState::get_full_update() const {
    GunUpdate full_update;
    full_update.set_gun(gun);
    full_update.set_mag_ammo(mag_ammo);
    full_update.set_reserve_ammo(reserve_ammo);
    return full_update;
}
