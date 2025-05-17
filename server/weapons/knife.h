#pragma once

#include "utility.h"
#include "common/models.h"

class Knife : public Utility {
public:
    Knife() : Utility(UtilityType::Knife) {}

    std::unique_ptr<Utility> clone() const override { return std::make_unique<Knife>(*this); }
};
