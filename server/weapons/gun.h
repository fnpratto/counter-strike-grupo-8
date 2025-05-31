#pragma once

#include <memory>
#include <vector>

#include "common/models.h"
#include "server/clock/clock.h"
#include "server/logic.h"
#include "server/map/map.h"
#include "server/states/gun_state.h"
#include "server/utils/random_float_generator.h"
#include "server/utils/vector_2d.h"

#include "bullet.h"

class Gun: public Logic<GunState, GunUpdate> {
protected:
    TimePoint time_last_shoot = TimePoint{};

public:
    Gun(GunType gun, int bullets_per_mag, int mag_ammo, int reserve_ammo):
            Logic<GunState, GunUpdate>(GunState(gun, bullets_per_mag, mag_ammo, reserve_ammo)) {}

    bool can_shoot(const float fire_rate, TimePoint now) {
        if (state.get_mag_ammo() == 0)
            return false;
        std::chrono::duration<float> secs_btw_shoots = now - time_last_shoot;
        if (secs_btw_shoots.count() < (1.0f / fire_rate))
            return false;
        return true;
    }

    GunType get_type() const { return state.get_gun(); }
    int get_bullets_per_mag() const { return state.get_bullets_per_mag(); }
    int get_mag_ammo() const { return state.get_mag_ammo(); }
    int get_reserve_ammo() const { return state.get_reserve_ammo(); }

    void add_mag() {
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

    Vector2D get_bullet_dir(const Vector2D& origin, const Vector2D& dest) {
        Vector2D dir(origin, dest);
        return dir.normalized();
    }

    virtual std::vector<Bullet> shoot(const Vector2D& origin, const Vector2D& dest,
                                      TimePoint now) = 0;

    void reload() {
        int bullets_to_reload = state.get_bullets_per_mag() - state.get_mag_ammo();
        state.set_mag_ammo(state.get_mag_ammo() + bullets_to_reload);
        state.set_reserve_ammo(state.get_reserve_ammo() - bullets_to_reload);
    }

    virtual ~Gun() = default;
};
