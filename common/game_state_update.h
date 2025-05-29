#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/game_state.h"
#include "server/clock/clock.h"

#define ATTR(...) __VA_ARGS__

class StateUpdate {
public:
    virtual bool has_change() const = 0;
    virtual void clear() = 0;

    virtual ~StateUpdate() = default;

protected:
    template <typename T>
    T merge([[maybe_unused]] const T& a, const T& b) const {
        return b;
    }

    template <typename T>
    std::optional<T> merge_optional(const std::optional<T>& a, const std::optional<T>& b) const {
        if (b.has_value()) {
            return b;
        }
        return a;
    }

    template <typename K, typename V>
    std::map<K, V> merge_map(const std::map<K, V>& a, const std::map<K, V>& b) const {
        std::map<K, V> merged = a;

        for (const auto& [key, value]: b) {
            if (merged.find(key) == merged.end()) {
                merged[key] = value;
            } else {
                merged[key] = merge(merged[key], value);
            }
        }

        return merged;
    }

    template <typename K, typename V>
    std::map<K, std::optional<V>> merge_optional_map(const std::map<K, std::optional<V>>& a,
                                                     const std::map<K, std::optional<V>>& b) const {
        std::map<K, std::optional<V>> merged = a;

        for (const auto& [key, value]: b) {
            if (merged.find(key) == merged.end()) {
                merged[key] = value;
            } else {
                merged[key] = merge_optional(merged[key], value);
            }
        }

        return merged;
    }
};


#define X_DEFINE_MEMBER(type, attr) std::optional<type> attr;
#define M_DEFINE_MEMBER(type, attr) type attr;
#define U_DEFINE_MEMBER(type, attr) type attr;


#define X_SETTER(type, attr)                                                \
    void set_##attr(const type& value) { attr = value; }                    \
    const std::optional<type>& get_optional_##attr() const { return attr; } \
    const type& get_##attr() const {                                        \
        if (!attr.has_value())                                              \
            throw std::runtime_error("Error: " #attr " not set");           \
        return attr.value();                                                \
    }
#define M_SETTER(type, attr)               \
    void set_##attr(const type& value) {   \
        if (!value.empty())                \
            attr = merge_map(attr, value); \
    }                                      \
    const type& get_##attr() const { return attr; }
#define U_SETTER(type, attr)                                          \
    void set_##attr(const type& value) { attr = attr.merged(value); } \
    const type& get_##attr() const { return attr; }


#define X_MERGER(type, attr)                                                                 \
    auto merged_##attr = merge_optional(get_optional_##attr(), other.get_optional_##attr()); \
    if (merged_##attr.has_value()) {                                                         \
        merged.set_##attr(merged_##attr.value());                                            \
    }
#define M_MERGER(type, attr) merged.set_##attr(merge_map(attr, other.get_##attr()));
#define U_MERGER(type, attr) merged.set_##attr(attr.merged(other.get_##attr()));

#define X_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return attr.has_value(); }
#define M_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return !attr.empty(); }
#define U_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return attr.has_change(); }

#define X_HAS_CHANGE(type, attr) \
    if (attr.has_value())        \
        return true;
#define M_HAS_CHANGE(type, attr) \
    if (!attr.empty())           \
        return true;
#define U_HAS_CHANGE(type, attr) \
    if (attr.has_change())       \
        return true;


#define X_CLEAR(type, attr) attr.reset();
#define M_CLEAR(type, attr) attr.clear();
#define U_CLEAR(type, attr) attr.clear();

#define DEFINE_UPDATE(CLASS, ATTRS)                                \
    class CLASS: public StateUpdate {                              \
        ATTRS(X_DEFINE_MEMBER, M_DEFINE_MEMBER, U_DEFINE_MEMBER)   \
                                                                   \
    public:                                                        \
        ATTRS(X_SETTER, M_SETTER, U_SETTER)                        \
                                                                   \
        CLASS merged(const CLASS& other) const {                   \
            CLASS merged;                                          \
                                                                   \
            ATTRS(X_MERGER, M_MERGER, U_MERGER)                    \
                                                                   \
            return merged;                                         \
        }                                                          \
                                                                   \
        ATTRS(X_CHANGED, M_CHANGED, U_CHANGED)                     \
                                                                   \
        bool has_change() const override {                         \
            ATTRS(X_HAS_CHANGE, M_HAS_CHANGE, U_HAS_CHANGE)        \
                                                                   \
            return false;                                          \
        }                                                          \
                                                                   \
        void clear() override { ATTRS(X_CLEAR, M_CLEAR, U_CLEAR) } \
    };


#define GUN_ATTRS(X, M, U)  \
    X(GunType, gun)         \
    X(int, bullets_per_mag) \
    X(int, mag_ammo)        \
    X(int, reserve_ammo)

DEFINE_UPDATE(GunUpdate, GUN_ATTRS)

#define UTILITY_ATTRS(X, M, U) X(UtilityType, utility)

DEFINE_UPDATE(UtilityUpdate, UTILITY_ATTRS)

#define INVENTORY_ATTRS(X, M, U)                   \
    M(ATTR(std::map<WeaponSlot, GunUpdate>), guns) \
    M(ATTR(std::map<WeaponSlot, UtilityUpdate>), utilities)

DEFINE_UPDATE(InventoryUpdate, INVENTORY_ATTRS)

#define PLAYER_ATTRS(X, M, U)     \
    X(Team, team)                 \
    X(float, pos_x)               \
    X(float, pos_y)               \
    X(float, aim_x)               \
    X(float, aim_y)               \
    X(bool, ready)                \
    X(int, health)                \
    X(int, money)                 \
    X(WeaponSlot, current_weapon) \
    U(InventoryUpdate, inventory) \
    X(bool, is_moving)            \
    X(int, move_dx)               \
    X(int, move_dy)

DEFINE_UPDATE(PlayerUpdate, PLAYER_ATTRS)

#define PHASE_ATTRS(X, M, U) \
    X(PhaseType, phase)      \
    X(TimePoint, time)

DEFINE_UPDATE(PhaseUpdate, PHASE_ATTRS)

#define GAME_ATTRS(X, M, U) \
    U(PhaseUpdate, phase)   \
    X(int, num_rounds)      \
    X(int, num_tts)         \
    X(int, num_cts)         \
    M(ATTR(std::map<std::string, PlayerUpdate>), players)
DEFINE_UPDATE(GameUpdate, GAME_ATTRS)
