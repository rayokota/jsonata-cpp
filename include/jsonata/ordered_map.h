// From nlohmann/json adapted for jsonata
// Refactored to Modern C++23

#pragma once

#include <functional>       // equal_to, less
#include <initializer_list> // initializer_list
#include <iterator>         // input_iterator_tag, distance, next, prev
#include <memory>           // allocator
#include <type_traits>      // remove_cvref_t, is_same_v, is_convertible_v
#include <utility>          // pair, move, forward
#include <vector>           // vector
#include "JException.h"

namespace jsonata {

    // C++20/23 Concept checking if Compare functor can compare types A and B interchangeably
    template<typename Compare, typename A, typename B>
    concept ComparableWith = requires(Compare comp, A&& a, B&& b) {
        { comp(std::forward<A>(a), std::forward<B>(b)) };
        { comp(std::forward<B>(b), std::forward<A>(a)) };
    };

    // Simplified C++23 trait evaluating if a generic type can be safely used as a lookup key
    template<typename Comparator, typename ObjectKeyType, typename KeyTypeCVRef,
             bool RequireTransparentComparator = true, bool ExcludeObjectKeyType = RequireTransparentComparator>
    struct is_usable_as_key_type {
            using KeyType = std::remove_cvref_t<KeyTypeCVRef>;

            // Mocking nlohmann's internally expected detection traits
            // (If using nlohmann framework directly, adjust detect_is_transparent checks as needed)
            static constexpr bool is_transparent = requires { typename Comparator::is_transparent; };

            static constexpr bool value =
                ComparableWith<Comparator, ObjectKeyType, KeyTypeCVRef> &&
                !(ExcludeObjectKeyType && std::is_same_v<KeyType, ObjectKeyType>) &&
                (!RequireTransparentComparator || is_transparent);
    };

    /// ordered_map: a minimal map-like container that preserves insertion order
    template <class Key, class T, class IgnoredLess = std::less<Key>,
             class Allocator = std::allocator<std::pair<const Key, T>>>
    struct ordered_map : std::vector<std::pair<const Key, T>, Allocator>
    {
            using key_type = Key;
            using mapped_type = T;
            using Container = std::vector<std::pair<const Key, T>, Allocator>;
            using iterator = typename Container::iterator;
            using const_iterator = typename Container::const_iterator;
            using size_type = typename Container::size_type;
            using value_type = typename Container::value_type;
            using key_compare = std::equal_to<Key>;

            // Default constructors
            ordered_map() noexcept(noexcept(Container())) : Container{} {}
            explicit ordered_map(const Allocator& alloc) noexcept(noexcept(Container(alloc))) : Container{alloc} {}

            template <class It>
            ordered_map(It first, It last, const Allocator& alloc = Allocator())
                : Container{first, last, alloc} {}

            ordered_map(std::initializer_list<value_type> init, const Allocator& alloc = Allocator())
                : Container{init, alloc} {}

            ordered_map(const ordered_map&) = default;
            ordered_map(ordered_map&&) noexcept(std::is_nothrow_move_constructible_v<Container>) = default;
            ~ordered_map() = default;

            ordered_map& operator=(const ordered_map& other)
            {
                if (this != &other)
                {
                    ordered_map tmp(other);
                    Container::operator=(std::move(static_cast<Container&>(tmp)));
                }
                return *this;
            }

            ordered_map& operator=(ordered_map&& other) noexcept(std::is_nothrow_move_assignable_v<Container>)
            {
                Container::operator=(std::move(static_cast<Container&>(other)));
                return *this;
            }

