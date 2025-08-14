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
#include "jsonata/Tokenizer.h"

#include <utf8/core.h>
#include <utf8/unchecked.h>

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "jsonata/JException.h"
#include "jsonata/Utils.h"

namespace jsonata {

// Static member definitions
const std::unordered_map<std::string, int64_t> Tokenizer::operators =
    Tokenizer::createOperators();
const std::unordered_map<std::string, std::string> Tokenizer::escapes =
    Tokenizer::createEscapes();

std::unordered_map<std::string, int64_t> Tokenizer::createOperators() {
    return {{".", 75},  {"[", 80},  {"]", 0},    {"{", 70},  {"}", 0},
            {"(", 80},  {")", 0},   {",", 0},    {"@", 80},  {"#", 80},
            {";", 80},  {":", 80},  {"?", 20},   {"+", 50},  {"-", 50},
            {"*", 60},  {"/", 60},  {"%", 60},   {"|", 20},  {"=", 40},
            {"<", 40},  {">", 40},  {"^", 40},   {"**", 60}, {"..", 20},
            {":=", 10}, {"!=", 40}, {"<=", 40},  {">=", 40}, {"~>", 40},
            {"?:", 40}, {"??", 40}, {"and", 30}, {"or", 25}, {"in", 40},
            {"&", 50},  {"!", 0},   {"~", 0}};
}

std::unordered_map<std::string, std::string> Tokenizer::createEscapes() {
    return {{"\"", "\""}, {"\\", "\\"}, {"/", "/"},  {"b", "\b"},
            {"f", "\f"},  {"n", "\n"},  {"r", "\r"}, {"t", "\t"}};
}

Tokenizer::Tokenizer(const std::string& path)
    : path_(path), position_(0), depth_(0) {
    // Calculate length in codepoints using utfcpp
    length_ = utf8::unchecked::distance(path_.begin(), path_.end());
}

std::unique_ptr<Tokenizer::Token> Tokenizer::create(const std::string& type,
                                                    const std::any& value) {
    return std::make_unique<Token>(type, value, static_cast<int64_t>(position_));
}

// Helper method to get codepoint at specific index (like Java charAt)
int32_t Tokenizer::charAt(size_t index) const {
    if (index >= length_) {
        return 0;  // Match Java behavior: out of bounds returns 0/null char
    }

    // Use utfcpp to advance to the correct codepoint
    auto it = path_.begin();
    utf8::unchecked::advance(it, index);
    return static_cast<int32_t>(utf8::unchecked::next(it));
}

// Helper method to extract substring by codepoint indices (like Java substring)
std::string Tokenizer::substring(size_t start, size_t end) const {
    if (start >= length_) {
        return "";
    }
    if (end > length_) {
        end = length_;
    }
    if (start >= end) {
        return "";
    }

    // Use utfcpp to find byte positions for the codepoint range
    auto startIt = path_.begin();
    utf8::unchecked::advance(startIt, start);

    auto endIt = startIt;
    utf8::unchecked::advance(endIt, end - start);

    return std::string(startIt, endIt);
}

bool Tokenizer::isClosingSlash(size_t position) const {
    if (position < length_ && charAt(position) == '/' && depth_ == 0) {
        int backslashCount = 0;
        while (position > backslashCount &&
               charAt(position - (backslashCount + 1)) == '\\') {
            backslashCount++;
        }
        if (backslashCount % 2 == 0) {
            return true;
        }
    }
    return false;
}

std::regex Tokenizer::scanRegex() {
    // The prefix '/' will have been previously scanned. Find the end of the
    // regex. Search for closing '/' ignoring any that are escaped, or within
    // brackets (matches Java logic exactly)
    size_t start = position_;
    std::string pattern;
    std::string flags;

    while (position_ < length_) {
        int32_t currentChar = charAt(position_);
        if (isClosingSlash(position_)) {
            // end of regex found
            pattern = substring(start, position_);
            if (pattern.empty()) {
                throw JException("S0301", static_cast<int64_t>(position_));
            }
            position_++;
            currentChar = charAt(position_);

            // flags
            start = position_;
            while (currentChar == 'i' || currentChar == 'm') {
                position_++;
                if (position_ < length_) {
                    currentChar = charAt(position_);
                } else {
                    currentChar = 0;
                }
            }
            flags = substring(start, position_);

            // Convert flags to std::regex_constants
            std::regex_constants::syntax_option_type regexFlags =
                std::regex_constants::ECMAScript;
            if (flags.find('i') != std::string::npos) {
                regexFlags |= std::regex_constants::icase;
            }

            try {
                return std::regex(pattern, regexFlags);
            } catch (const std::regex_error& e) {
                throw JException("S0301", static_cast<int64_t>(start), pattern);
            }
        }

        if ((currentChar == '(' || currentChar == '[' || currentChar == '{') &&
            (position_ == 0 || charAt(position_ - 1) != '\\')) {
            depth_++;
        }
        if ((currentChar == ')' || currentChar == ']' || currentChar == '}') &&
            (position_ == 0 || charAt(position_ - 1) != '\\')) {
            depth_--;
        }
        position_++;
    }
    throw JException("S0302", static_cast<int64_t>(position_));
}

std::unique_ptr<Tokenizer::Token> Tokenizer::next(bool prefix) {
    if (position_ >= length_) return nullptr;

    // Skip whitespace using codepoint iteration (matches Java logic exactly)
    int32_t currentChar = charAt(position_);
    while (position_ < length_ &&
           (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' ||
            currentChar == '\r')) {
        position_++;
        if (position_ >= length_) return nullptr;
        currentChar = charAt(position_);
    }

    // Skip comments using codepoint iteration (matches Java logic exactly)
    if (currentChar == '/' && position_ + 1 < length_ &&
        charAt(position_ + 1) == '*') {
        size_t commentStart = position_;
        position_ += 2;
        currentChar = charAt(position_);
        while (!(currentChar == '*' && charAt(position_ + 1) == '/')) {
            currentChar = charAt(++position_);
            if (position_ >= length_) {
                // no closing tag
                throw JException("S0106", static_cast<int64_t>(commentStart));
            }
        }
        position_ += 2;
        currentChar = charAt(position_);
        return next(prefix);  // need this to swallow any following whitespace
    }

    // Test for regex - NOT in prefix context (matches Java reference: prefix != true)
    if (prefix != true && currentChar == '/') {
        position_++;
        auto regex = scanRegex();
        return create("regex", regex);
    }

    // Test for double-char operators using codepoint iteration
    bool haveMore = position_ < length_ - 1;  // Java: position+1 is valid

    // .. range operator
    if (currentChar == '.' && haveMore && charAt(position_ + 1) == '.') {
        position_ += 2;
        return create("operator", std::string(".."));
    }
    // := assign operator
    if (currentChar == ':' && haveMore && charAt(position_ + 1) == '=') {
        position_ += 2;
        return create("operator", std::string(":="));
    }
    // != not equals
    if (currentChar == '!' && haveMore && charAt(position_ + 1) == '=') {
        position_ += 2;
        return create("operator", std::string("!="));
    }
    // >= greater than or equal
    if (currentChar == '>' && haveMore && charAt(position_ + 1) == '=') {
        position_ += 2;
        return create("operator", std::string(">="));
    }
    // <= less than or equal
    if (currentChar == '<' && haveMore && charAt(position_ + 1) == '=') {
        position_ += 2;
        return create("operator", std::string("<="));
    }
    // ** descendant wildcard
    if (currentChar == '*' && haveMore && charAt(position_ + 1) == '*') {
        position_ += 2;
        return create("operator", std::string("**"));
    }
    // ~> chain function
    if (currentChar == '~' && haveMore && charAt(position_ + 1) == '>') {
        position_ += 2;
        return create("operator", std::string("~>"));
    }
    // ?: default / elvis operator
    if (currentChar == '?' && haveMore && charAt(position_ + 1) == ':') {
        position_ += 2;
        return create("operator", std::string("?:"));
    }
    // ?? coalescing operator
    if (currentChar == '?' && haveMore && charAt(position_ + 1) == '?') {
        position_ += 2;
        return create("operator", std::string("??"));
    }

    // Test for single char operators using codepoint iteration (matches Java
    // logic)
    if (currentChar < 128) {  // ASCII check like Java
        std::string singleChar(1, static_cast<char>(currentChar));
        if (operators.find(singleChar) != operators.end()) {
            position_++;
            return create("operator", singleChar);
        }
    }

    // Test for string literals using codepoint iteration (matches Java logic)
    if (currentChar == '"' || currentChar == '\'') {
        int32_t quoteType = currentChar;
        // double quoted string literal - find end of string
        position_++;
        std::string qstr = "";
        while (position_ < length_) {
            currentChar = charAt(position_);
            if (currentChar == '\\') {  // escape sequence
                position_++;
                currentChar = charAt(position_);
                if (currentChar == 0) {  // End of string
                    throw JException("S0101", static_cast<int64_t>(position_));
                }
                std::string escapeChar(1, static_cast<char>(currentChar));
                auto it = escapes.find(escapeChar);
                if (it != escapes.end()) {
                    qstr += it->second;
                } else if (currentChar == 'u') {
                    // u should be followed by 4 hex digits (matches Java logic
                    // exactly)
                    std::string octets =
                        substring(position_ + 1, position_ + 5);
                    if (octets.length() == 4 &&
                        std::all_of(octets.begin(), octets.end(),
                                    [](char c) { return std::isxdigit(c); })) {
                        int64_t codepoint = std::stoi(octets, nullptr, 16);

                        // Check if this is a high surrogate that should be
                        // paired with a low surrogate
                        if (utf8::internal::is_lead_surrogate(codepoint)) {
                            // Look ahead for a second \uXXXX sequence
                            if (position_ + 10 < length_ &&
                                charAt(position_ + 5) == '\\' &&
                                charAt(position_ + 6) == 'u') {
                                std::string trailOctets =
                                    substring(position_ + 7, position_ + 11);
                                if (trailOctets.length() == 4 &&
                                    std::all_of(trailOctets.begin(),
                                                trailOctets.end(), [](char c) {
                                                    return std::isxdigit(c);
                                                })) {
                                    int64_t trailCodepoint =
                                        std::stoi(trailOctets, nullptr, 16);
                                    if (utf8::internal::is_trail_surrogate(
                                            trailCodepoint)) {
                                        // Convert UTF-16 surrogate pair to
                                        // UTF-8 using utfcpp
                                        std::array<uint16_t, 2> u16 = {
                                            static_cast<uint16_t>(codepoint),
                                            static_cast<uint16_t>(
                                                trailCodepoint)};
                                        utf8::unchecked::utf16to8(
                                            u16.begin(), u16.end(),
                                            std::back_inserter(qstr));
                                        position_ +=
                                            10;  // Skip both \uXXXX sequences
                                    } else {
                                        throw JException(
                                            "S0104",
                                            static_cast<int64_t>(position_));
                                    }
                                } else {
                                    throw JException(
                                        "S0104", static_cast<int64_t>(position_));
                                }
                            } else {
                                throw JException("S0104",
                                                 static_cast<int64_t>(position_));
                            }
                        } else {
                            // Regular Unicode codepoint (not a surrogate pair)
                            // via utfcpp utf16to8
                            uint16_t u16 = static_cast<uint16_t>(codepoint);
                            utf8::unchecked::utf16to8(&u16, &u16 + 1,
                                                      std::back_inserter(qstr));
                            position_ += 4;
                        }
                    } else {
                        throw JException("S0104", static_cast<int64_t>(position_));
                    }
                } else {
                    // illegal escape sequence
                    throw JException("S0301", static_cast<int64_t>(position_),
                                     currentChar);
                }
            } else if (currentChar == quoteType) {
                position_++;
                return create("string", qstr);
            } else {
                // Append the character as UTF-8 (matches Java logic: qstr +=
                // currentChar)
                if (currentChar < 0x80) {
                    qstr += static_cast<char>(currentChar);
                } else {
                    utf8::unchecked::append(static_cast<uint32_t>(currentChar),
                                            std::back_inserter(qstr));
                }
            }
            position_++;
        }
        throw JException("S0101", static_cast<int64_t>(position_));
    }

    // Test for numbers using codepoint-based substring (matches Java logic)
    {
        static const std::regex numregex(
            "^-?(0|([1-9][0-9]*))(\\.[0-9]+)?([Ee][-+]?[0-9]+)?");
        std::string remaining = substring(position_, length_);
        std::smatch match;
        if (std::regex_search(remaining, match, numregex) &&
            match.position() == 0) {
            std::string numStr = match.str(0);
            double num = std::stod(numStr);
            if (!std::isnan(num) && std::isfinite(num)) {
                // Convert byte length to codepoint length for position
                // advancement
                size_t byteLen = numStr.length();
                size_t codepointLen =
                    utf8::unchecked::distance(numStr.begin(), numStr.end());
                position_ += codepointLen;
                return create("number", Utils::convertNumber(num));
            }
            throw JException("S0102", static_cast<int64_t>(position_));
        }
    }

    // Test for quoted names (backticks) using codepoint iteration
    if (currentChar == '`') {
        // scan for closing quote
        position_++;
        size_t start = position_;
        while (position_ < length_ && charAt(position_) != '`') {
            position_++;
        }
        if (position_ < length_) {
            std::string name = substring(start, position_);
            position_++;
            return create("name", name);
        }
        position_ = length_;
        throw JException("S0105", static_cast<int64_t>(position_));
    }

    // Test for names using codepoint iteration (matches Java logic exactly)
    size_t i = position_;
    int32_t ch;
    while (true) {
        ch = i < length_ ? charAt(i) : 0;
        if (i == length_ || ch == ' ' || ch == '\t' || ch == '\n' ||
            ch == '\r' ||
            (ch < 128 && operators.find(std::string(
                             1, static_cast<char>(ch))) != operators.end())) {
            if (charAt(position_) == '$') {
                // variable reference
                std::string name = substring(position_ + 1, i);
                position_ = i;
                return create("variable", name);
            } else {
                std::string name = substring(position_, i);
                position_ = i;
                if (name == "or" || name == "in" || name == "and") {
                    return create("operator", name);
                } else if (name == "true") {
                    return create("value", true);
                } else if (name == "false") {
                    return create("value", false);
                } else if (name == "null") {
                    return create("value", std::any{});
                } else {
                    if (position_ == length_ && name.empty()) {
                        // whitespace at end of input
                        return nullptr;
                    }
                    return create("name", name);
                }
            }
        } else {
            i++;
        }
    }
}

}  // namespace jsonata