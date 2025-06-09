#include "bomb_state.h"

BombState::BombState() { 
    updates = get_full_update(); 
}

bool BombState::is_planted() const { return planted_pos.has_value(); }

BombUpdate BombState::get_full_update() const {
    BombUpdate full_update;
    if (planted_pos.has_value())
        full_update.set_planted_pos(planted_pos.value());
    else
        full_update.set_planted_pos(std::nullopt);
    return full_update;
}
