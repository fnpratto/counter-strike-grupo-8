#pragma once

#include "utility.h"
#include "common/models.h"
#include "weapons_cons.h"

class Knife : public Utility {
public:
    Knife() : Utility(UtilityType::Knife, KnifeConfig::damage) {}

    std::unique_ptr<Utility> clone() const override { return std::make_unique<Knife>(*this); }
};
