#pragma once

#include <memory>

#include "common/models.h"
#include "common/updates/utility_update.h"
#include "server/logic.h"
#include "server/states/utility_state.h"

class Utility: public Logic<UtilityState, UtilityUpdate> {
public:
    Utility(UtilityType type, int damage):
            Logic<UtilityState, UtilityUpdate>(UtilityState(type, damage)) {}

    UtilityType get_type() const { return state.get_type(); }

    virtual ~Utility() = default;
};
