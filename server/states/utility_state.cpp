#include "utility_state.h"

UtilityState::UtilityState(UtilityType type, int damage): type(type), damage(damage) {
    updates = get_full_update();
}

UtilityType UtilityState::get_type() const { return type; }

int UtilityState::get_damage() const { return damage; }

void UtilityState::set_type(UtilityType new_type) {
    type = new_type;
    updates.set_type(new_type);
}
void UtilityState::set_damage(int new_damage) {
    damage = new_damage;
    updates.set_damage(new_damage);
}

UtilityUpdate UtilityState::get_full_update() const {
    UtilityUpdate full_update;
    full_update.set_type(type);
    full_update.set_damage(damage);
    return full_update;
}
