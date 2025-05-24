#pragma once

#include "common/models.h"

#include "utility.h"
#include "weapons_cons.h"

class Bomb: public Utility {
public:
    Bomb(): Utility(UtilityType::C4, BombConfig::damage) {}
};
