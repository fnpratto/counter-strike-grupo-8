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
    std::map<A, V> changes;

public:
    // New type-safe method using templates
    template <typename T>
    void add_change(A attr, T value) {
        // Check if T can be stored in variant V
        static_assert(std::is_convertible_v<T, V>,
                      "Value type is not compatible with this StateUpdate's variant type");

        // Validate attribute-specific type at runtime
        if (!is_valid_type_for_attr(attr, value)) {
            throw std::invalid_argument(
                    "Type mismatch: The provided value type is not valid for this attribute");
        }

        changes[attr] = value;
    }

    // To be specialized by derived classes
    template <typename T>
    constexpr bool is_valid_type_for_attr(A attr, const T& value) const {
        (void)attr;
        (void)value;
        return false;  // Base implementation is permissive
    }

    const std::map<A, V>& get_changes() const { return changes; }
};

// GunUpdate type checking
enum class GunAttr { BULLETS_PER_MAG, MAG_AMMO, RESERVE_AMMO };
typedef std::variant<int> GunValue;

class GunUpdate: public StateUpdate<GunAttr, GunValue> {
public:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GunAttr attr, const T& value) const {
        // All GunAttr attributes expect int values
        return std::is_same_v<T, int>;
    }
};

// InventoryUpdate type checking
enum class InventoryAttr { MONEY, GUNS, UTILITIES };
typedef std::variant<int, GunState> InventoryValue;

class InventoryUpdate: public StateUpdate<InventoryAttr, InventoryValue> {
public:
    template <typename T>
    constexpr bool is_valid_type_for_attr(InventoryAttr attr, const T& value) const {
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

// PlayerUpdate type checking
enum class PlayerAttr { TEAM, INVENTORY, HEALTH, POS_X, POS_Y, CURRENT_WEAPON };
typedef std::variant<Team, InventoryState, int, float, WeaponSlot> PlayerValue;

class PlayerUpdate: public StateUpdate<PlayerAttr, PlayerValue> {
public:
    template <typename T>
    constexpr bool is_valid_type_for_attr(PlayerAttr attr, const T& value) const {
        switch (attr) {
            case PlayerAttr::TEAM:
                return std::is_same_v<T, Team>;
            case PlayerAttr::INVENTORY:
                return std::is_same_v<T, InventoryState>;
            case PlayerAttr::HEALTH:
                return std::is_same_v<T, int>;
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

// GameStateUpdate type checking
enum class GameStateAttr { GAME_PHASE, NUM_ROUNDS, NUM_TTS, NUM_PLAYERS, PLAYERS };
typedef std::variant<int, std::map<std::string, PlayerState>> GameStateValue;

class GameStateUpdate: public StateUpdate<GameStateAttr, GameStateValue> {
public:
    template <typename T>
    constexpr bool is_valid_type_for_attr(GameStateAttr attr, const T& value) const {
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

        GameStateUpdate game_state_update;
        game_state_update.add_change(GameStateAttr::NUM_ROUNDS, 5);
        game_state_update.add_change(GameStateAttr::NUM_TTS, 3);
        game_state_update.add_change(GameStateAttr::NUM_PLAYERS, 10);
        game_state_update.add_change(GameStateAttr::PLAYERS, std::map<std::string, PlayerState>{});
    }
};
