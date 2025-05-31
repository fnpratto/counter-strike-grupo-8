#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

#include "common/models.h"
#include "server/clock/clock.h"
#include "server/utils/vector_2d.h"

#define ATTR(...) __VA_ARGS__

class StateUpdate {
public:
    virtual bool has_change() const = 0;
    virtual void clear() = 0;

    virtual ~StateUpdate() = default;

protected:
    template <typename T>
    std::optional<T> merge(const std::optional<T>& a, const std::optional<T>& b) const {
        if (b.has_value()) {
            return b;
        }
        return a;
    }

    template <typename K, typename V>
    std::map<K, V> merge(const std::map<K, V>& a, const std::map<K, V>& b) const {
        std::map<K, V> merged = a;

        for (const auto& [key, value]: b) {
            if (merged.find(key) == merged.end()) {
                merged[key] = value;
            } else {
                if constexpr (std::is_base_of_v<StateUpdate, V>) {
                    merged[key] = merged[key].merged(value);
                } else {
                    merged[key] = value;  // For simple types, just overwrite
                }
            }
        }

        return merged;
    }
};

#define X_DEFINE_MEMBER(type, attr) std::optional<type> attr;
#define M_DEFINE_MEMBER(key_type, value_type, attr) std::map<key_type, value_type> attr;
#define U_DEFINE_MEMBER(type, attr) type attr;

#define X_SETTER(type, attr)                                                \
    void set_##attr(const type& value) { attr = value; }                    \
    const std::optional<type>& get_optional_##attr() const { return attr; } \
    const type& get_##attr() const {                                        \
        if (!attr.has_value())                                              \
            throw std::runtime_error("Error: " #attr " not set");           \
        return attr.value();                                                \
    }
#define M_SETTER(key_type, value_type, attr)                                 \
    void set_##attr(const std::map<key_type, value_type>& value) {           \
        if (!value.empty())                                                  \
            attr = merge(attr, value);                                       \
    }                                                                        \
    void add_##attr##_change(const key_type& key, const value_type& value) { \
        if (value.has_change())                                              \
            attr[key] = value;                                               \
    }                                                                        \
    const std::map<key_type, value_type>& get_##attr() const { return attr; }
#define U_SETTER(type, attr)                                          \
    void set_##attr(const type& value) { attr = attr.merged(value); } \
    const type& get_##attr() const { return attr; }


#define X_MERGER(type, attr)                                                        \
    auto merged_##attr = merge(get_optional_##attr(), other.get_optional_##attr()); \
    if (merged_##attr.has_value()) {                                                \
        merged.set_##attr(merged_##attr.value());                                   \
    }
#define M_MERGER(key_type, value_type, attr) merged.set_##attr(merge(attr, other.get_##attr()));
#define U_MERGER(type, attr) merged.set_##attr(attr.merged(other.get_##attr()));

#define X_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return attr.has_value(); }
#define M_CHANGED(key_type, value_type, attr) \
    bool has_##attr##_changed() const { return !attr.empty(); }
#define U_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return attr.has_change(); }

#define X_HAS_CHANGE(type, attr) \
    if (attr.has_value())        \
        return true;
#define M_HAS_CHANGE(key_type, value_type, attr) \
    if (!attr.empty())                           \
        return true;
#define U_HAS_CHANGE(type, attr) \
    if (attr.has_change())       \
        return true;


#define X_CLEAR(type, attr) attr.reset();
#define M_CLEAR(key_type, value_type, attr) attr.clear();
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
