#pragma once

#include "common/models.h"
#include "common/updates/utility_update.h"

#include "state.h"

class UtilityState: public State<UtilityUpdate> {
    UtilityType type;
    int damage;

public:
    UtilityState(UtilityType type, int damage): type(type), damage(damage) {
        updates = get_full_update();
    }

    UtilityType get_type() const { return type; }
    int get_damage() const { return damage; }

    void set_type(UtilityType new_type) {
        type = new_type;
        updates.set_type(new_type);
    }
    void set_damage(int new_damage) {
        damage = new_damage;
        updates.set_damage(new_damage);
    }

    UtilityUpdate get_full_update() const override {  // cppcheck-suppress[virtualCallInConstructor]
        UtilityUpdate full_update;
        full_update.set_type(type);
        full_update.set_damage(damage);
        return full_update;
    }
};
