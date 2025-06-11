#include "bomb_state.h"

BombState::BombState(): planted(false) { updates = get_full_update(); }

bool BombState::is_planted() const { return planted; }

BombUpdate BombState::get_full_update() const {
    BombUpdate full_update;
    full_update.set_planted(planted);
    return full_update;
}
