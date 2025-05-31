#pragma once

#include "common/models.h"
#include "common/state.h"
#include "common/updates/utility_update.h"

class UtilityState: public State<UtilityUpdate> {
    UtilityType type;
    int damage;

public:
    UtilityState() = default;
    UtilityState(UtilityType utility_type, int damage): type(utility_type), damage(damage) {}

    UtilityType get_type() const { return type; }
    int get_damage() const { return damage; }

    void set_utility(UtilityType new_utility) {
        type = new_utility;
        updates.set_utility(new_utility);
    }
};
