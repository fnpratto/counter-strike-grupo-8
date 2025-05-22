#pragma once

#include "utility.h"
#include "common/models.h"
#include "weapons_cons.h"

class Knife : public Utility {
public:
    Knife() : Utility(UtilityType::Knife, KnifeConfig::damage) {}
};
