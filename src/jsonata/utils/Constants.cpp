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
#include "jsonata/utils/Constants.h"

namespace jsonata {
namespace utils {

const std::string Constants::ERR_MSG_SEQUENCE_UNSUPPORTED =
    "Formatting or parsing an integer as a sequence starting with %s is not "
    "supported by this implementation";

const std::string Constants::ERR_MSG_DIFF_DECIMAL_GROUP =
    "In a decimal digit pattern, all digits must be from the same decimal "
    "group";

const std::string Constants::ERR_MSG_NO_CLOSING_BRACKET =
    "No matching closing bracket ']' in date/time picture string";

const std::string Constants::ERR_MSG_UNKNOWN_COMPONENT_SPECIFIER =
    "Unknown component specifier %s in date/time picture string";

const std::string Constants::ERR_MSG_INVALID_NAME_MODIFIER =
    "The 'name' modifier can only be applied to months and days in the "
    "date/time picture string, not %s";

const std::string Constants::ERR_MSG_TIMEZONE_FORMAT =
    "The timezone integer format specifier cannot have more than four digits";

const std::string Constants::ERR_MSG_MISSING_FORMAT =
    "The date/time picture string is missing specifiers required to parse the "
    "timestamp";

const std::string Constants::ERR_MSG_INVALID_OPTIONS_SINGLE_CHAR =
    "Argument 3 of function %s is invalid. The value of the %s property must "
    "be a single character";

const std::string Constants::ERR_MSG_INVALID_OPTIONS_STRING =
    "Argument 3 of function %s is invalid. The value of the %s property must "
    "be a string";

// Decimal format symbols
const std::string Constants::SYMBOL_DECIMAL_SEPARATOR = "decimal-separator";
const std::string Constants::SYMBOL_GROUPING_SEPARATOR = "grouping-separator";
const std::string Constants::SYMBOL_INFINITY = "infinity";
const std::string Constants::SYMBOL_MINUS_SIGN = "minus-sign";
const std::string Constants::SYMBOL_NAN = "NaN";
const std::string Constants::SYMBOL_PERCENT = "percent";
const std::string Constants::SYMBOL_PER_MILLE = "per-mille";
const std::string Constants::SYMBOL_ZERO_DIGIT = "zero-digit";
const std::string Constants::SYMBOL_DIGIT = "digit";
const std::string Constants::SYMBOL_PATTERN_SEPARATOR = "pattern-separator";

}  // namespace utils
}  // namespace jsonata