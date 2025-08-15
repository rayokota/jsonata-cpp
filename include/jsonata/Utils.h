/**
 * jsonata-cpp is the JSONata C++ reference port
 *
 * Copyright Dashjoin GmbH. https://dashjoin.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <any>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace jsonata {

// Forward declarations
class JFunction;
class JFunctionCallable;

class Utils {
  public:
    // Sentinel type to represent a JSON null literal (distinct from undefined)
    class NullValue {};

    // Unified list type that can represent either a regular list or a range
    // (declared first for use in function signatures)
    class JList : public std::vector<std::any> {
      private:
        // Range-specific members
        int64_t range_start_ = 0;
        int64_t range_end_ = 0;
        bool is_range_ = false;

      public:
        // Regular list constructors
        JList() : std::vector<std::any>() {}
        JList(size_t capacity) : std::vector<std::any>() {
            this->reserve(capacity);
        }
        JList(const std::vector<std::any>& other)
            : std::vector<std::any>(other) {}
        JList(const JList& other);

        // Add initializer list constructor
        JList(std::initializer_list<std::any> init)
            : std::vector<std::any>(init),
              range_start_(0),
              range_end_(0),
              is_range_(false) {}

        // Range constructor
        JList(int64_t start, int64_t end);

        // JSONata specific flags
        bool sequence = false;
        bool outerWrapper = false;
        bool tupleStream = false;
        bool keepSingleton = false;
        bool cons = false;

        // Range detection
        bool isRange() const { return is_range_; }

        // Range-specific methods
        int64_t getRangeStart() const { return range_start_; }
        int64_t getRangeEnd() const { return range_end_; }

        // Override size() for ranges
        size_t size() const;

        // Override operator[] for ranges (const)
        std::any operator[](size_t index) const;

        // Provide non-const operator[] so assignments work; materialize ranges
        // on demand
        std::any& operator[](size_t index);

        // Override at() for ranges (const)
        std::any at(size_t index) const;

        // Provide non-const at() so assignments work; materialize ranges on
        // demand
        std::any& at(size_t index);

        // Override empty() for ranges
        bool empty() const;

        // Range iterator support
        class range_iterator {
          private:
            int64_t current_;

          public:
            range_iterator(int64_t val) : current_(val) {}
            std::any operator*() const;
            range_iterator& operator++();
            bool operator!=(const range_iterator& other) const;
            bool operator==(const range_iterator& other) const;
        };

        // For ranges, we need special iterator handling
        range_iterator range_begin() const;
        range_iterator range_end() const;

        // Simple solution: Override begin/end to materialize ranges when needed
        // for iteration This is simpler than complex custom iterators and
        // ensures compatibility
        using base_iterator = typename std::vector<std::any>::iterator;
        using base_const_iterator =
            typename std::vector<std::any>::const_iterator;

        // Override begin/end to handle ranges by materializing if needed
        base_iterator begin();
        base_iterator end();
        base_const_iterator begin() const;
        base_const_iterator end() const;
        base_const_iterator cbegin() const;
        base_const_iterator cend() const;

      private:
        // If this list represents a range, materialize it into a concrete
        // vector
        void materializeRangeIfNeeded();
    };

    // Type alias for backward compatibility
    using RangeList = JList;

    // Type checking utilities
    static bool isNumeric(const std::any& v);
    static bool isIntegral(const std::any& v);
    static bool isArrayOfStrings(const std::any& v);
    static bool isArrayOfNumbers(const std::any& v);
    static bool isFunction(const std::any& o);

    // Numeric coercions
    static int64_t toLong(const std::any& v);
    static double toDouble(const std::any& v);

    // General JSONata type checks (moved from Functions)
    static bool isPrimitive(const std::any& value);
    static bool isNumber(const std::any& value);
    static bool isString(const std::any& value);
    static bool isArray(const std::any& value);
    static bool isObject(const std::any& value);
    static bool isBoolean(const std::any& value);
    static bool isNull(const std::any& value);
    static bool isNullValue(const std::any& value);
    static std::optional<std::string> type(const std::any& value);

    // Sequence and list utilities
    static JList createSequence();
    static JList createSequence(const std::any& el);

    // Utility functions
    static bool isSequence(const std::any& result);
    static std::any convertNumber(const std::any& n);
    static JList arrayify(const std::any& value);
    static void checkUrl(const std::string& str);
    static std::any convertValue(const std::any& val);
    static std::any convertNulls(const std::any& res);
    static void quote(const std::string& string, std::ostringstream& w);

    // Special values
    static const std::any NONE;
    static const std::any NULL_VALUE;

  private:
    static void convertNullsMap(std::any& res);
    static void convertNullsList(std::any& res);
    static void recurse(std::any& val);
};

}  // namespace jsonata
