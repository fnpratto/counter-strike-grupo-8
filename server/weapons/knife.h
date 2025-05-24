#pragma once

#include "common/models.h"

#include "utility.h"
#include "weapons_cons.h"

class Knife: public Utility {
public:
    Knife(): Utility(UtilityType::Knife, KnifeConfig::damage) {}
};
