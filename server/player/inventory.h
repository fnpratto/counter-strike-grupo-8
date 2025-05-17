#pragma once

#include <memory>
#include <map>

#include "common/models.h"
#include "server/weapons/gun.h"
#include "server/weapons/utility.h"

class Inventory {
private:
    int money;
    std::map<WeaponSlot, std::unique_ptr<Gun>> guns;
    std::map<WeaponSlot, std::unique_ptr<Utility>> utilities;

public:
    Inventory();

    Inventory(const Inventory& other);
    Inventory& operator=(const Inventory& other);

    bool has_bomb() const;

    int get_money() const;
    std::unique_ptr<Gun> get_prim_weapon() const;
    std::unique_ptr<Gun> get_sec_weapon() const;
    std::unique_ptr<Utility> get_melee_weapon() const;
    std::unique_ptr<Utility> get_bomb() const;

    void add_money(int amount);
    void decrease_money(int amount);
    
    void add_bomb();
    void add_primary_weapon(const GunType& weapon_type);
    void add_magazines(const WeaponSlot& weapon_slot, int num_mags);

    ~Inventory();
};
