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
#include "jsonata/Utils.h"

#include <cmath>
#include <limits>

#include "jsonata/Functions.h"  // For Functions::isLambda in Utils::type
#include "jsonata/JException.h"
#include "jsonata/Jsonata.h"  // For JFunction definition

namespace jsonata {

// Static member definitions
// Use a unique sentinel for NONE to distinguish it from a real null/undefined
// input
namespace {
// Address of this variable serves as a unique process-wide sentinel token
static int64_t JSONATA_NONE_SENTINEL_VAR = 0;
}  // namespace
const std::any Utils::NONE =
    reinterpret_cast<void*>(&JSONATA_NONE_SENTINEL_VAR);
const std::any Utils::NULL_VALUE = Utils::NullValue{};

// JFunction is now included from Jsonata.h

bool Utils::isNumeric(const std::any& v) {
    if (!v.has_value()) return false;

    try {
        // Support common integral types across platforms
        if (v.type() == typeid(int64_t) || v.type() == typeid(uint64_t) ||
            v.type() == typeid(long long) || v.type() == typeid(unsigned long long) ||
            v.type() == typeid(long) || v.type() == typeid(unsigned long) ||
            v.type() == typeid(int) || v.type() == typeid(unsigned int)) {
            return true;
        }

        if (v.type() == typeid(double)) {
            double d = std::any_cast<double>(v);
            bool isNum = !std::isnan(d);
            if (isNum && !std::isfinite(d)) {
                throw JException("D1001", 0, v);
            }
            return isNum;
        }
    } catch (const std::bad_any_cast&) {
        return false;
    }

    return false;
}

bool Utils::isIntegral(const std::any& v) {
    if (!v.has_value()) return false;

    // Only accept integer types (signed/unsigned), not double or float (matches
    // Java Long/Integer check)
    return v.type() == typeid(int64_t) || v.type() == typeid(uint64_t) ||
           v.type() == typeid(long long) || v.type() == typeid(unsigned long long) ||
           v.type() == typeid(long) || v.type() == typeid(unsigned long) ||
           v.type() == typeid(int) || v.type() == typeid(unsigned int);
}

int64_t Utils::toLong(const std::any& v) {
    if (!v.has_value()) {
        throw JException("D1001", 0, v);
    }
    if (v.type() == typeid(int64_t))
        return std::any_cast<int64_t>(v);
    if (v.type() == typeid(uint64_t))
        return static_cast<int64_t>(std::any_cast<uint64_t>(v));
    if (v.type() == typeid(long long))
        return static_cast<int64_t>(std::any_cast<long long>(v));
    if (v.type() == typeid(unsigned long long))
        return static_cast<int64_t>(std::any_cast<unsigned long long>(v));
    if (v.type() == typeid(long))
        return static_cast<int64_t>(std::any_cast<long>(v));
    if (v.type() == typeid(unsigned long))
        return static_cast<int64_t>(std::any_cast<unsigned long>(v));
    if (v.type() == typeid(int))
        return static_cast<int64_t>(std::any_cast<int>(v));
    if (v.type() == typeid(unsigned int))
        return static_cast<int64_t>(std::any_cast<unsigned int>(v));
    if (v.type() == typeid(double))
        return static_cast<int64_t>(std::any_cast<double>(v));
    // Not a numeric type we know how to coerce
    throw JException("D1001", 0, v);
}

double Utils::toDouble(const std::any& v) {
    if (!v.has_value()) {
        throw JException("D1001", 0, v);
    }
    if (v.type() == typeid(double)) return std::any_cast<double>(v);
    if (v.type() == typeid(int64_t))
        return static_cast<double>(std::any_cast<int64_t>(v));
    if (v.type() == typeid(uint64_t))
        return static_cast<double>(std::any_cast<uint64_t>(v));
    if (v.type() == typeid(long long))
        return static_cast<double>(std::any_cast<long long>(v));
    if (v.type() == typeid(unsigned long long))
        return static_cast<double>(std::any_cast<unsigned long long>(v));
    if (v.type() == typeid(long))
        return static_cast<double>(std::any_cast<long>(v));
    if (v.type() == typeid(unsigned long))
        return static_cast<double>(std::any_cast<unsigned long>(v));
    if (v.type() == typeid(int))
        return static_cast<double>(std::any_cast<int>(v));
    if (v.type() == typeid(unsigned int))
        return static_cast<double>(std::any_cast<unsigned int>(v));
    // Not a numeric type we know how to coerce
    throw JException("D1001", 0, v);
}

bool Utils::isPrimitive(const std::any& value) {
    return value.has_value() &&
           (value.type() == typeid(bool) ||
            value.type() == typeid(std::string) ||
            value.type() == typeid(double) ||
            value.type() == typeid(int64_t) ||
            value.type() == typeid(uint64_t));
}

bool Utils::isNumber(const std::any& value) {
    return value.has_value() &&
           (value.type() == typeid(double) ||
            value.type() == typeid(int64_t) || value.type() == typeid(uint64_t) ||
            value.type() == typeid(long long) || value.type() == typeid(unsigned long long) ||
            value.type() == typeid(long) || value.type() == typeid(unsigned long) ||
            value.type() == typeid(int) || value.type() == typeid(unsigned int));
}

bool Utils::isString(const std::any& value) {
    return value.has_value() && value.type() == typeid(std::string);
}

bool Utils::isBoolean(const std::any& value) {
    return value.has_value() && value.type() == typeid(bool);
}

bool Utils::isNull(const std::any& value) { return !value.has_value(); }

bool Utils::isNullValue(const std::any& value) {
    if (!value.has_value()) return false;
    return value.type() == typeid(Utils::NullValue);
}

bool Utils::isArray(const std::any& value) {
    if (!value.has_value()) return false;
    // Java reference: if(value instanceof List) return "array"
    // Check for all array-like types in C++
    return value.type() == typeid(Utils::JList) ||
           value.type() == typeid(std::vector<std::any>);
}

bool Utils::isObject(const std::any& value) {
    if (!value.has_value()) return false;
    return value.type() == typeid(nlohmann::ordered_map<std::string, std::any>);
}

std::optional<std::string> Utils::type(const std::any& value) {
    if (!value.has_value()) {
        return "null";
    } else if (isNullValue(value)) {
        return "null";
    } else if (isNumber(value)) {
        return "number";
    } else if (isNullValue(value)) {
        return "null";
    } else if (isString(value)) {
        return "string";
    } else if (isBoolean(value)) {
        return "boolean";
    } else if (isArray(value)) {
        return "array";
    } else if (isFunction(value) || Functions::isLambda(value)) {
        return "function";
    } else if (isObject(value)) {
        return "object";
    } else {
        return "object";  // Default to "object" like Java instead of "unknown"
    }
}

bool Utils::isArrayOfStrings(const std::any& v) {
    if (!v.has_value()) return false;

    // Check for JList first
    if (v.type() == typeid(Utils::JList)) {
        auto vec = arrayify(v);
        for (const auto& item : vec) {
            if (!item.has_value() || item.type() != typeid(std::string)) {
                return false;
            }
        }
        return true;
    }

    // Also check for std::vector<std::any> for backward compatibility (tests,
    // JSON parsing)
    if (v.type() == typeid(std::vector<std::any>)) {
        auto vec = std::any_cast<std::vector<std::any>>(v);
        for (const auto& item : vec) {
            if (!item.has_value() || item.type() != typeid(std::string)) {
                return false;
            }
        }
        return true;
    }

    return false;
}

bool Utils::isArrayOfNumbers(const std::any& v) {
    if (!v.has_value()) return false;

    // Check for JList first
    if (v.type() == typeid(Utils::JList)) {
        auto vec = arrayify(v);
        for (const auto& item : vec) {
            if (!isNumeric(item)) {
                return false;
            }
        }
        return true;
    }

    // Also check for std::vector<std::any> for backward compatibility (tests,
    // JSON parsing)
    if (v.type() == typeid(std::vector<std::any>)) {
        auto vec = std::any_cast<std::vector<std::any>>(v);
        for (const auto& item : vec) {
            if (!isNumeric(item)) {
                return false;
            }
        }
        return true;
    }

    return false;
}

bool Utils::isFunction(const std::any& o) {
    if (!o.has_value()) return false;

    try {
        // Check for function types
        if (o.type() == typeid(std::function<std::any()>) ||
            o.type() ==
                typeid(std::function<std::any(const std::vector<std::any>&)>) ||
            o.type() == typeid(JFunction)) {
            return true;
        }
    } catch (const std::bad_any_cast&) {
        // Ignore
    }

    return false;
}

Utils::JList Utils::createSequence() { return createSequence(NONE); }

Utils::JList Utils::createSequence(const std::any& el) {
    JList sequence;
    sequence.sequence = true;

    // Detect the special NONE sentinel and do not add it to the sequence
    bool isNoneSentinel = false;
    if (el.has_value() && el.type() == typeid(void*)) {
        auto ptr = std::any_cast<void*>(el);
        if (ptr == reinterpret_cast<void*>(&JSONATA_NONE_SENTINEL_VAR)) {
            isNoneSentinel = true;
        }
    }

    if (!isNoneSentinel) {
        if (el.type() == typeid(Utils::JList)) {
            auto vec = arrayify(el);
            // If single element list, add that element; otherwise add the list
            // itself
            if (vec.size() == 1) {
                sequence.push_back(vec[0]);
            } else {
                sequence.push_back(el);
            }
        } else {
            // Add the element directly, even if it is an explicit JSON null or
            // an undefined/empty value. This matches Java's
            // Utils.createSequence which adds null as an element (el != NONE).
            sequence.push_back(el);
        }
    }

    return sequence;
}

bool Utils::isSequence(const std::any& result) {
    try {
        auto jlist = std::any_cast<JList>(result);
        return jlist.sequence;
    } catch (const std::bad_any_cast&) {
        return false;
    }
}

std::any Utils::convertNumber(const std::any& n) {
    if (!isNumeric(n)) return std::any{};

    try {
        if (n.type() == typeid(double)) {
            double d = std::any_cast<double>(n);
            int64_t l = static_cast<int64_t>(d);
            if (static_cast<double>(l) == d) {
                // Number is not fractional
                if (l >= std::numeric_limits<int64_t>::min() &&
                    l <= std::numeric_limits<int64_t>::max()) {
                    return static_cast<int64_t>(l);
                } else {
                    return l;
                }
            }
            return d;
        }

        // Already an integer type
        return n;
    } catch (const std::bad_any_cast&) {
        return std::any{};
    }
}

Utils::JList Utils::arrayify(const std::any& value) {
    if (!value.has_value()) {
        return JList{};
    }

    // If it's a JList, handle both range and regular cases
    if (value.type() == typeid(JList)) {
        const auto& jlist = std::any_cast<JList>(value);

        if (jlist.isRange()) {
            // Materialize the range into a regular JList
            JList result;
            for (size_t i = 0; i < jlist.size(); ++i) {
                result.push_back(jlist[i]);
            }
            return result;
        } else {
            // For regular JList, return as-is
            return jlist;
        }
    }

    // If it's a std::vector<std::any>, convert to JList for backward
    // compatibility
    if (value.type() == typeid(std::vector<std::any>)) {
        const auto& vec = std::any_cast<std::vector<std::any>>(value);
        JList result(vec);
        return result;
    }

    // Otherwise, wrap single value in a JList
    JList result;
    result.push_back(value);
    return result;
}

void Utils::checkUrl(const std::string& str) {
    bool isHigh = false;
    for (size_t i = 0; i < str.length(); i++) {
        bool wasHigh = isHigh;
        char16_t ch = static_cast<char16_t>(str[i]);
        isHigh = (ch >= 0xD800 && ch <= 0xDBFF);  // High surrogate range

        if (wasHigh && isHigh) {
            throw JException("Malformed URL", static_cast<int64_t>(i));
        }
    }
    if (isHigh) {
        throw JException("Malformed URL", 0);
    }
}

std::any Utils::convertValue(const std::any& val) {
    // Java reference: return val != Jsonata.NULL_VALUE ? val : null;
    if (isNullValue(val)) {
        return std::any{};
    }
    if (val.has_value() && val.type() == typeid(std::string)) {
        auto str = std::any_cast<std::string>(val);
        (void)str; // no-op retain for compatibility; JSON null is now sentinel
    }
    return val;
}

std::any Utils::convertNulls(const std::any& res) {
    std::any result = res;
    recurse(result);
    return convertValue(result);
}

void Utils::convertNullsMap(std::any& res) {
    try {
        auto& map =
            std::any_cast<nlohmann::ordered_map<std::string, std::any>&>(res);
        for (auto it = map.begin(); it != map.end(); ++it) {
            std::any& valRef = it->second;
            std::any converted = convertValue(valRef);
            if (converted.type() != valRef.type()) {
                valRef = converted;
            }
            recurse(valRef);
        }
    } catch (const std::bad_any_cast&) {
        // Not a map, ignore
    }
}

void Utils::convertNullsList(std::any& res) {
    try {
        auto& jlist = std::any_cast<JList&>(res);
        for (auto& item : jlist) {
            std::any converted = convertValue(item);
            if (converted.type() != item.type()) {
                item = converted;
            }
            recurse(item);
        }
    } catch (const std::bad_any_cast&) {
        // Not a JList, ignore
    }
}

void Utils::recurse(std::any& val) {
    convertNullsMap(val);
    convertNullsList(val);
}

void Utils::quote(const std::string& string, std::ostringstream& w) {
    // Note: This is a simplified implementation that handles ASCII characters.
    // For full Unicode support matching Java's behavior, we would need proper
    // UTF-8/UTF-16 handling.

    char b = 0;
    char c = 0;

    for (size_t i = 0; i < string.length(); i++) {
        b = c;
        c = string[i];

        switch (c) {
            case '\\':
            case '"':
                w << '\\' << c;
                break;
            case '\b':
                w << "\\b";
                break;
            case '\t':
                w << "\\t";
                break;
            case '\n':
                w << "\\n";
                break;
            case '\f':
                w << "\\f";
                break;
            case '\r':
                w << "\\r";
                break;
            default: {
                unsigned char uc = static_cast<unsigned char>(c);
                // Match Java logic: c < ' ' || (c >= '\u0080' && c < '\u00a0')
                // || (c >= '\u2000' && c < '\u2100') For ASCII chars (0-127),
                // check the first two conditions
                if (c < ' ' || (uc >= 0x80 && uc < 0xa0)) {
                    w << "\\u";
                    std::ostringstream hex;
                    hex << std::hex << std::uppercase
                        << static_cast<uint32_t>(uc);
                    std::string hhhh = hex.str();
                    // Pad with zeros to 4 digits
                    size_t hlen = hhhh.length();
                    size_t zeros = (hlen < 4 ? 4 - hlen : 0);
                    for (size_t j = 0; j < zeros; ++j) {
                        w << '0';
                    }
                    w << hhhh;
                } else {
                    // For UTF-8 multi-byte sequences and characters >= 0x2000,
                    // proper Unicode handling would be needed here.
                    // For now, output as-is for basic functionality.
                    w << c;
                }
                break;
            }
        }
    }
}

// JList method implementations

// Copy constructor
Utils::JList::JList(const JList& other)
    : std::vector<std::any>(other),
      range_start_(other.range_start_),
      range_end_(other.range_end_),
      is_range_(other.is_range_) {
    // Copy JSONata-specific flags
    sequence = other.sequence;
    outerWrapper = other.outerWrapper;
    tupleStream = other.tupleStream;
    keepSingleton = other.keepSingleton;
    cons = other.cons;
}

// Range constructor
Utils::JList::JList(int64_t start, int64_t end)
    : std::vector<std::any>(),
      range_start_(start),
      range_end_(end),
      is_range_(true) {
    if (start > end) {
        throw std::invalid_argument("Range start must be <= end");
    }
    if (end - start >= std::numeric_limits<size_t>::max()) {
        throw std::overflow_error("Range too large");
    }
}

// Override size() for ranges
size_t Utils::JList::size() const {
    if (is_range_) {
        return static_cast<size_t>(range_end_ - range_start_ + 1);
    }
    return std::vector<std::any>::size();
}

// Override operator[] for ranges (const)
std::any Utils::JList::operator[](size_t index) const {
    if (is_range_) {
        if (index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        int64_t value = range_start_ + static_cast<int64_t>(index);
        return Utils::convertNumber(std::any(static_cast<double>(value)));
    }
    return std::vector<std::any>::operator[](index);
}

// Provide non-const operator[] so assignments work; materialize ranges on
// demand
std::any& Utils::JList::operator[](size_t index) {
    materializeRangeIfNeeded();
    return std::vector<std::any>::operator[](index);
}

// Override at() for ranges (const)
std::any Utils::JList::at(size_t index) const {
    if (is_range_) {
        if (index >= size()) {
            throw std::out_of_range("Index out of bounds");
        }
        int64_t value = range_start_ + static_cast<int64_t>(index);
        return Utils::convertNumber(std::any(static_cast<double>(value)));
    }
    return std::vector<std::any>::at(index);
}

// Provide non-const at() so assignments work; materialize ranges on demand
std::any& Utils::JList::at(size_t index) {
    materializeRangeIfNeeded();
    return std::vector<std::any>::at(index);
}

// Override empty() for ranges
bool Utils::JList::empty() const {
    if (is_range_) {
        return size() == 0;
    }
    return std::vector<std::any>::empty();
}

// Range iterator operator*
std::any Utils::JList::range_iterator::operator*() const {
    return Utils::convertNumber(std::any(static_cast<double>(current_)));
}

// Range iterator operator++
Utils::JList::range_iterator& Utils::JList::range_iterator::operator++() {
    ++current_;
    return *this;
}

// Range iterator operator!=
bool Utils::JList::range_iterator::operator!=(
    const range_iterator& other) const {
    return current_ != other.current_;
}

// Range iterator operator==
bool Utils::JList::range_iterator::operator==(
    const range_iterator& other) const {
    return current_ == other.current_;
}

// For ranges, we need special iterator handling
Utils::JList::range_iterator Utils::JList::range_begin() const {
    return range_iterator(range_start_);
}

Utils::JList::range_iterator Utils::JList::range_end() const {
    return range_iterator(range_end_ + 1);
}

// If this list represents a range, materialize it into a concrete vector
void Utils::JList::materializeRangeIfNeeded() {
    if (!is_range_) return;
    const size_t count = static_cast<size_t>(range_end_ - range_start_ + 1);
    this->reserve(this->size() + count);
    for (size_t i = 0; i < count; ++i) {
        int64_t value = range_start_ + static_cast<int64_t>(i);
        this->push_back(
            Utils::convertNumber(std::any(static_cast<double>(value))));
    }
    // Clear range state after materialization
    is_range_ = false;
    range_start_ = 0;
    range_end_ = 0;
}

// JList begin/end methods - materialize ranges when iteration is needed
Utils::JList::base_iterator Utils::JList::begin() {
    // Materialize range if needed for iteration
    const_cast<JList*>(this)->materializeRangeIfNeeded();
    return std::vector<std::any>::begin();
}

Utils::JList::base_iterator Utils::JList::end() {
    // Materialize range if needed for iteration
    const_cast<JList*>(this)->materializeRangeIfNeeded();
    return std::vector<std::any>::end();
}

Utils::JList::base_const_iterator Utils::JList::begin() const {
    // For const access, we need to materialize the range if needed
    // This is a bit tricky since we can't modify a const object
    if (is_range_) {
        // Cast away const to materialize - this is safe because the logical
        // contents don't change, just the internal representation
        const_cast<JList*>(this)->materializeRangeIfNeeded();
    }
    return std::vector<std::any>::begin();
}

Utils::JList::base_const_iterator Utils::JList::end() const {
    // For const access, we need to materialize the range if needed
    if (is_range_) {
        // Cast away const to materialize - this is safe because the logical
        // contents don't change, just the internal representation
        const_cast<JList*>(this)->materializeRangeIfNeeded();
    }
    return std::vector<std::any>::end();
}

Utils::JList::base_const_iterator Utils::JList::cbegin() const {
    return begin();
}

Utils::JList::base_const_iterator Utils::JList::cend() const { return end(); }

}  // namespace jsonata