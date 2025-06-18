#include "bomb_state.h"

BombState::BombState(int secs_to_explode, BombPhaseType bomb_phase):
        secs_to_explode(secs_to_explode), bomb_phase(bomb_phase) {
    updates = get_full_update();
}

int BombState::get_secs_to_explode() const { return secs_to_explode; }

BombPhaseType BombState::get_bomb_phase() const { return bomb_phase; }

void BombState::set_secs_to_explode(int secs_to_explode) {
    if (this->secs_to_explode == secs_to_explode)
        return;
    this->secs_to_explode = secs_to_explode;
    updates.set_secs_to_explode(this->secs_to_explode);
}

void BombState::set_bomb_phase(BombPhaseType bomb_phase) {
    if (this->bomb_phase == bomb_phase)
        return;
    this->bomb_phase = bomb_phase;
    updates.set_bomb_phase(this->bomb_phase);
}

BombUpdate BombState::get_full_update() const {
    BombUpdate full_update;
    full_update.set_bomb_phase(bomb_phase);
    full_update.set_secs_to_explode(secs_to_explode);
    return full_update;
}
