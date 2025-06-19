#include "bomb.h"

#include <algorithm>

#include "items_config.h"

Bomb::Bomb():
        Logic<BombState, BombUpdate>(
                BombState(BombConfig::secs_to_explode, BombPhaseType::NotPlanted)) {}

bool Bomb::is_planted() const { return state.get_bomb_phase() == BombPhaseType::Planted; }

bool Bomb::should_explode() const {
    return state.get_bomb_phase() == BombPhaseType::Planted && state.get_secs_to_explode() == 0;
}

bool Bomb::get_is_exploded() const { return state.get_bomb_phase() == BombPhaseType::Exploded; }

void Bomb::start_planting(TimePoint now) {
    plant_time = now;
    state.set_bomb_phase(BombPhaseType::Planting);
}

void Bomb::stop_planting(TimePoint now) {
    if (state.get_bomb_phase() != BombPhaseType::Planting)
        return;
    if (now - plant_time < std::chrono::seconds(BombConfig::secs_to_plant))
        state.set_bomb_phase(BombPhaseType::Planted);
    else
        state.set_bomb_phase(BombPhaseType::NotPlanted);
}

void Bomb::advance(TimePoint now) {
    if (!is_planted())
        return;
    float new_secs_to_explode =
            state.get_secs_to_explode() -
            std::chrono::duration_cast<std::chrono::seconds>(now - plant_time).count();
    state.set_secs_to_explode(std::max(0.0f, new_secs_to_explode));
}

Effect Bomb::explode(const Vector2D& origin) {
    state.set_bomb_phase(BombPhaseType::Exploded);
    return Effect(origin, BombConfig::damage, BombConfig::max_range, BombConfig::precision,
                  BombConfig::falloff);
}

void Bomb::reset() {
    state.set_bomb_phase(BombPhaseType::NotPlanted);
    state.set_secs_to_explode(BombConfig::secs_to_explode);
}
