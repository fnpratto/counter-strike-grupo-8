#pragma once

#include <memory>

#include "common/models.h"

class Utility {
protected:
    UtilityType utility;

public:
    Utility(UtilityType utility) : utility(utility) {}

    virtual std::unique_ptr<Utility> clone() const = 0;
    
    UtilityType get_type() const { return utility; }
    
    virtual ~Utility() = default;
};
