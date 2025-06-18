#include "bomb_state.h"

BombState::BombState(bool planted): planted(planted) { updates = get_full_update(); }

bool BombState::is_planted() const { return planted; }

void BombState::set_planted(bool planted) {
    if (this->planted == planted)
        return;
    this->planted = planted;
    updates.set_planted(this->planted);
}

BombUpdate BombState::get_full_update() const {
    BombUpdate full_update;
    full_update.set_planted(planted);
    return full_update;
}
