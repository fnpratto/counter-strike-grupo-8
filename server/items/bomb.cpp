#include "bomb.h"

#include <algorithm>

Bomb::Bomb(const GameConfig::ItemsConfig::BombConfig& bomb_config):
        Logic<BombState, BombUpdate>(
                BombState(bomb_config.secs_to_explode, BombPhaseType::NotPlanted)),
        bomb_config(bomb_config) {}

void Bomb::change_bomb_phase(BombPhaseType new_phase, TimePoint now) {
    if (state.get_bomb_phase() == new_phase)
        return;
    phase_start_time = now;
    state.set_bomb_phase(new_phase);
}

bool Bomb::is_planting() const { return state.get_bomb_phase() == BombPhaseType::Planting; }

bool Bomb::is_planted() const {
    return state.get_bomb_phase() == BombPhaseType::Planted ||
           state.get_bomb_phase() == BombPhaseType::Defusing;
}

bool Bomb::is_defusing() const { return state.get_bomb_phase() == BombPhaseType::Defusing; }

bool Bomb::is_defused() const { return state.get_bomb_phase() == BombPhaseType::Defused; }

bool Bomb::is_exploded() const { return state.get_bomb_phase() == BombPhaseType::Exploded; }

bool Bomb::should_explode() const { return is_planted() && state.get_secs_to_explode() == 0; }

void Bomb::start_planting(TimePoint now) {
    if (state.get_bomb_phase() != BombPhaseType::NotPlanted)
        return;
    change_bomb_phase(BombPhaseType::Planting, now);
}

void Bomb::stop_planting(TimePoint now) {
    if (state.get_bomb_phase() != BombPhaseType::Planting)
        return;
    change_bomb_phase(BombPhaseType::NotPlanted, now);
}

void Bomb::start_defusing(TimePoint now) {
    if (state.get_bomb_phase() != BombPhaseType::Planted)
        return;
    change_bomb_phase(BombPhaseType::Defusing, now);
}

void Bomb::stop_defusing(TimePoint now) {
    if (state.get_bomb_phase() != BombPhaseType::Defusing)
        return;
    change_bomb_phase(BombPhaseType::Planted, now);
}

void Bomb::advance(TimePoint now) {
    if (is_planting()) {
        if (now - phase_start_time >= std::chrono::seconds(bomb_config.secs_to_plant))
            change_bomb_phase(BombPhaseType::Planted, now);
        return;
    }

    if (is_defusing()) {
        if (now - phase_start_time >= std::chrono::seconds(bomb_config.secs_to_defuse))
            change_bomb_phase(BombPhaseType::Defused, now);
    }

    if (!is_planted())
        return;

    float new_secs_to_explode = state.get_secs_to_explode() -
                                std::chrono::duration<float>(now - phase_start_time).count();
    state.set_secs_to_explode(std::max(0.0f, new_secs_to_explode));
}

Effect Bomb::explode(const Vector2D& origin) {
    state.set_bomb_phase(BombPhaseType::Exploded);
    return Effect(origin, bomb_config.damage, bomb_config.max_range, bomb_config.precision,
                  bomb_config.falloff);
}

void Bomb::reset() {
    change_bomb_phase(BombPhaseType::NotPlanted, TimePoint::min());
    state.set_secs_to_explode(bomb_config.secs_to_explode);
}
