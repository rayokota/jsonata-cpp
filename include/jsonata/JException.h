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
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

// Include Utils for JList
#include "Utils.h"

namespace jsonata {

// Forward declarations
class Tokenizer;

class JException : public std::runtime_error {
  private:
    std::string error_;
    int64_t location_;
    std::any current_;
    std::any expected_;

    // Recovery fields
    std::string type_;
    Utils::JList
        remaining_;  // Will be Tokenizer::Token when Tokenizer is available

  public:
    // Constructors
    JException(const std::string& error);
    JException(const std::string& error, int64_t location);
    JException(const std::string& error, int64_t location,
               const std::any& currentToken);
    JException(const std::string& error, int64_t location,
               const std::any& currentToken, const std::any& expected);
    JException(const std::exception& cause, const std::string& error,
               int64_t location, const std::any& currentToken,
               const std::any& expected);

    // Getters
    const std::string& getError() const;
    int64_t getLocation() const;
    const std::any& getCurrent() const;
    const std::any& getExpected() const;

    // Error message generation
    std::string getDetailedErrorMessage() const;

    // Static message generation methods
    static std::string msg(const std::string& error, int64_t location,
                           const std::any& arg1, const std::any& arg2);
    static std::string msg(const std::string& error, int64_t location,
                           const std::any& arg1, const std::any& arg2,
                           bool details);

    // Recovery fields accessors
    const std::string& getType() const { return type_; }
    void setType(const std::string& type) { type_ = type; }

    const Utils::JList& getRemaining() const { return remaining_; }
    void setRemaining(const Utils::JList& remaining) { remaining_ = remaining; }

  private:
    static std::string generateMessage(const std::string& error, int64_t location,
                                       const std::any& arg1,
                                       const std::any& arg2,
                                       bool details = false);
};

}  // namespace jsonata