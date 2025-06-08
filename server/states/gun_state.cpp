#include "gun_state.h"

GunState::GunState(GunType gun_type, int bullets_per_mag, int mag_ammo, int reserve_ammo):
        gun(gun_type),
        bullets_per_mag(bullets_per_mag),
        mag_ammo(mag_ammo),
        reserve_ammo(reserve_ammo),
        is_attacking(false) {
    updates = get_full_update();
}

GunType GunState::get_gun() const { return gun; }
int GunState::get_bullets_per_mag() const { return bullets_per_mag; }
int GunState::get_mag_ammo() const { return mag_ammo; }
int GunState::get_reserve_ammo() const { return reserve_ammo; }
bool GunState::get_is_attacking() const { return is_attacking; }

void GunState::set_gun(GunType new_gun) {
    if (gun == new_gun)
        return;
    gun = new_gun;
    updates.set_gun(new_gun);
}
void GunState::set_bullets_per_mag(int new_bullets_per_mag) {
    if (bullets_per_mag == new_bullets_per_mag)
        return;
    bullets_per_mag = new_bullets_per_mag;
    updates.set_bullets_per_mag(new_bullets_per_mag);
}
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
    full_update.set_bullets_per_mag(bullets_per_mag);
    full_update.set_mag_ammo(mag_ammo);
    full_update.set_reserve_ammo(reserve_ammo);
    return full_update;
}
