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
#pragma once

#include <any>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "Utils.h"

namespace jsonata {

// Forward declarations
class Parser;
class Jsonata;
class Frame;

/**
 * Built-in function library for JSONata expressions.
 * Provides mathematical, string, array, and utility functions.
 */
class Functions {
  public:
    // Aggregation functions
    static std::optional<double> sum(const Utils::JList& args);
    static std::optional<int64_t> count(const Utils::JList& args);
    static std::optional<double> max(const Utils::JList& args);
    static std::optional<double> min(const Utils::JList& args);
    static std::optional<double> average(const Utils::JList& args);

    // String functions
    static std::optional<std::string> string(const std::any& arg,
                                             bool prettify = false);
    static void validateInput(const std::any& arg);

    // Array functions
    static std::optional<int64_t> length(const std::any& arg);
    static std::any reverse(const Utils::JList& args);
    static std::any sort(const Utils::JList& args);
    static std::any distinct(const std::any& _arr);

    // String manipulation functions
    static std::optional<std::string> lowercase(const std::string& str);
    static std::optional<std::string> uppercase(const std::string& str);
    static std::optional<std::string> trim(const std::string& str);
    static std::any split(const std::string& str, const std::string& separator,
                          int64_t limit = -1);
    static std::any split(const std::string& str, const std::regex& pattern,
                          int64_t limit = -1);
    static std::optional<std::string> join(const Utils::JList& arr,
                                           const std::string& separator = "");

    // Mathematical functions
    static std::optional<double> abs(double value);
    static std::optional<double> floor(double value);
    static std::optional<double> ceil(double value);
    static std::optional<double> round(double value, int64_t precision = 0);
    static std::optional<double> sqrt(double value);
    static std::optional<double> power(double base, double exponent);
    static std::optional<std::string> formatBase(double value, int64_t radix = 10);

    // Type checking functions moved to Utils

    // Function application infrastructure
    static std::any funcApply(const std::any& func, const Utils::JList& args);
    static Utils::JList hofFuncArgs(const std::any& func, const std::any& arg1,
                                    const std::any& arg2, const std::any& arg3);
    static int64_t getFunctionArity(const std::any& func);
    static std::optional<bool> toBoolean(const std::any& arg);
    static std::optional<bool> not_(const std::any& arg);

    // Higher-order functions
    static std::any map(const Utils::JList& args);
    static std::any filter(const Utils::JList& args);
    static std::any foldLeft(const Utils::JList& args);
    static std::any single(const Utils::JList& args);

    // Advanced object/array functions
    static std::any merge(const Utils::JList& args);
    static std::any append(const Utils::JList& args);
    static std::any spread(const Utils::JList& args);
    static std::any sift(const Utils::JList& args);

    // Context-aware sort function for lambda comparator support
    static std::any sortWithContext(const Utils::JList& args,
                                    const std::any& input,
                                    std::shared_ptr<Frame> environment);

    // Helper function for default comparison logic
    static bool defaultComparator(const std::any& a, const std::any& b);

    // Additional missing functions from Java
    static std::optional<std::string> substring(
        const std::string& str, int64_t start,
        std::optional<int64_t> length = std::nullopt);
    static std::optional<std::string> substringBefore(const std::string& str,
                                                      const std::string& chars);
    static std::optional<std::string> substringAfter(const std::string& str,
                                                     const std::string& chars);
    static bool contains(const std::string& str, const std::any& token);
    static std::optional<std::string> replace(const std::string& str,
                                              const std::any& pattern,
                                              const std::any& replacement,
                                              int64_t limit = -1);
    static std::optional<double> number(const std::any& arg);
    static std::any zip(const Utils::JList& args);
    static Utils::JList keys(const std::any& arg);
    static bool exists(const std::any& arg);
    static std::any each(const std::any& obj, const std::any& func);
    static double random();
    static std::optional<std::string> pad(const std::string& str, int64_t width,
                                          const std::string& char_ = " ");
    static std::optional<std::string> formatNumber(
        double value, const std::string& picture,
        const nlohmann::ordered_map<std::string, std::any>& options = {});
    static std::any shuffle(const Utils::JList& args);
    static std::any lookup(const std::any& input, const std::string& key);
    static void error(const std::string& message);
    static void assertFn(bool condition, const std::string& message);
    static std::any functionClone(const std::any& arg);

    // Missing critical functions from Java
    static std::optional<std::string> base64encode(const std::string& str);
    static std::optional<std::string> base64decode(const std::string& str);
    static std::optional<std::string> encodeUrlComponent(
        const std::string& str);
    static std::optional<std::string> decodeUrlComponent(
        const std::string& str);
    static std::optional<std::string> encodeUrl(const std::string& str);
    static std::optional<std::string> decodeUrl(const std::string& str);

