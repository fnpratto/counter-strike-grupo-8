#pragma once

#include "common/models.h"
#include "common/updates/utility_update.h"

#include "state.h"

class UtilityState: public State<UtilityUpdate> {
    UtilityType type;
    int damage;

public:
    UtilityState(UtilityType type, int damage);

    UtilityType get_type() const;
    int get_damage() const;

    void set_type(UtilityType new_type);
    void set_damage(int new_damage);

    UtilityUpdate get_full_update() const override;  // cppcheck-suppress[virtualCallInConstructor]
};
