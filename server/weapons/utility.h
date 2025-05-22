#pragma once

#include <memory>

#include "common/models.h"
#include "common/game_state.h"

class Utility {
protected:
    UtilityType utility;
    int damage;

public:
    Utility(UtilityType utility, int damage) : 
            utility(utility),
            damage(damage) {}

    UtilityType get_type() const { return utility; }
    
    UtilityState state() const {
        UtilityState utility_state;
        utility_state.utility = utility;
        return utility_state;
    }

    virtual ~Utility() = default;
};
