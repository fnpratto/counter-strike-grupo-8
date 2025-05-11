#pragma once

#include <map>

#include "common/models.h"

struct Inventory {
    int money;
    std::map<WeaponSlot, WeaponType> weapons;
};
