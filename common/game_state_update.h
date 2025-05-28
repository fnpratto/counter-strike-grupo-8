#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

#include "common/game_state.h"
#include "server/clock/clock.h"
#include "server/utils/vector_2d.h"

// Type traits for attribute-value mapping
template <typename Attr, Attr A, typename ExpectedType>
struct AttrTypeTrait {
    using expected_type = ExpectedType;
    static constexpr Attr attr = A;
};


template <typename A, typename V>
class StateUpdate {
protected:
    std::map<A, V> changes;

public:
    const std::map<A, V>& get_changes() const { return changes; }

    bool has_change(A attr) const { return changes.find(attr) != changes.end(); }

    template <typename T>
    T get_change(A attr) const {
        const auto& value = changes.at(attr);
        const T* ptr = std::get_if<T>(&value);
        if (!ptr) {
            throw std::bad_variant_access();
        }
        return *ptr;
    }

    void clear() { changes.clear(); }
};


// GunUpdate type checking
enum class GunAttr { TYPE, MAG_AMMO, RESERVE_AMMO };
typedef std::variant<GunType, int> GunValue;


class GunUpdate: public StateUpdate<GunAttr, GunValue> {
public:
    template <typename T>
    void add_change(GunAttr attr, T value) {
        // Validate attribute-specific type at runtime
        if (!is_valid_type_for_attr(attr, value)) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        changes[attr] = value;
    }

    GunUpdate merged(const GunUpdate& other) const {
        GunUpdate merged;

        const auto& new_changes = other.get_changes();

        for (const auto& [a, v]: changes) {
            bool has_new = new_changes.find(a) != new_changes.end();
            const auto& val = has_new ? new_changes.at(a) : v;

            if (a == GunAttr::TYPE) {
                const auto& new_updates = std::get<GunType>(val);
                merged.add_change(a, new_updates);
            } else if (a == GunAttr::MAG_AMMO || a == GunAttr::RESERVE_AMMO) {
                const auto& new_updates = std::get<int>(val);
                merged.add_change(a, new_updates);
            }
        }

        for (const auto& [a, v]: new_changes) {
            if (changes.find(a) == changes.end()) {
                if (a == GunAttr::TYPE) {
                    const auto& new_updates = std::get<GunType>(v);
                    merged.add_change(a, new_updates);
                } else if (a == GunAttr::MAG_AMMO || a == GunAttr::RESERVE_AMMO) {
                    const auto& new_updates = std::get<int>(v);
                    merged.add_change(a, new_updates);
                }
            }
        }

        return merged;
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GunAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case GunAttr::TYPE:
                return std::is_same_v<T, GunType>;
            case GunAttr::MAG_AMMO:
            case GunAttr::RESERVE_AMMO:
                return std::is_same_v<T, int>;
            default:
                return false;
        }
    }
};

// InventoryUpdate type checking
enum class InventoryAttr { GUNS, UTILITIES };
typedef std::variant<std::map<WeaponSlot, GunUpdate>, std::map<WeaponSlot, UtilityType>>
        InventoryValue;


class InventoryUpdate: public StateUpdate<InventoryAttr, InventoryValue> {
public:
    template <typename T>
    void add_change(InventoryAttr attr, T value) {
        // Validate attribute-specific type at runtime
        if (!is_valid_type_for_attr(attr, value)) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        changes[attr] = value;
    }

    void add_change(InventoryAttr attr, std::map<WeaponSlot, UtilityType> value) {
        // Validate attribute-specific type at runtime
        if (attr != InventoryAttr::UTILITIES) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        std::map<WeaponSlot, UtilityType> merged;

        if (has_change(attr)) {
            const auto& old_updates = std::get<std::map<WeaponSlot, UtilityType>>(changes[attr]);
            for (const auto& [slot, utility_type]: old_updates) {
                bool has_new = value.find(slot) != value.end();
                const auto& val = has_new ? value.at(slot) : utility_type;
                merged[slot] = val;
            }
            for (auto& [slot, utility_type]: value) {
                if (old_updates.find(slot) == old_updates.end())
                    merged[slot] = utility_type;
            }
        } else {
            merged = std::move(value);
        }

        changes[attr] = std::move(merged);
    }

