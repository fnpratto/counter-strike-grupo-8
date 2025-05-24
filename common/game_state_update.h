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

// GunUpdate type checking
enum class GunAttr { BULLETS_PER_MAG, MAG_AMMO, RESERVE_AMMO };
typedef std::variant<int> GunValue;


// InventoryUpdate type checking
enum class InventoryAttr { MONEY, GUNS, UTILITIES };
typedef std::variant<int, GunState> InventoryValue;


// PlayerUpdate type checking
enum class PlayerAttr { TEAM, INVENTORY, HEALTH, POS_X, POS_Y, CURRENT_WEAPON };
typedef std::variant<Team, InventoryState, int, float, WeaponSlot> PlayerValue;


// GameStateUpdate type checking
enum class GameStateAttr { GAME_PHASE, NUM_ROUNDS, NUM_TTS, NUM_PLAYERS, PLAYERS };
typedef std::variant<int, std::map<std::string, PlayerState>> GameStateValue;


template <typename A, typename V>
class StateUpdate {
protected:
    std::map<A, V> changes;

public:
    const std::map<A, V>& get_changes() const { return changes; }
};


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

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(InventoryAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case InventoryAttr::MONEY:
                return std::is_same_v<T, int>;
            case InventoryAttr::GUNS:
                return std::is_same_v<T, GunState>;
            default:
                return false;
        }
    }
};
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

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(PlayerAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case PlayerAttr::HEALTH:
                return std::is_same_v<T, int>;
            case PlayerAttr::TEAM:
                return std::is_same_v<T, Team>;
            case PlayerAttr::INVENTORY:
                return std::is_same_v<T, InventoryState>;
            case PlayerAttr::POS_X:
            case PlayerAttr::POS_Y:
                return std::is_same_v<T, float>;
            case PlayerAttr::CURRENT_WEAPON:
                return std::is_same_v<T, WeaponSlot>;
            default:
                return false;
        }
    }
};
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

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GunAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case GunAttr::BULLETS_PER_MAG:
            case GunAttr::MAG_AMMO:
            case GunAttr::RESERVE_AMMO:
                return std::is_same_v<T, int>;
            default:
                return false;
        }
    }
};

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

protected:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GameStateAttr attr, const T& value) const {
        (void)value;

        switch (attr) {
            case GameStateAttr::GAME_PHASE:
            case GameStateAttr::NUM_ROUNDS:
            case GameStateAttr::NUM_TTS:
            case GameStateAttr::NUM_PLAYERS:
                return std::is_same_v<T, int>;
            case GameStateAttr::PLAYERS:
                return std::is_same_v<T, std::map<std::string, PlayerState>>;
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
        gun_update.add_change(GunAttr::BULLETS_PER_MAG, 30);
        gun_update.add_change(GunAttr::MAG_AMMO, 10);
        gun_update.add_change(GunAttr::RESERVE_AMMO, 90);

        InventoryUpdate inv_update;
        inv_update.add_change(InventoryAttr::MONEY, 1000);
        inv_update.add_change(InventoryAttr::GUNS, GunState{});

        PlayerUpdate player_update;
        player_update.add_change(PlayerAttr::TEAM, Team::TT);
        player_update.add_change(PlayerAttr::INVENTORY, InventoryState{});
        player_update.add_change(PlayerAttr::HEALTH, 100);
        player_update.add_change(PlayerAttr::POS_X, 50.0f);
        player_update.add_change(PlayerAttr::POS_Y, 75.0f);

        GameUpdate game_state_update;
        game_state_update.add_change(GameStateAttr::NUM_ROUNDS, 5);
        game_state_update.add_change(GameStateAttr::NUM_TTS, 3);
        game_state_update.add_change(GameStateAttr::NUM_PLAYERS, 10);
        game_state_update.add_change(GameStateAttr::PLAYERS, std::map<std::string, PlayerState>{});
    }
};
