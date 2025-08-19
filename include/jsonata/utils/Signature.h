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

// Derived from original JSONata4Java Signature code under this license:
/*
 * (c) Copyright 2018, 2019 IBM Corporation
 * 1 New Orchard Road,
 * Armonk, New York, 10504-1722
 * United States
 * +1 914 499 1900
 * support: Nathaniel Mills wnm3@us.ibm.com
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
 *
 */
#pragma once

#include <any>
#include <memory>
#include <regex>
#include <string>
#include <vector>

// Include Utils to resolve JList
#include "../Utils.h"

namespace jsonata {

namespace utils {

/**
 * Manages signature related functions
 */
class Signature {
  public:
    struct Param {
        std::string type;
        std::string regex;
        bool context = false;
        bool array = false;
        std::string subtype;
        std::string contextRegex;

        std::string toString() const {
            return "Param " + type + " regex=" + regex +
                   " ctx=" + (context ? "true" : "false") +
                   " array=" + (array ? "true" : "false");
        }
    };

  private:
    Param param_;
    std::vector<Param> params_;
    Param prevParam_;
    std::regex regex_;
    std::string signature_;
    std::string functionName_;

  public:
    Signature(const std::string& signature, const std::string& function);

    void setFunctionName(const std::string& functionName);

    /**
     * Validates arguments against the signature
     * @param args List of arguments to validate
     * @param context Context object for validation
     * @return Validated arguments list
     */
    Utils::JList validate(const Utils::JList& args, const std::any& context);

    /**
     * Returns the total number of parameters in the signature
     */
    int64_t getNumberOfArgs() const;

    /**
     * Returns the minimum number of arguments (non-optional parameters)
     */
    int64_t getMinNumberOfArgs() const;

  private:
    int64_t findClosingBracket(const std::string& str, int64_t start, char openSymbol,
                           char closeSymbol);

    std::string getSymbol(const std::any& value);

    void next();

    std::regex parseSignature(const std::string& signature);

    void throwValidationError(const Utils::JList& badArgs,
                              const std::string& badSig,
                              const std::string& functionName);

    // Helper methods for type checking
    bool isNumericType(const std::any& value);
    bool isBooleanType(const std::any& value);
    bool isArrayType(const std::any& value);
    std::string checkObjectType(const std::any& value);
    bool isFunctionType(const std::any& value);
    bool isLambdaType(const std::any& value);
};

}  // namespace utils
}  // namespace jsonata