    void add_change(InventoryAttr attr, std::map<WeaponSlot, GunUpdate> value) {
        // Validate attribute-specific type at runtime
        if (attr != InventoryAttr::GUNS) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        std::map<WeaponSlot, GunUpdate> merged;
        const auto& old_updates = std::get<std::map<WeaponSlot, GunUpdate>>(changes[attr]);
        for (const auto& [slot, update]: old_updates) {
            if (value.find(slot) != value.end()) {
                merged[slot] = update.merged(value[slot]);
            } else {
                merged[slot] = update;
            }
        }
        for (auto& [slot, update]: value) {
            if (old_updates.find(slot) == old_updates.end())
                merged[slot] = update;
        }

        changes[attr] = merged;
    }

    InventoryUpdate merged(const InventoryUpdate& other) const {
        InventoryUpdate merged;

        const auto& new_changes = other.get_changes();

        for (const auto& [a, v]: changes) {
            bool has_new = new_changes.find(a) != new_changes.end();
            const auto& val = has_new ? new_changes.at(a) : v;

            if (a == InventoryAttr::GUNS) {
                const auto& new_updates = std::get<std::map<WeaponSlot, GunUpdate>>(val);
                const auto& old_updates = std::get<std::map<WeaponSlot, GunUpdate>>(v);
                // Merge maps
                std::map<WeaponSlot, GunUpdate> guns_merged;
                for (const auto& [slot, gun]: old_updates) {
                    bool has_new = new_updates.find(slot) != new_updates.end();
                    const auto& val = has_new ? new_updates.at(slot) : gun;
                    guns_merged[slot] = gun.merged(val);
                }
                for (auto& [slot, gun]: new_updates) {
                    if (old_updates.find(slot) == old_updates.end())
                        guns_merged[slot] = gun;
                }
                merged.add_change(a, guns_merged);
            } else if (a == InventoryAttr::UTILITIES) {
                const auto& new_updates = std::get<std::map<WeaponSlot, UtilityType>>(val);
                const auto& old_updates = std::get<std::map<WeaponSlot, UtilityType>>(v);
                // Merge maps
                std::map<WeaponSlot, UtilityType> utilities_merged;
                for (const auto& [slot, util]: old_updates) {
                    bool has_new = new_updates.find(slot) != new_updates.end();
                    const auto& val = has_new ? new_updates.at(slot) : util;
                    utilities_merged[slot] = val;
                }
                for (auto& [slot, util]: new_updates) {
                    if (old_updates.find(slot) == old_updates.end())
                        utilities_merged[slot] = util;
                }
                merged.add_change(a, utilities_merged);
            }
        }

        for (const auto& [a, v]: new_changes) {
            if (changes.find(a) == changes.end()) {
                if (a == InventoryAttr::GUNS) {
                    const auto& new_updates = std::get<std::map<WeaponSlot, GunUpdate>>(v);
                    merged.add_change(a, new_updates);
                } else if (a == InventoryAttr::UTILITIES) {
                    const auto& new_updates = std::get<std::map<WeaponSlot, UtilityType>>(v);
                    merged.add_change(a, new_updates);
                }
            }
        }

        return merged;
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(InventoryAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case InventoryAttr::GUNS:
                return std::is_same_v<T, std::map<WeaponSlot, GunUpdate>>;
            case InventoryAttr::UTILITIES:
                return std::is_same_v<T, std::map<WeaponSlot, UtilityType>>;
            default:
                return false;
        }
    }
};

// PlayerUpdate type checking
enum class PlayerAttr {
    READY,
    MONEY,
    TEAM,
    INVENTORY,
    HEALTH,
    POS_X,
    POS_Y,
    AIM_X,
    AIM_Y,
    CURRENT_WEAPON,
    IS_MOVING,
    MOVE_DIR
};
typedef std::variant<bool, Team, InventoryUpdate, int, float, WeaponSlot, Vector2D> PlayerValue;

class PlayerUpdate: public StateUpdate<PlayerAttr, PlayerValue> {
public:
    template <typename T>
    void add_change(PlayerAttr attr, T value) {
        if (!is_valid_type_for_attr(attr, value)) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        changes[attr] = value;
    }

    void add_change(PlayerAttr attr, InventoryUpdate value) {
        // Validate attribute-specific type at runtime
        if (attr != PlayerAttr::INVENTORY) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        const auto& old_updates = std::get<InventoryUpdate>(changes[attr]);
        changes[attr] = old_updates.merged(value);
    }

