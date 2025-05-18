#pragma once

#include <memory>

#include "common/models.h"

class Utility {
protected:
    UtilityType utility;
    int damage;

public:
    Utility(UtilityType utility, int damage) : 
            utility(utility),
            damage(damage) {}

    virtual std::unique_ptr<Utility> clone() const = 0;
    
    UtilityType get_type() const { return utility; }
    
    virtual ~Utility() = default;
};
