#pragma once

#include <map>
#include <optional>
#include <type_traits>
#include <vector>

// Type traits to identify specific containers
template <typename T>
struct is_vector: std::false_type {};

template <typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>>: std::true_type {};

template <typename T>
struct is_map: std::false_type {};

template <typename Key, typename Value, typename Compare, typename Alloc>
struct is_map<std::map<Key, Value, Compare, Alloc>>: std::true_type {};

template <typename T>
struct is_optional: std::false_type {};

template <typename T>
struct is_optional<std::optional<T>>: std::true_type {};