    // Helper function for URL validation (ported from Java Utils.checkUrl)
    static void checkUrl(const std::string& str,
                         const std::string& functionName);

    // DateTime functions
    static std::optional<int64_t> dateTimeToMillis(
        const std::string& timestamp, const std::string& picture = "");
    static std::optional<std::string> dateTimeFromMillis(
        int64_t millis, const std::string& picture = "",
        const std::string& timezone = "UTC");
    static std::optional<std::string> formatInteger(int64_t value,
                                                    const std::string& picture);
    static std::optional<int64_t> parseInteger(const std::string& value,
                                            const std::string& picture);

    // Advanced functions
    static std::any functionEval(const std::string& expr,
                                 const std::any& focus);
    static std::string now(const std::string& picture = "",
                           const std::string& timezone = "UTC");
    static int64_t millis();

    // Regex functions
    static Utils::JList evaluateMatcher(const std::regex& pattern,
                                        const std::string& str);
    static Utils::JList match(const std::string& str, const std::regex& pattern,
                              int64_t limit = -1);

    // Lambda detection
    static bool isLambda(const std::any& value);

    // Utility functions for data type checking
    static bool isNumeric(const std::any& value);

    // Function registry and application
    using FunctionImpl = std::function<std::any(const Utils::JList&)>;

    struct FunctionEntry {
        FunctionImpl implementation;
        std::string signature;

        FunctionEntry(FunctionImpl impl, const std::string& sig)
            : implementation(impl), signature(sig) {}
    };

    static nlohmann::ordered_map<std::string, FunctionEntry>
    getFunctionRegistry();
    static std::any applyFunction(const std::string& name,
                                  const Utils::JList& args);
    // Backward compatibility overload for std::vector<std::any>
    static std::any applyFunction(const std::string& name,
                                  const std::vector<std::any>& args);

  private:
    // Internal helper functions
    static void stringifyInternal(std::ostringstream& os, const std::any& arg,
                                  bool prettify,
                                  const std::string& indent = "");
    static Utils::JList extractNumbers(const Utils::JList& args);
    static Utils::JList extractStrings(const Utils::JList& args);
    static bool isValidJsonType(const std::any& arg);

    // Quote string for JSON output
    static void quoteString(std::ostringstream& os, const std::string& str);

    // Format double with Java BigDecimal logic
    static void formatDouble(std::ostringstream& os, double value);

    // Missing helper functions from Java implementation
    static std::string substr(const std::string& str, int64_t start,
                              int64_t length = -1);
    static int64_t getUnicodeLength(const std::string& str);
    static bool isNumericString(const std::string& str);
    static std::string safeReplacement(const std::string& replacement);
    static std::string safeReplaceAll(const std::string& str,
                                      const std::regex& pattern,
                                      const std::string& replacement);
    static std::string safeReplaceFirst(const std::string& str,
                                        const std::regex& pattern,
                                        const std::string& replacement);
    static std::string safeReplaceAllFn(const std::string& str,
                                        const std::regex& pattern,
                                        const std::any& func);
    static nlohmann::ordered_map<std::string, std::any> toJsonataMatch(
        const std::smatch& match);
    static std::string encodeURI(const std::string& uri);
    static std::string leftPad(const std::string& str, int64_t size,
                               const std::string& padStr = " ");
    static std::string rightPad(const std::string& str, int64_t size,
                                const std::string& padStr = " ");

    // formatNumber options processing
    struct FormatSymbols {
        char decimalSeparator = '.';
        char groupingSeparator = ',';
        std::string infinity = "Infinity";
        char minusSign = '-';
        char plusSign = '+';
        std::string nan = "NaN";
        char percent = '%';
        std::string perMille = "‰";  // ‰ symbol
        char zeroDigit = '0';
        char digit = '#';
        char patternSeparator = ';';
    };

    static FormatSymbols processOptionsArg(
        const nlohmann::ordered_map<std::string, std::any>& options);
    static std::string getFormattingCharacter(const std::string& value,
                                              const std::string& propertyName,
                                              bool isChar);

    // formatNumber helper functions
    static std::string formatNumberWithPattern(double value,
                                               const std::string& pattern,
                                               const FormatSymbols& symbols);
    static std::string formatScientificNotation(double value,
                                                const std::string& pattern,
                                                const FormatSymbols& symbols,
                                                bool isNegative);
};

}  // namespace jsonata