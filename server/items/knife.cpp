#include "knife.h"

#include <utility>

#include "items_config.h"

Knife::Knife(): Logic<KnifeState, KnifeUpdate>(KnifeState()) {}

bool Knife::is_attacking() const { return state.get_is_attacking(); }

void Knife::start_attacking(TimePoint now) {
    if (!can_attack(KnifeConfig::attack_rate, now))
        return;
    state.set_is_attacking(true);
}

std::vector<AttackEffect> Knife::attack(const Vector2D& origin, const Vector2D& dir,
                                        TimePoint now) {
    std::vector<AttackEffect> effects;
    if (!is_attacking())
        return effects;
    int damage = get_random_damage(KnifeConfig::min_damage, KnifeConfig::max_damage);

    Effect effect(origin, damage, KnifeConfig::max_range, KnifeConfig::precision,
                  KnifeConfig::falloff);
    effects.push_back(AttackEffect{std::move(effect), dir});

    state.set_is_attacking(false);
    time_last_attack = now;
    return effects;
}

void Knife::reset() {
    time_last_attack = TimePoint::min();
    state.set_is_attacking(false);
}
