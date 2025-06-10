#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "common/utils/random_float_generator.h"
#include "common/utils/vector_2d.h"
#include "server/attack_effects/attack_effect.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/states/gun_state.h"

#include "weapon.h"

class Gun: public Logic<GunState, GunUpdate>, public Weapon {
public:
    Gun(GunType gun, int bullets_per_mag, int mag_ammo, int reserve_ammo):
            Logic<GunState, GunUpdate>(GunState(gun, bullets_per_mag, mag_ammo, reserve_ammo)) {}

    bool has_ammo() { return state.get_mag_ammo() > 0; }

    GunType get_type() const { return state.get_gun(); }
    int get_bullets_per_mag() const { return state.get_bullets_per_mag(); }
    int get_mag_ammo() const { return state.get_mag_ammo(); }
    int get_reserve_ammo() const { return state.get_reserve_ammo(); }

    void add_mag() {
        // TODO bullets per mag could be a constant per gun type
        state.set_reserve_ammo(state.get_reserve_ammo() + state.get_bullets_per_mag());
    }

    void decrease_mag_ammo() {
        if (state.get_mag_ammo() > 0)
            state.set_mag_ammo(state.get_mag_ammo() - 1);
    }

    int get_random_damage(const int min_dam, const int max_dam) {
        RandomFloatGenerator rfg(min_dam, max_dam);
        return rfg.generate();
    }

    void reload() {
        int bullets_to_reload = state.get_bullets_per_mag() - state.get_mag_ammo();
        state.set_mag_ammo(state.get_mag_ammo() + bullets_to_reload);
        state.set_reserve_ammo(state.get_reserve_ammo() - bullets_to_reload);
    }

    virtual ~Gun() = default;
};