    PlayerUpdate merged(const PlayerUpdate& other) const {
        PlayerUpdate merged;

        const auto& new_changes = other.get_changes();

        for (const auto& [a, v]: changes) {
            bool has_new = new_changes.find(a) != new_changes.end();
            const auto& val = has_new ? new_changes.at(a) : v;

            if (a == PlayerAttr::INVENTORY) {
                const auto& new_updates = std::get<InventoryUpdate>(val);
                const auto& old_updates = std::get<InventoryUpdate>(v);
                merged.add_change(a, old_updates.merged(new_updates));
            } else if (a == PlayerAttr::READY || a == PlayerAttr::IS_MOVING) {
                const auto& new_updates = std::get<bool>(val);
                merged.add_change(a, new_updates);
            } else if (a == PlayerAttr::HEALTH || a == PlayerAttr::MONEY) {
                const auto& new_updates = std::get<int>(val);
                merged.add_change(a, new_updates);
            } else if (a == PlayerAttr::TEAM) {
                const auto& new_updates = std::get<Team>(val);
                merged.add_change(a, new_updates);
            } else if (a == PlayerAttr::POS_X || a == PlayerAttr::POS_Y || a == PlayerAttr::AIM_X ||
                       a == PlayerAttr::AIM_Y) {
                const auto& new_updates = std::get<float>(val);
                merged.add_change(a, new_updates);
            } else if (a == PlayerAttr::CURRENT_WEAPON) {
                const auto& new_updates = std::get<WeaponSlot>(val);
                merged.add_change(a, new_updates);
            } else if (a == PlayerAttr::MOVE_DIR) {
                const auto& new_updates = std::get<Vector2D>(val);
                merged.add_change(a, new_updates);
            }
        }

        for (const auto& [a, v]: new_changes) {
            if (changes.find(a) == changes.end()) {
                if (a == PlayerAttr::INVENTORY) {
                    const auto& new_updates = std::get<InventoryUpdate>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PlayerAttr::READY || a == PlayerAttr::IS_MOVING) {
                    const auto& new_updates = std::get<bool>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PlayerAttr::HEALTH || a == PlayerAttr::MONEY) {
                    const auto& new_updates = std::get<int>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PlayerAttr::TEAM) {
                    const auto& new_updates = std::get<Team>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PlayerAttr::POS_X || a == PlayerAttr::POS_Y ||
                           a == PlayerAttr::AIM_X || a == PlayerAttr::AIM_Y) {
                    const auto& new_updates = std::get<float>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PlayerAttr::CURRENT_WEAPON) {
                    const auto& new_updates = std::get<WeaponSlot>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PlayerAttr::MOVE_DIR) {
                    const auto& new_updates = std::get<Vector2D>(v);
                    merged.add_change(a, new_updates);
                }
            }
        }

        return merged;
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(PlayerAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case PlayerAttr::READY:
            case PlayerAttr::IS_MOVING:
                return std::is_same_v<T, bool>;
            case PlayerAttr::HEALTH:
            case PlayerAttr::MONEY:
                return std::is_same_v<T, int>;
            case PlayerAttr::TEAM:
                return std::is_same_v<T, Team>;
            case PlayerAttr::INVENTORY:
                return std::is_same_v<T, InventoryUpdate>;
            case PlayerAttr::POS_X:
            case PlayerAttr::POS_Y:
            case PlayerAttr::AIM_X:
            case PlayerAttr::AIM_Y:
                return std::is_same_v<T, float>;
            case PlayerAttr::CURRENT_WEAPON:
                return std::is_same_v<T, WeaponSlot>;
            case PlayerAttr::MOVE_DIR:
                return std::is_same_v<T, Vector2D>;
            default:
                return false;
        }
    }
};


// PhaseUpdate type checking
enum class PhaseAttr { PHASE, TIME };
typedef std::variant<PhaseType, TimePoint> PhaseValue;


class PhaseUpdate: public StateUpdate<PhaseAttr, PhaseValue> {
public:
    template <typename T>
    void add_change(PhaseAttr attr, T value) {
        // Validate attribute-specific type at runtime
        if (!is_valid_type_for_attr(attr, value)) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        changes[attr] = value;
    }

    PhaseUpdate merged(const PhaseUpdate& other) const {
        PhaseUpdate merged;

        const auto& new_changes = other.get_changes();

        for (const auto& [a, v]: changes) {
            bool has_new = new_changes.find(a) != new_changes.end();
            const auto& val = has_new ? new_changes.at(a) : v;

            if (a == PhaseAttr::PHASE) {
                const auto& new_updates = std::get<PhaseType>(val);
                merged.add_change(a, new_updates);
            } else if (a == PhaseAttr::TIME) {
                const auto& new_updates = std::get<TimePoint>(val);
                merged.add_change(a, new_updates);
            }
        }

        for (const auto& [a, v]: new_changes) {
            if (changes.find(a) == changes.end()) {
                if (a == PhaseAttr::PHASE) {
                    const auto& new_updates = std::get<PhaseType>(v);
                    merged.add_change(a, new_updates);
                } else if (a == PhaseAttr::TIME) {
                    const auto& new_updates = std::get<TimePoint>(v);
                    merged.add_change(a, new_updates);
                }
            }
        }

        return merged;
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(PhaseAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case PhaseAttr::PHASE:
                return std::is_same_v<T, PhaseType>;
            case PhaseAttr::TIME:
                return std::is_same_v<T, TimePoint>;
            default:
                return false;
        }
    }
};


