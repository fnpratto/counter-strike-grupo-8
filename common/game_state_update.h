#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "common/game_state.h"

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

    void clear() { changes.clear(); }
};


// GunUpdate type checking
enum class GunAttr { MAG_AMMO, RESERVE_AMMO };
typedef std::variant<int> GunValue;


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
            if (new_changes.find(a) != new_changes.end()) {
                merged.add_change(a, new_changes.at(a));
            } else {
                merged.add_change(a, v);
            }
        }

        for (const auto& [a, v]: new_changes) {
            if (changes.find(a) == changes.end())
                merged.add_change(a, v);
        }

        return merged;
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GunAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
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
        const auto& old_updates = std::get<std::map<WeaponSlot, UtilityType>>(changes[attr]);
        for (const auto& [slot, utility_type]: old_updates) {
            if (value.find(slot) != value.end()) {
                merged[slot] = value[slot];
            } else {
                merged[slot] = utility_type;
            }
        }
        for (auto& [slot, utility_type]: value) {
            if (old_updates.find(slot) == old_updates.end())
                merged[slot] = utility_type;
        }

        changes[attr] = merged;
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
    CURRENT_WEAPON
};
typedef std::variant<bool, Team, InventoryUpdate, int, float, WeaponSlot> PlayerValue;

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

    PlayerUpdate merged(const PlayerUpdate& other) const {
        PlayerUpdate merged;

        const auto& new_changes = other.get_changes();

        for (const auto& [a, v]: changes) {
            if (new_changes.find(a) != new_changes.end()) {
                merged.add_change(a, new_changes.at(a));
            } else {
                merged.add_change(a, v);
            }
        }

        for (const auto& [a, v]: new_changes) {
            if (changes.find(a) == changes.end())
                merged.add_change(a, v);
        }

        return merged;
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(PlayerAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case PlayerAttr::READY:
                return std::is_same_v<T, bool>;
            case PlayerAttr::HEALTH:
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
            default:
                return false;
        }
    }
};


// PhaseUpdate type checking
enum class PhaseAttr { PHASE };
typedef std::variant<PhaseType> PhaseValue;


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

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(PhaseAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case PhaseAttr::PHASE:
                return std::is_same_v<T, PhaseType>;
            default:
                return false;
        }
    }
};


// GameUpdate type checking
enum class GameStateAttr { PHASE_TYPE, NUM_ROUNDS, NUM_TTS, NUM_PLAYERS, PLAYERS };
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

    void add_change(GameStateAttr attr, std::map<std::string, PlayerUpdate> value) {
        // Validate attribute-specific type at runtime
        if (attr != GameStateAttr::PLAYERS) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        std::map<std::string, PlayerUpdate> merged;
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

            changes[attr] = merged;
        }
    }

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GameStateAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case GameStateAttr::PHASE_TYPE:
                return std::is_same_v<T, PhaseUpdate>;
            case GameStateAttr::NUM_ROUNDS:
            case GameStateAttr::NUM_TTS:
            case GameStateAttr::NUM_PLAYERS:
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

        PhaseUpdate phase_update;
        phase_update.add_change(PhaseAttr::PHASE, PhaseType::WarmUp);

        GameUpdate game_state_update;
        game_state_update.add_change(GameStateAttr::PHASE_TYPE, phase_update);
        game_state_update.add_change(GameStateAttr::NUM_ROUNDS, 5);
        game_state_update.add_change(GameStateAttr::NUM_TTS, 3);
        game_state_update.add_change(GameStateAttr::NUM_PLAYERS, 10);
        game_state_update.add_change(GameStateAttr::PLAYERS, std::map<std::string, PlayerUpdate>{});
    }
};
