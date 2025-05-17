#pragma once

#include "utility.h"
#include "common/models.h"

class Bomb : public Utility {
public:
    Bomb() : Utility(UtilityType::C4) {}

    std::unique_ptr<Utility> clone() const override { return std::make_unique<Bomb>(*this); }
};
