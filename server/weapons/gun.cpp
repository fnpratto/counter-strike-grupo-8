#include "gun.h"

#include <algorithm>
#include <utility>

#include "common/utils/random_float_generator.h"
#include "server/attack_effects/gun_attack.h"

Gun::Gun(GunType gun, GunConfig initial_config):
        Logic<GunState, GunUpdate>(GunState(gun, initial_config)) {}

std::unique_ptr<Gun> Gun::glock() { return std::make_unique<Gun>(GunType::Glock, GlockConfig); }

std::unique_ptr<Gun> Gun::ak47() { return std::make_unique<Gun>(GunType::AK47, Ak47Config); }

std::unique_ptr<Gun> Gun::awp() { return std::make_unique<Gun>(GunType::AWP, AwpConfig); }

std::unique_ptr<Gun> Gun::m3() { return std::make_unique<Gun>(GunType::M3, M3Config); }

bool Gun::has_ammo() { return state.get_mag_ammo() > 0; }

GunType Gun::get_type() const { return state.get_gun(); }
int Gun::get_mag_ammo() const { return state.get_mag_ammo(); }
int Gun::get_reserve_ammo() const { return state.get_reserve_ammo(); }

void Gun::add_mag() {
    state.set_reserve_ammo(state.get_reserve_ammo() + state.get_gun_config().bullets_per_mag);
}

void Gun::start_attacking() { state.set_is_attacking(true); }

void Gun::decrease_mag_ammo() {
    if (state.get_mag_ammo() > 0)
        state.set_mag_ammo(state.get_mag_ammo() - 1);
}

int Gun::get_random_damage(const int min_dam, const int max_dam) {
    RandomFloatGenerator rfg(min_dam, max_dam);
    return rfg.generate();
}

void Gun::reload() {
    int bullets_to_reload = state.get_gun_config().bullets_per_mag - state.get_mag_ammo();
    state.set_mag_ammo(state.get_mag_ammo() + bullets_to_reload);
    state.set_reserve_ammo(state.get_reserve_ammo() - bullets_to_reload);
}

// TODO: Handle burst_interval
std::vector<std::unique_ptr<AttackEffect>> Gun::attack(Player& player_origin, const Vector2D& dir,
                                                       TimePoint now) {
    GunConfig gun_config = state.get_gun_config();
    std::vector<std::unique_ptr<AttackEffect>> effects;
    if (!state.get_is_attacking() || !has_ammo() || !can_attack(gun_config.attack_rate, now))
        return effects;

    int bullets = std::min(gun_config.bullets_per_attack, state.get_mag_ammo());
    for (int i = 0; i < bullets; i++) {
        int damage = get_random_damage(gun_config.min_damage, gun_config.max_damage);
        Vector2D varied_dir = dir.varied_dir_in_cone(gun_config.dir_variation_angle);

        auto effect = std::make_unique<GunAttack>(player_origin, damage, varied_dir,
                                                  gun_config.precision);
        effects.push_back(std::move(effect));

        decrease_mag_ammo();
        time_last_attack = now;
    }

    return effects;
}
