#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/models.h"
#include "common/utils/container_type_traits.h"
#include "common/utils/vector_2d.h"
#include "server/clock/clock.h"

#define ATTR(...) __VA_ARGS__

class StateUpdate {
public:
    virtual bool has_change() const = 0;
    virtual void clear() = 0;

    virtual ~StateUpdate() = default;

protected:
    template <typename T>
    std::optional<T> merge(const std::optional<T>& a, const std::optional<T>& b) const {
        if (!a.has_value() && !b.has_value()) {
            return std::nullopt;
        } else if (!a.has_value()) {
            return b;
        } else if (!b.has_value()) {
            return a;
        } else {
            return merge(a.value(), b.value());
        }
    }

    template <typename K, typename V>
    std::map<K, V> merge(const std::map<K, V>& a, const std::map<K, V>& b) const {
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

    template <typename T>
    std::vector<T> merge(const std::vector<T>& a, const std::vector<T>& b) const {
        std::vector<T> merged = a;
        merged.reserve(a.size() + b.size());
        merged.insert(merged.end(), a.begin(), a.end());
        merged.insert(merged.end(), b.begin(), b.end());
        return merged;
    }

    template <typename T>
    typename std::enable_if<std::is_base_of_v<StateUpdate, T>, T>::type merge(const T& a,
                                                                              const T& b) const {
        return a.merged(b);
    }

    template <typename T>
    typename std::enable_if<!is_vector<T>::value && !is_optional<T>::value && !is_map<T>::value &&
                                    !std::is_base_of_v<StateUpdate, T>,
                            T>::type
            merge([[maybe_unused]] const T& a, const T& b) const {
        return b;
    }
};

#define X_DEFINE_MEMBER(type, attr) std::optional<type> attr;
#define M_DEFINE_MEMBER(key_type, value_type, attr) std::map<key_type, value_type> attr;
#define V_DEFINE_MEMBER(type, attr) std::vector<type> attr;

#define X_SETTER(type, attr)                                                \
    void set_##attr(const type& value) {                                    \
        if (attr.has_value()) {                                             \
            attr = merge(attr.value(), value);                              \
        } else {                                                            \
            attr = value;                                                   \
        }                                                                   \
    }                                                                       \
    const std::optional<type>& get_optional_##attr() const { return attr; } \
    const type& get_##attr() const {                                        \
        if (!attr.has_value())                                              \
            throw std::runtime_error(#attr " not set");                     \
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
#define V_SETTER(type, attr)                          \
    void set_##attr(const std::vector<type>& value) { \
        if (!value.empty())                           \
            attr = merge(attr, value);                \
    }                                                 \
    const std::vector<type>& get_##attr() const { return attr; }

#define X_MERGER(type, attr)                                                        \
    auto merged_##attr = merge(get_optional_##attr(), other.get_optional_##attr()); \
    if (merged_##attr.has_value()) {                                                \
        merged.set_##attr(merged_##attr.value());                                   \
    }
#define M_MERGER(key_type, value_type, attr) merged.set_##attr(merge(attr, other.get_##attr()));
#define V_MERGER(type, attr) merged.set_##attr(merge(attr, other.get_##attr()));

#define X_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return attr.has_value(); }
#define M_CHANGED(key_type, value_type, attr) \
    bool has_##attr##_changed() const { return !attr.empty(); }
#define V_CHANGED(type, attr) \
    bool has_##attr##_changed() const { return !attr.empty(); }

#define X_HAS_CHANGE(type, attr) \
    if (attr.has_value())        \
        return true;
#define M_HAS_CHANGE(key_type, value_type, attr) \
    if (!attr.empty())                           \
        return true;
#define V_HAS_CHANGE(type, attr) \
    if (!attr.empty())           \
        return true;

#define X_CLEAR(type, attr) attr.reset();
#define M_CLEAR(key_type, value_type, attr) attr.clear();
#define V_CLEAR(type, attr) attr.clear();

#define DEFINE_UPDATE(CLASS, ATTRS)                                \
    class CLASS: public StateUpdate {                              \
        ATTRS(X_DEFINE_MEMBER, M_DEFINE_MEMBER, V_DEFINE_MEMBER)   \
                                                                   \
    public:                                                        \
        ATTRS(X_SETTER, M_SETTER, V_SETTER)                        \
                                                                   \
        CLASS merged(const CLASS& other) const {                   \
            CLASS merged;                                          \
                                                                   \
            ATTRS(X_MERGER, M_MERGER, V_MERGER)                    \
                                                                   \
            return merged;                                         \
        }                                                          \
                                                                   \
        ATTRS(X_CHANGED, M_CHANGED, V_CHANGED)                     \
                                                                   \
        bool has_change() const override {                         \
            ATTRS(X_HAS_CHANGE, M_HAS_CHANGE, V_HAS_CHANGE)        \
                                                                   \
            return false;                                          \
        }                                                          \
                                                                   \
        void clear() override { ATTRS(X_CLEAR, M_CLEAR, V_CLEAR) } \
    };