            // Standard Emplace
            std::pair<iterator, bool> emplace(const key_type& key, T&& t)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return {it, false};
                    }
                }
                Container::emplace_back(key, std::forward<T>(t));
                return {std::prev(this->end()), true};
            }

            // C++23 Heterogeneous Emplace using a 'requires' clause constraint
            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            std::pair<iterator, bool> emplace(KeyType&& key, T&& t)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return {it, false};
                    }
                }
                Container::emplace_back(std::forward<KeyType>(key), std::forward<T>(t));
                return {std::prev(this->end()), true};
            }

            // Element Access Operators
            T& operator[](const key_type& key)
            {
                return emplace(key, T{}).first->second;
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            T& operator[](KeyType&& key)
            {
                return emplace(std::forward<KeyType>(key), T{}).first->second;
            }

            const T& operator[](const key_type& key) const
            {
                return at(key);
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            const T& operator[](KeyType&& key) const
            {
                return at(std::forward<KeyType>(key));
            }

            // At Methods
            T& at(const key_type& key)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it->second;
                    }
                }
                throw JException("key not found");
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            T& at(KeyType&& key)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it->second;
                    }
                }
                throw JException("key not found");
            }

            const T& at(const key_type& key) const
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it->second;
                    }
                }
                throw JException("key not found");
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            const T& at(KeyType&& key) const
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it->second;
                    }
                }
                throw JException("key not found");
            }

            // Erase Operations
            size_type erase(const key_type& key)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        for (auto next = it; ++next != this->end(); ++it)
                        {
                            it->~value_type();
                            new (&*it) value_type{std::move(*next)};
                        }
                        Container::pop_back();
                        return 1;
                    }
                }
                return 0;
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            size_type erase(KeyType&& key)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        for (auto next = it; ++next != this->end(); ++it)
                        {
                            it->~value_type();
                            new (&*it) value_type{std::move(*next)};
                        }
                        Container::pop_back();
                        return 1;
                    }
                }
                return 0;
            }

            iterator erase(iterator pos)
            {
                return erase(pos, std::next(pos));
            }

            iterator erase(iterator first, iterator last)
            {
                if (first == last)
                {
                    return first;
                }

                const auto elements_affected = std::distance(first, last);
                const auto offset = std::distance(Container::begin(), first);

                for (auto it = first; std::next(it, elements_affected) != Container::end(); ++it)
                {
                    it->~value_type();
                    new (&*it) value_type{std::move(*std::next(it, elements_affected))};
                }

                Container::resize(this->size() - static_cast<size_type>(elements_affected));
                return Container::begin() + offset;
            }

            // Count and Find Lookup Operations
            size_type count(const key_type& key) const
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return 1;
                    }
                }
                return 0;
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            size_type count(KeyType&& key) const
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return 1;
                    }
                }
                return 0;
            }

            iterator find(const key_type& key)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it;
                    }
                }
                return Container::end();
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            iterator find(KeyType&& key)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it;
                    }
                }
                return Container::end();
            }

            const_iterator find(const key_type& key) const
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it;
                    }
                }
                return Container::end();
            }

            template<class KeyType>
                requires (is_usable_as_key_type<key_compare, key_type, KeyType>::value)
            const_iterator find(KeyType&& key) const
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, key))
                    {
                        return it;
                    }
                }
                return Container::end();
            }

            // Insert Operations
            std::pair<iterator, bool> insert(value_type&& value)
            {
                return emplace(value.first, std::move(value.second));
            }

            std::pair<iterator, bool> insert(const value_type& value)
            {
                for (auto it = this->begin(); it != this->end(); ++it)
                {
                    if (m_compare(it->first, value.first))
                    {
                        return {it, false};
                    }
                }
                Container::push_back(value);
                return {--this->end(), true};
            }

            // C++20/23 Concept constrained range input insertion instead of require_input_iter traits
            template<typename InputIt>
                requires std::is_convertible_v<typename std::iterator_traits<InputIt>::iterator_category, std::input_iterator_tag>
            void insert(InputIt first, InputIt last)
            {
                for (auto it = first; it != last; ++it)
                {
                    insert(*it);
                }
            }

        private:
            key_compare m_compare = key_compare();
    };

} // namespace jsonata