// GameUpdate type checking
enum class GameStateAttr { PHASE, NUM_ROUNDS, NUM_TTS, PLAYERS };
typedef std::variant<PhaseUpdate, int, std::map<std::string, PlayerUpdate>> GameStateValue;


class GameUpdate: public StateUpdate<GameStateAttr, GameStateValue> {
public:
    template <typename T>
    void add_change(GameStateAttr attr, T value) {
        // Validate attribute-specific type at runtime
        if (!is_valid_type_for_attr(attr, value)) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        changes[attr] = value;
    }

    void add_change(GameStateAttr attr, PhaseUpdate value) {
        // Validate attribute-specific type at runtime
        if (attr != GameStateAttr::PHASE) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        const auto& old_updates = std::get<PhaseUpdate>(changes[attr]);
        changes[attr] = old_updates.merged(value);
    }

    void add_change(GameStateAttr attr, std::map<std::string, PlayerUpdate> value) {
        // Validate attribute-specific type at runtime
        if (attr != GameStateAttr::PLAYERS) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        // Merge maps
        std::map<std::string, PlayerUpdate> merged;
        if (has_change(attr)) {
            const auto& old_updates = std::get<std::map<std::string, PlayerUpdate>>(changes[attr]);
            for (const auto& [p_name, update]: old_updates) {
                if (value.find(p_name) != value.end()) {
                    merged[p_name] = update.merged(value[p_name]);
                } else {
                    merged[p_name] = update;
                }
            }
            for (auto& [p_name, update]: value) {
                if (old_updates.find(p_name) == old_updates.end())
                    merged[p_name] = update;
            }
        } else {
            merged = std::move(value);
        }

        changes[attr] = std::move(merged);
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GameStateAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case GameStateAttr::PHASE:
                return std::is_same_v<T, PhaseUpdate>;
            case GameStateAttr::NUM_ROUNDS:
            case GameStateAttr::NUM_TTS:
                return std::is_same_v<T, int>;
            case GameStateAttr::PLAYERS:
                return std::is_same_v<T, std::map<std::string, PlayerUpdate>>;
            default:
                return false;
        }
    }
};

// Example usage

class Demo {
public:
    void example() {
        GunUpdate gun_update;
        gun_update.add_change(GunAttr::TYPE, GunType::AK47);
        gun_update.add_change(GunAttr::MAG_AMMO, 10);
        gun_update.add_change(GunAttr::RESERVE_AMMO, 90);

        InventoryUpdate inv_update;
        inv_update.add_change(InventoryAttr::GUNS, std::map<WeaponSlot, GunUpdate>{});
        inv_update.add_change(InventoryAttr::UTILITIES, std::map<WeaponSlot, UtilityType>{});

        PlayerUpdate player_update;
        player_update.add_change(PlayerAttr::TEAM, Team::TT);
        player_update.add_change(PlayerAttr::INVENTORY, InventoryUpdate{});
        player_update.add_change(PlayerAttr::HEALTH, 100);
        player_update.add_change(PlayerAttr::POS_X, 50.0f);
        player_update.add_change(PlayerAttr::POS_Y, 75.0f);
        player_update.add_change(PlayerAttr::AIM_X, 0.1f);
        player_update.add_change(PlayerAttr::AIM_Y, 0.5f);
        player_update.add_change(PlayerAttr::IS_MOVING, true);
        player_update.add_change(PlayerAttr::MOVE_DIR, Vector2D(1, 1));

        PhaseUpdate phase_update;
        phase_update.add_change(PhaseAttr::PHASE, PhaseType::WarmUp);
        phase_update.add_change(PhaseAttr::TIME, TimePoint{});

        GameUpdate game_state_update;
        game_state_update.add_change(GameStateAttr::PHASE, phase_update);
        game_state_update.add_change(GameStateAttr::NUM_ROUNDS, 5);
        game_state_update.add_change(GameStateAttr::NUM_TTS, 3);
        game_state_update.add_change(GameStateAttr::PLAYERS, std::map<std::string, PlayerUpdate>{});
    }
};
