#include "knife_state.h"

KnifeState::KnifeState(): is_attacking(false) { updates = get_full_update(); }

bool KnifeState::get_is_attacking() const { return is_attacking; }

void KnifeState::set_is_attacking(bool is_attacking) {
    if (this->is_attacking == is_attacking)
        return;
    this->is_attacking = is_attacking;
    updates.set_is_attacking(this->is_attacking);
}

KnifeUpdate KnifeState::get_full_update() const {
    KnifeUpdate full_update;
    full_update.set_is_attacking(false);
    return full_update;
}
