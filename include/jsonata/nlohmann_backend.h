#pragma once

#include "Utils.h"
#include <concepts>

#include "common_backend.h"

#include <nlohmann/json.hpp>

#include <nlohmann/json.hpp>
#include <type_traits>
#include <concepts>

template <typename T>
concept isNlohmann = requires(T j) {
        typename T::value_type;
        { j.is_object() } -> std::same_as<bool>;
        { j.is_array() } -> std::same_as<bool>;
        // This is the "fingerprint" of a nlohmann-like container
        T::array();
    };

namespace jsonata
{
// Single bridge specialization
template<typename T>
    requires isNlohmann<T>
struct json_bridge_impl<T,void>
{
        using BaseT = std::remove_cvref_t<T>;
        using is_json_bridge_type = void; // Tag to satisfy the concept
        // all nlohmann have json a sorted version
        using sortedPartner = nlohmann::json;

        static BaseT create(auto&& value) {
            using V = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<V, std::nullptr_t>) {
                return BaseT();
            } else if constexpr (std::is_constructible_v<T, V>) {
                return value;
            } else {
                static_assert(false, "Unsupported type for nlohmann creation");
            }
        }

        static BaseT create() { return BaseT(); }

        static size_t size(const BaseT &value) { return value.size(); }

        static bool isEmpty(const BaseT &value) { return value.empty(); }

        static bool isObject(const BaseT &value) { return value.is_object(); }

        static bool isArray(const BaseT &value) { return value.is_array(); }

        static bool isNumber(const BaseT &value) { return value.is_number(); }

        static bool isInteger(const BaseT &value) { return value.is_number_integer(); }

        static bool isFloat(const BaseT &value) { return value.is_number_float(); }

        static bool isUnsignedInteger(const BaseT &value) { return value.is_number_unsigned(); }

        static bool isBool(const BaseT &value) { return value.is_boolean(); }

        static bool isString(const BaseT &value) { return value.is_string(); }

        static bool isNull(const BaseT &value) { return value.is_null(); }

        static bool EQ(const BaseT &v1,const BaseT & v2) { return v1 == v2; }

        static bool contains(const BaseT &value, const std::string &k) { return value.contains(k); }

        static decltype(auto) at(const BaseT &value, const std::string &k) { return value[k]; }

        static decltype(auto) at(const BaseT &value, int k) { return value[k]; }

        template<typename outType>
        static outType get(const BaseT &value)
        {
            return value.template get<outType>();
        }

        static BaseT parse(const std::string &s) { return T::parse(s); }

        static BaseT array() { return BaseT::array(); }

        static BaseT object() { return BaseT::object(); }

        static void pushBack(BaseT &val, auto value) { val.push_back(value); }

        template<typename action>
        static void mutateForAll(BaseT &from, action a)
        {
            if (from.is_array()) {
                int idx = 0;
                for (const auto &el : from) {
                    a(std::to_string(idx++), el);
                }
            } else {
                for (const auto &[k, v] : from) {
                    a(k, v);
                }
            }
            return;
        }

        template<typename action>
        static void forAll(const BaseT &from, action a)
        {
            using fromType = std::decay_t<decltype(from)>;
            if constexpr (std::is_same_v<
                              fromType,
                              std::initializer_list<std::pair<std::string, nlohmann::ordered_json>>>
                          || std::is_same_v<
                              fromType,
                              std::initializer_list<std::pair<std::string, nlohmann::json>>>) {
                for (const auto &[k, v] : from) {
                    action(k, v);
                }
            } else if constexpr (std::is_same_v<fromType,
                                                std::initializer_list<nlohmann::ordered_json>>
                                 || std::is_same_v<fromType, std::initializer_list<nlohmann::json>>) {
                int idx = 0;
                for (const auto &el : from) {
                    action(std::to_string(idx++), el);
                }
            } else if (from.is_array()) {
                int idx = 0;
                for (const auto &el : from) {
                    a(std::to_string(idx++), el);
                }
            } else {
                for (const auto &[k, v] : from.items()) {
                    a(k, v);
                }
            }
        }

        template<typename keyType, typename valueType>
        static void appendValue(BaseT &to, const keyType &k, const valueType &v)
        {
            if (to.is_array()) {
                to.push_back(v);
            } else {
                to[k] = v;
            }
        }

        template<typename keyType, typename valueType>
        static void set(BaseT &to, const keyType &k, const valueType &v)
        {
            to[k] = v;
        }

        static std::string dump(const BaseT &v) { return v.dump(); }

        static bool getPropertyValueOfType(const BaseT &root,
                                    const std::string &propertyName,
                                    auto &propertyValue)
        {
            using propertyType = std::decay_t<decltype(propertyValue)>;
            auto convertor = [&](auto E, auto &Prop) {
                using EType = std::decay_t<decltype(E)>; // This is the type of E

                if constexpr ( isTaggedProperty_v<propertyType> ) {
                    // special conversion -> Array
                    if (E.is_array()) {
                        propertyValue.value = E;
                        return true;
                    }
                    return false;
                } else if constexpr (std::is_base_of_v<std::string, propertyType>) {
                    // property must be string
                    if (E.is_string()) {
                        Prop = E.template get<std::string>();
                        return true;
                    }
                } else if constexpr (std::is_same_v<propertyType, bool>) {
                    if (E.is_boolean()) {
                        Prop = E.template get<bool>();
                        return true;
                    }
                } else if constexpr (isNlohmann<propertyType>) {
                    Prop = E;
                    return true;
                } else {
                    // This triggers ONLY if none of the above branches are taken
                    static_assert(sizeof(propertyType) == 0, "cannot get unknown propertytype");
                }

                return false;
            };

            auto overrideIt = root.find(propertyName);
            return (overrideIt != root.end()) ? convertor((*overrideIt), propertyValue) : false;
        }

        // static bool getPropertyValueOfType(const BaseT &root,
        //                                    const std::string &propertyName,
        //                                    TaggedProperty<T,AsArray> propertyValue)
        // {
        //     auto convertor = [&](auto E, TaggedProperty<T,AsArray> Prop) {
        //         using EType = std::decay_t<decltype(E)>; // This is the type of E
        //         if (E.is_array()) {
        //             Prop.value = E;
        //             return true;
        //         }
        //         return false;
        //     };

        //     auto overrideIt = root.find(propertyName);
        //     return (overrideIt != root.end()) ? convertor((*overrideIt), propertyValue) : false;;
        // }
};
}