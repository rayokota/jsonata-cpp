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
#include "jsonata/utils/Signature.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>

#include "jsonata/JException.h"
#include "jsonata/Jsonata.h"  // For JFunction class definition
#include "jsonata/Parser.h"   // For Parser::Symbol
#include "jsonata/Utils.h"    // For RangeList

namespace jsonata {
namespace utils {

Signature::Signature(const std::string &signature, const std::string &function)
    : functionName_(function) {
    parseSignature(signature);
}

void Signature::setFunctionName(const std::string &functionName) {
    functionName_ = functionName;
}

int64_t Signature::findClosingBracket(const std::string &str, int64_t start,
                                  char openSymbol, char closeSymbol) {
    // returns the position of the closing symbol (e.g. bracket) in a string
    // that balances the opening symbol at position start
    int64_t depth = 1;
    int64_t position = start;
    while (position < static_cast<int64_t>(str.length())) {
        position++;
        char symbol = str[position];
        if (symbol == closeSymbol) {
            depth--;
            if (depth == 0) {
                // we're done
                break;  // out of while loop
            }
        } else if (symbol == openSymbol) {
            depth++;
        }
    }
    return position;
}

std::string Signature::getSymbol(const std::any &value) {
    // Check if value is empty (equivalent to null in Java)
    if (!value.has_value()) {
        return "m";  // m for missing
    }

    // Check for string type
    if (value.type() == typeid(std::string)) {
        return "s";
    }

    // Treat explicit JSON null sentinel as missing for signature validation
    if (Utils::isNullValue(value)) {
        return "m";
    }

    // Check for numeric types
    if (isNumericType(value)) {
        return "n";
    }

    // Check for boolean type
    if (isBooleanType(value)) {
        return "b";
    }

    // Check for array types
    if (isArrayType(value)) {
        return "a";
    }

    // Check for object types (including regex objects)
    std::string objectType = checkObjectType(value);
    if (!objectType.empty()) {
        return objectType;
    }

    // Check for function type
    if (isFunctionType(value)) {
        return "f";
    }

    // Check for lambda type
    if (isLambdaType(value)) {
        return "f";
    }

    // Default to missing
    return "m";
}

bool Signature::isNumericType(const std::any &value) {
    return Utils::isNumber(value);
}

bool Signature::isBooleanType(const std::any &value) {
    return Utils::isBoolean(value);
}

bool Signature::isArrayType(const std::any &value) {
    return Utils::isArray(value);
    Utils::isSequence(value);  // JList is a sequence in JSONata
}

std::string Signature::checkObjectType(const std::any &value) {
    // Try map (object)
    try {
        std::any_cast<nlohmann::ordered_map<std::string, std::any>>(value);
        return "o";
    } catch (const std::bad_any_cast &) {
        // Try shared_ptr<map> - check if it's a regex object
        try {
            auto regex = std::any_cast<std::regex>(value);
            return "f";
        } catch (const std::bad_any_cast &) {
            return "";
        }
    }
}

bool Signature::isFunctionType(const std::any &value) {
    // Check for JFunction (native functions like $sum, $map, etc.) - Java
    // reference equivalent
    try {
        std::any_cast<JFunction>(value);
        return true;
    } catch (const std::bad_any_cast &) {
        // Check for std::function (generic function objects)
        try {
            std::any_cast<std::function<std::any()>>(value);
            return true;
        } catch (const std::bad_any_cast &) {
            return false;
        }
    }
}

bool Signature::isLambdaType(const std::any &value) {
    try {
        if (value.type() == typeid(std::shared_ptr<Parser::Symbol>)) {
            auto symbolPtr =
                std::any_cast<std::shared_ptr<Parser::Symbol>>(value);
            return symbolPtr && symbolPtr->_jsonata_lambda;
        }
        return false;
    } catch (const std::bad_any_cast &) {
        return false;
    }
}

void Signature::next() {
    params_.push_back(param_);
    prevParam_ = param_;
    param_ = Param();
}

std::regex Signature::parseSignature(const std::string &signature) {
    // create a Regex that represents this signature and return a function that
    // when invoked, returns the validated (possibly fixed-up) arguments, or
    // throws a validation error step through the signature, one symbol at a
    // time
    int64_t position = 1;
    while (position < static_cast<int64_t>(signature.length())) {
        char symbol = signature[position];
        if (symbol == ':') {
            // TODO figure out what to do with the return type
            // ignore it for now
            break;
        }

        switch (symbol) {
            case 's':    // string
            case 'n':    // number
            case 'b':    // boolean
            case 'l':    // not so sure about expecting null?
            case 'o': {  // object
                param_.regex = "[" + std::string(1, symbol) + "m]";
                param_.type = std::string(1, symbol);
                next();
                break;
            }
            case 'a': {  // array
                // normally treat any value as singleton array
                param_.regex = "[asnblfom]";
                param_.type = std::string(1, symbol);
                param_.array = true;
                next();
                break;
            }
            case 'f': {  // function
                param_.regex = "f";
                param_.type = std::string(1, symbol);
                next();
                break;
            }
            case 'j': {  // any JSON type
                param_.regex = "[asnblom]";
                param_.type = std::string(1, symbol);
                next();
                break;
            }
            case 'x': {  // any type
                param_.regex = "[asnblfom]";
                param_.type = std::string(1, symbol);
                next();
                break;
            }
            case '-': {  // use context if param not supplied
                if (!params_.empty()) {
                    params_.back().context = true;
                    params_.back().regex += "?";
                }
                break;
            }
            case '?':    // optional param
            case '+': {  // one or more
                if (!params_.empty()) {
                    params_.back().regex += symbol;
                }
                break;
            }
            case '(': {  // choice of types
                // search forward for matching ')'
                int64_t endParen =
                    findClosingBracket(signature, position, '(', ')');
                std::string choice =
                    signature.substr(position + 1, endParen - position - 1);
                if (choice.find("<") == std::string::npos) {
                    // no parameterized types, simple regex
                    param_.regex = "[" + choice + "m]";
                } else {
                    // TODO harder
                    throw std::runtime_error(
                        "Choice groups containing parameterized types are not "
                        "supported");
                }
                param_.type = "(" + choice + ")";
                position = endParen;
                next();
                break;
            }
            case '<': {  // type parameter - can only be applied to 'a' and 'f'
                if (!params_.empty()) {
                    std::string type = params_.back().type;
                    if (type == "a" || type == "f") {
                        // search forward for matching '>'
                        int64_t endPos =
                            findClosingBracket(signature, position, '<', '>');
                        params_.back().subtype = signature.substr(
                            position + 1, endPos - position - 1);
                        position = endPos;
                    } else {
                        throw std::runtime_error(
                            "Type parameters can only be applied to functions "
                            "and arrays");
                    }
                } else {
                    throw std::runtime_error(
                        "Type parameters can only be applied to functions and "
                        "arrays");
                }
                break;
            }
        }
        position++;
    }  // end while processing symbols in signature

    std::string regexStr = "^";
    for (const auto &param : params_) {
        regexStr += "(" + param.regex + ")";
    }
    regexStr += "$";

    try {
        regex_ = std::regex(regexStr);
        signature_ = regexStr;
    } catch (const std::regex_error &e) {
        throw std::runtime_error("Regex compilation failed: " +
                                 std::string(e.what()));
    }
    return regex_;
}

void Signature::throwValidationError(const Utils::JList &badArgs,
                                     const std::string &badSig,
                                     const std::string &functionName) {
    // to figure out where this went wrong we need apply each component of the
    // regex to each argument until we get to the one that fails to match
    std::string partialPattern = "^";

    int64_t goodTo = 0;
    for (size_t index = 0; index < params_.size(); index++) {
        partialPattern += params_[index].regex;
        std::regex tester(partialPattern);
        std::smatch match;
        if (!std::regex_match(badSig, match, tester)) {
            // failed here - Java reference: line 289
            throw JException("T0410", -1, std::to_string(goodTo + 1),
                             functionName);
        }
        goodTo = static_cast<int64_t>(match[0].length());
    }
    // if it got this far, it's probably because of extraneous arguments (we
    // haven't added the trailing '$' in the regex yet.
    // Java reference: line 295
    throw JException("T0410", -1, std::to_string(goodTo + 1), functionName);
}

Utils::JList Signature::validate(const Utils::JList &args,
                                 const std::any &context) {
    Utils::JList result;

    std::string suppliedSig = "";
    for (const auto &arg : args) {
        std::string sym = getSymbol(arg);
        suppliedSig += sym;
    }

    // Validate signature against regex pattern

    std::smatch isValid;
    if (std::regex_match(suppliedSig, isValid, regex_)) {
        Utils::JList validatedArgs;
        size_t argIndex = 0;

        for (size_t index = 0; index < params_.size(); index++) {
            const Param &param = params_[index];
            std::any arg = argIndex < args.size() ? args[argIndex] : std::any{};
            std::string match = isValid[index + 1].str();

            if (match.empty()) {
                if (param.context && !param.regex.empty()) {
                    // substitute context value for missing arg
                    // first check that the context value is the right type
                    std::string contextType = getSymbol(context);
                    // test contextType against the regex for this arg (without
                    // the trailing ?)
                    std::regex paramRegex(param.regex);
                    if (std::regex_match(contextType, paramRegex)) {
                        // If context is a JList with outerWrapper, unwrap it
                        // for function arguments
                        std::any contextArg = context;
                        if (context.type() == typeid(Utils::JList)) {
                            auto jlist = std::any_cast<Utils::JList>(context);
                            if (jlist.outerWrapper && !jlist.empty()) {
                                contextArg = jlist[0];
                            }
                        }
                        validatedArgs.push_back(contextArg);
                    } else {
                        // context value not compatible with this argument
                        throw JException("T0411", -1,
                                         "Context value is not a compatible "
                                         "type with argument " +
                                             std::to_string(argIndex + 1));
                    }
                } else {
                    // Java behavior: for optional array params (a?), missing
                    // args should be treated as [null]
                    if (param.type == "a" && param.regex.size() > 0 &&
                        param.regex.back() == '?') {
                        Utils::JList wrappedMissing;
                        // Represent missing as [null]
                        wrappedMissing.push_back(std::any{});
                        validatedArgs.push_back(wrappedMissing);
                    } else {
                        validatedArgs.push_back(arg);
                    }
                    argIndex++;
                }
            } else {
                // may have matched multiple args (if the regex ends with a '+'
                // split into single tokens
                for (char single : match) {
                    if (param.type == "a") {
                        if (single == 'm') {
                            // missing (undefined)
                            // Only optional array params (a?) are expanded to
                            // [null]
                            if (!param.regex.empty() &&
                                param.regex.back() == '?') {
                                Utils::JList wrappedMissing;
                                wrappedMissing.push_back(std::any{});
                                arg = wrappedMissing;
                            } else {
                                // For required array params, pass through
                                // missing/null
                                arg = std::any{};
                            }
                        } else {
                            arg = argIndex < args.size() ? args[argIndex]
                                                         : std::any{};
                            bool arrayOK = true;
                            // is there type information on the contents of the
                            // array?
                            if (!param.subtype.empty()) {
                                // Java reference: line 352 - if
                                // (!single.equals("a") &&
                                // !match.equals(param.subtype))
                                if (single != 'a' && match != param.subtype) {
                                    arrayOK = false;
                                } else if (single == 'a') {
                                    // Java reference: lines 355-369 - handle
                                    // List (both vector and RangeList)
                                    try {
                                        // Try std::vector first (most common
                                        // case)
                                        auto argArr =
                                            std::any_cast<Utils::JList>(arg);
                                        if (!argArr.empty()) {
                                            std::string itemType =
                                                getSymbol(argArr[0]);
                                            if (itemType !=
                                                std::string(1,
                                                            param.subtype[0])) {
                                                arrayOK = false;
                                            } else {
                                                // make sure every item in the
                                                // array is this type
                                                for (const auto &item :
                                                     argArr) {
                                                    if (getSymbol(item) !=
                                                        itemType) {
                                                        arrayOK = false;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    } catch (const std::bad_any_cast &) {
                                        try {
                                            // Java reference: exact same logic
                                            // for RangeList as List
                                            auto rangeList =
                                                std::any_cast<Utils::RangeList>(
                                                    arg);
                                            if (rangeList.size() > 0) {
                                                std::any firstItem =
                                                    rangeList[0];
                                                std::string itemType =
                                                    getSymbol(firstItem);
                                                if (itemType !=
                                                    std::string(
                                                        1, param.subtype[0])) {
                                                    arrayOK = false;
                                                } else {
                                                    // make sure every item in
                                                    // the array is this type
                                                    for (size_t i = 0;
                                                         i < rangeList.size();
                                                         ++i) {
                                                        std::any item =
                                                            rangeList[i];
                                                        if (getSymbol(item) !=
                                                            itemType) {
                                                            arrayOK = false;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        } catch (const std::bad_any_cast &) {
                                            arrayOK = false;
                                        }
                                    }
                                }
                            }
                            if (!arrayOK) {
                                // Java reference: lines 373-377 - throw
                                // JException with T0412 code Java passes: arg
                                // (actual argument), param.subtype (expected
                                // subtype)
                                throw JException("T0412", -1, arg,
                                                 param.subtype);
                            }
                            // the function expects an array. If it's not one,
                            // make it so
                            if (single != 'a') {
                                Utils::JList wrappedArg = {arg};
                                arg = wrappedArg;
                            }
                        }
                        validatedArgs.push_back(arg);
                        argIndex++;
                    } else {
                        arg = argIndex < args.size() ? args[argIndex]
                                                     : std::any{};
                        validatedArgs.push_back(arg);
                        argIndex++;
                    }
                }
            }
        }
        return validatedArgs;
    }
    throwValidationError(args, suppliedSig, functionName_);
    return {};  // dead code -> compiler happy
}

int64_t Signature::getNumberOfArgs() const {
    return static_cast<int64_t>(params_.size());
}

int64_t Signature::getMinNumberOfArgs() const {
    int64_t res = 0;
    for (const auto &p : params_) {
        if (p.regex.find('?') == std::string::npos) {
            res++;
        }
    }
    return res;
}

}  // namespace utils
}  // namespace jsonata