/**
 * jsonata-cpp is the JSONata C++ reference port
 *
 * Copyright Dashjoin GmbH. https://dashjoin.com
 * Copyright Robert Yokota
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

// Derived from Javascript code under this license:
/**
 * © Copyright IBM Corp. 2016, 2018 All Rights Reserved
 *   Project name: JSONata
 *   This project is licensed under the MIT License, see LICENSE
 */
#pragma once

#include <any>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>

namespace jsonata {

class Tokenizer {
  public:
    // Token representation
    struct Token {
        std::string type;
        std::any value;
        int64_t position;
        std::string id;

        Token() : position(0) {}
        Token(const std::string& t, const std::any& v, int64_t pos)
            : type(t), value(v), position(pos) {}
    };

  public:
    // Static operator precedence map (public for Parser access)
    static const std::unordered_map<std::string, int64_t> operators;

  private:
    // Static escape sequence map
    static const std::unordered_map<std::string, std::string> escapes;

    // Instance variables
    std::string path_;
    size_t position_;
    size_t length_;
    int64_t depth_;

  public:
    // Constructor
    explicit Tokenizer(const std::string& path);

    // Main tokenization method
    std::unique_ptr<Token> next(bool prefix = false);

    // Getters
    size_t getPosition() const { return position_; }
    const std::string& getPath() const { return path_; }

  private:
    // Helper methods
    std::unique_ptr<Token> create(const std::string& type,
                                  const std::any& value);
    bool isClosingSlash(size_t position) const;
    std::regex scanRegex();

    // Codepoint access methods (like Java charAt)
    int32_t charAt(size_t index) const;
    std::string substring(size_t start, size_t end) const;

    // Static initialization helpers
    static std::unordered_map<std::string, int64_t> createOperators();
    static std::unordered_map<std::string, std::string> createEscapes();
};

}  // namespace jsonata