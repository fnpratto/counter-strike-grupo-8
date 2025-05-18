#pragma once

#include "utility.h"
#include "common/models.h"
#include "weapons_cons.h"

class Bomb : public Utility {
public:
    Bomb() : Utility(UtilityType::C4, BombConfig::damage) {}

    std::unique_ptr<Utility> clone() const override { return std::make_unique<Bomb>(*this); }
};
