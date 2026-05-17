#pragma once

#include <type_traits>
#include <vector>

// for jlIST
template<typename T>
struct is_vector : std::false_type
{};

template<typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type
{};

template<typename T>
concept isStdVector = is_vector<T>::value;

// This is the "Contract" class which we will specify based on actual json classes (see qjson_backend e.a.)
namespace jsonata
{
    template<typename T, typename Enable = void>
    struct json_bridge_impl;

    template<typename T, typename Enable>
    struct json_bridge_impl {};

    template<typename T>
    using json_bridge =
        json_bridge_impl<std::remove_cvref_t<T>,void>;
}

// for special conversion in getPropertyValueOfType
template<typename T, typename Tag>
struct TaggedProperty {
        using valueType = T;
        using tagType = Tag;
        T& value;
};

// Convenience helpers
struct AsArray {};

template<typename T> auto Array(T& v) { return TaggedProperty<T, AsArray>{v}; }
// use as x.getPropertyValueOfType( "name", Array(destination) );

template<typename T> struct isTaggedProperty : std::false_type {};

template<typename T, typename Tag>
struct isTaggedProperty<TaggedProperty<T, Tag>> : std::true_type {};

template<typename T>
inline constexpr bool isTaggedProperty_v = isTaggedProperty<T>::value;

#include <optional>
#include <type_traits>

// 1. The base case: any type that is NOT a std::optional
template<typename T>
struct isOptional : std::false_type {};

// 2. The specialization: if the type is std::optional<U>, it matches this
template<typename U>
struct isOptional<std::optional<U>> : std::true_type {};

// 3. The helper variable (the "_v" version)
template<typename T>
inline constexpr bool isOptional_v = isOptional<std::decay_t<T>>::value;

//
// given a type T of either std::optional<X> or X, extract X
//

// 1. The "Default" case: If it's not an optional, just return the type itself.
template <typename T>
struct unwrapType {
        using type = T;
};

// 2. The "Special" case: If it IS an optional, extract the inner value_type.
template <typename T>
struct unwrapType<std::optional<T>> {
        using type = T;
};

// Helper alias to keep your code clean
template <typename T>
using unwrapType_t = typename unwrapType<T>::type;


