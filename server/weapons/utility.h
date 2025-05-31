#pragma once

#include <memory>

#include "common/models.h"
#include "common/updates/utility_update.h"
#include "common/utility_state.h"

class Utility {
protected:
    UtilityState state;

public:
    Utility(UtilityType type, int damage): state(type, damage) {}

    UtilityType get_type() const { return state.get_type(); }
    UtilityUpdate get_updates() const { return state.get_updates(); }
    void clear_updates() { state.clear_updates(); }

    UtilityState get_state() const { return state; }

    virtual ~Utility() = default;
};
