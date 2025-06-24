#include "knife.h"

#include <utility>

Knife::Knife(const GameConfig::ItemsConfig::KnifeConfig& knife_config):
        Logic<KnifeState, KnifeUpdate>(KnifeState()), knife_config(knife_config) {}

bool Knife::is_attacking() const { return state.get_is_attacking(); }

void Knife::start_attacking(TimePoint now) {
    if (!can_attack(knife_config.attack_rate, now))
        return;
    state.set_is_attacking(true);
}

std::vector<AttackEffect> Knife::attack(const Vector2D& origin, const Vector2D& dir,
                                        TimePoint now) {
    std::vector<AttackEffect> effects;
    if (!is_attacking())
        return effects;
    int damage = get_random_damage(knife_config.min_damage, knife_config.max_damage);

    Effect effect(origin, damage, knife_config.max_range, knife_config.precision,
                  knife_config.falloff);
    effects.push_back(AttackEffect{std::move(effect), dir});

    state.set_is_attacking(false);
    time_last_attack = now;
    return effects;
}

void Knife::reset() {
    time_last_attack = TimePoint();
    state.set_is_attacking(false);
}
