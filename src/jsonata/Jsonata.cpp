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
#include "jsonata/Jsonata.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <mutex>
#include <regex>

#include "jsonata/Functions.h"
#include "jsonata/JException.h"
#include "jsonata/Timebox.h"
#include "jsonata/Utils.h"

namespace jsonata {

// Static member definitions
std::shared_ptr<Frame> Jsonata::staticFrame_ = nullptr;

// Initialize static frame on first use
std::shared_ptr<Frame> Jsonata::getStaticFrame() {
    if (!staticFrame_) {
        staticFrame_ = std::make_shared<Frame>(nullptr);
        initializeBuiltinFunctions(staticFrame_);
    }
    return staticFrame_;
}

void Jsonata::initializeBuiltinFunctions(std::shared_ptr<Frame> frame) {
    // Register all built-in functions with signatures in the frame
    auto registryWithSignatures = Functions::getFunctionRegistry();
    for (const auto& [name, entry] : registryWithSignatures) {
        JFunction jfunc;

        // Special handling for higher-order functions that need context access
        if (name == "sort") {
            // Sort function needs access to apply method for lambda comparators
            jfunc.implementation = [](const Utils::JList& args,
                                      const std::any& input,
                                      std::shared_ptr<Frame> env) -> std::any {
                return Functions::sortWithContext(args, input, env);
            };
        } else {
            // Copy entry to avoid C++20 structured binding capture issue
            auto entryImpl = entry.implementation;
            jfunc.implementation = [entryImpl](const Utils::JList& args,
                                               const std::any& input,
                                               std::shared_ptr<Frame> env) {
                return entryImpl(args);
            };
        }

        // Create signature object from signature string
        jfunc.signature =
            std::make_shared<jsonata::utils::Signature>(entry.signature, name);
        frame->bind(name, std::any(jfunc));
    }
}

// Frame implementation
Frame::Frame() : parent_(nullptr), timeout_(0), recursionDepth_(0) {
    timestamp_ = std::chrono::steady_clock::now();
}

Frame::Frame(std::shared_ptr<Frame> enclosingEnvironment)
    : parent_(enclosingEnvironment), timeout_(0), recursionDepth_(0) {
    timestamp_ = std::chrono::steady_clock::now();
}

void Frame::bind(const std::string& name, const std::any& value) {
    bindings_[name] = value;
}

std::any Frame::lookup(const std::string& name) const {
    auto it = bindings_.find(name);
    if (it != bindings_.end()) {
        return it->second;
    }
    if (parent_) {
        return parent_->lookup(name);
    }
    return std::any{};  // null
}

void Frame::setRuntimeBounds(int64_t timeout, int64_t maxRecursionDepth) {
    timeout_ = timeout;
    recursionDepth_ = maxRecursionDepth;
    // Create Timebox to handle recursion depth checking (Java reference logic)
    // Each frame gets its own timebox instance
    auto timebox = std::make_unique<Timebox>(*this, timeout, maxRecursionDepth);
    // Keep the timebox alive by storing a reference (avoiding static)
    timebox_ = std::move(timebox);
}

void Frame::setEvaluateEntryCallback(EntryCallback callback) {
    entryCallback_ = callback;
}

void Frame::setEvaluateExitCallback(ExitCallback callback) {
    exitCallback_ = callback;
}

// Jsonata implementation
Jsonata::Jsonata() {
    parser_ = std::make_unique<Parser>();
    currentInstance_ = this;
    // Initialize environment like Java does: environment =
    // createFrame(staticFrame)
    environment_ = createFrame(getStaticFrame());
    initializeErrorCodes();
}

// Instance methods matching Java reference implementation
std::shared_ptr<Frame> Jsonata::createFrame() {
    return std::make_shared<Frame>(nullptr);
}

std::shared_ptr<Frame> Jsonata::createFrame(
    std::shared_ptr<Frame> enclosingEnvironment) {
    return std::make_shared<Frame>(enclosingEnvironment);
}

Jsonata Jsonata::jsonata(const std::string& expression) {
    Jsonata instance(expression);
    // Parse and store the expression
    return instance;
}

std::shared_ptr<Parser::Symbol> Jsonata::parse(const std::string& expression) {
    return parser_->parse(expression);
}

std::any Jsonata::evaluate(std::shared_ptr<Parser::Symbol> expr,
                           const std::any& input,
                           std::shared_ptr<Frame> environment) {
    // Thread safety: Make sure each evaluate is executed on an instance per
    // thread
    return getPerThreadInstance()->_evaluate(expr, input, environment);
}

std::any Jsonata::_evaluate(std::shared_ptr<Parser::Symbol> expr,
                            const std::any& input,
                            std::shared_ptr<Frame> environment) {
    if (!expr) {
        return std::any{};
    }

    // DEBUG: Removed excessive debug output

    std::any result;

    // Store current input and environment in thread-local context
    tls_input_ = input;
    tls_environment_ = environment;

    // Entry callback
    auto entryCallback = environment->lookup("__evaluate_entry");
    if (entryCallback.has_value()) {
        try {
            auto callback = std::any_cast<EntryCallback>(entryCallback);
            callback(expr, input, environment);
        } catch (const std::bad_any_cast&) {
            // Ignore invalid callback
        }
    }

    // Main evaluation dispatch based on expression type
    const std::string& type = expr->type;
    // Debug output removed

    if (type == "string" || type == "number" ||
        type == "value") {
        result = evaluateLiteral(expr);
    } else if (type == "name") {
        result = evaluateName(expr, input, environment);
    } else if (type == "variable") {
        result = evaluateVariable(expr, input, environment);
    } else if (type == "binary") {
        result = evaluateBinary(expr, input, environment);
    } else if (type == "unary") {
        result = evaluateUnary(expr, input, environment);
    } else if (type == "function") {
        result = evaluateFunction(expr, input, environment);
    } else if (type == "regex") {
        result = evaluateRegex(expr, input, environment);
    } else if (type == "wildcard") {
        result = evaluateWildcard(expr, input);
    } else if (type == "path") {
        result = evaluatePath(expr, input, environment);
    } else if (type == "condition") {
        result = evaluateCondition(expr, input, environment);
    } else if (type == "descendant") {
        result = evaluateDescendant(expr, input, environment);
    } else if (type == "apply") {
        result = evaluateApply(expr, input, environment);
    } else if (type == "bind") {
        result = evaluateBind(expr, input, environment);
    } else if (type == "lambda") {
        result = evaluateLambda(expr, input, environment);
    } else if (type == "transform") {
        result = evaluateTransform(expr, input, environment);
    } else if (type == "parent") {
        result = evaluateParent(expr, input, environment);
    } else if (type == "block") {
        result = evaluateBlock(expr, input, environment);
    } else if (type == "partial") {
        // Following exact Java logic for partial applications
        result = evaluatePartialApplication(expr, input, environment);
    } else {
        // Unsupported expression type - return null for now
        result = std::any{};
    }

    // Apply predicates if present - matches Java lines 210-213
    if (!expr->predicate.empty()) {
        for (const auto& pred : expr->predicate) {
            // Extract the predicate expression from pred->expr (matches Java:
            // pred.expr)
            if (pred && pred->expr.has_value()) {
                try {
                    auto predicateExpr =
                        std::any_cast<std::shared_ptr<Parser::Symbol>>(
                            pred->expr);
                    result = evaluateFilter(predicateExpr, result, environment);
                } catch (const std::bad_any_cast&) {
                    // Skip invalid predicate
                }
            }
        }
    }

    // Apply group expression if present - matches Java lines 215-217
    // Java: if (!expr.type.equals("path") && expr.group!=null)
    if (type != "path" && expr->group != nullptr) {
        result = evaluateGroupExpression(expr->group, result, environment);
    }

    // Exit callback
    auto exitCallback = environment->lookup("__evaluate_exit");
    if (exitCallback.has_value()) {
        try {
            auto callback = std::any_cast<ExitCallback>(exitCallback);
            callback(expr, input, environment, result);
        } catch (const std::bad_any_cast&) {
            // Ignore invalid callback
        }
    }

    // Result mangling - matches Java lines 225-235
    // mangle result (list of 1 element -> 1 element, empty list -> null)
    bool isArrayConstructor = expr && expr->type == "unary" &&
                              expr->value.has_value() &&
                              expr->value.type() == typeid(std::string) &&
                              std::any_cast<std::string>(expr->value) == "[";

    if (result.has_value() && Utils::isSequence(result)) {
        try {
            auto _result = std::any_cast<Utils::JList>(result);
            if (!_result.tupleStream) {
                if (expr->keepArray) {
                    _result.keepSingleton = true;
                }
                if (_result.empty()) {
                    result = std::any{};  // Java: result = null
                } else if (_result.size() == 1) {
                    result = _result.keepSingleton
                                 ? std::any(_result)
                                 : _result[0];  // Java: result =
                                                // _result.keepSingleton ?
                                                // _result : _result.get(0)
                }
                // If we modified keepSingleton but didn't change result, update
                // it with the modified JList
                if (expr->keepArray && !_result.empty() &&
                    _result.size() != 1) {
                    result = std::any(_result);
                }
            }
        } catch (const std::bad_any_cast&) {
            // If cast fails, leave result unchanged
        }
    }

    return result;
}

std::any Jsonata::evaluateLiteral(std::shared_ptr<Parser::Symbol> expr) {
    // Java reference: return expr.value!=null ? expr.value : NULL_VALUE;
    // Use special marker for JSON null literals like Java reference
    if (expr->value.has_value()) {
        return expr->value;
    } else {
        // Use sentinel for JSON null (like Java's NULL_VALUE)
        return Utils::NULL_VALUE;
    }
}

std::any Jsonata::evaluateName(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment) {
    // For name expressions, lookup the property in the input object
    if (!input.has_value()) {
        return std::any{};
    }

    try {
        std::string name = std::any_cast<std::string>(expr->value);
        auto result = Functions::lookup(input, name);
        return result;
    } catch (const std::bad_any_cast&) {
        // Handle invalid types
    }

    return std::any{};
}

std::any Jsonata::evaluateVariable(std::shared_ptr<Parser::Symbol> expr,
                                   const std::any& input,
                                   std::shared_ptr<Frame> environment) {
    try {
        std::string varName = std::any_cast<std::string>(expr->value);

        // Java reference: if the variable name is empty string, then it refers
        // to context value Java: expr.value.equals("") means standalone "$"
        if (varName.empty()) {
            // Handle JList with outerWrapper like Java (lines 1291-1292)
            // Java: result = input instanceof JList &&
            // ((JList)input).outerWrapper ? ((JList)input).get(0) : input;
            if (input.type() == typeid(Utils::JList)) {
                auto jlist = std::any_cast<Utils::JList>(input);
                if (jlist.outerWrapper) {
                    // Java calls ((JList)input).get(0) which returns the
                    // ORIGINAL input that was wrapped For wrapped empty array
                    // [], get(0) should return the original empty array []
                    if (!jlist.empty()) {
                        return jlist[0];
                    } else {
                        // For empty JList with outerWrapper, return original
                        // input
                        return input;
                    }
                }
            }
            return input;
        } else {
            // Java reference: lookup variable name in environment (line 1293)
            return environment->lookup(varName);
        }
    } catch (const std::bad_any_cast&) {
        return std::any{};
    }
}

std::any Jsonata::evaluateBinary(std::shared_ptr<Parser::Symbol> expr,
                                 const std::any& input,
                                 std::shared_ptr<Frame> environment) {
    if (!expr->lhs || !expr->rhs) {
        throw JException("S0201", expr->position,
                         "Binary expression missing operands");
    }

    auto lhs = evaluate(expr->lhs, input, environment);
    std::string op = std::any_cast<std::string>(expr->value);

    // Short-circuit evaluation for logical operators
    if (op == "and" || op == "or") {
        auto rhsEvaluator = [this, expr, input, environment]() -> std::any {
            return this->evaluate(expr->rhs, input, environment);
        };
        return evaluateBooleanExpression(lhs, rhsEvaluator, op);
    }

    auto rhs = evaluate(expr->rhs, input, environment);

    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
        return evaluateNumericExpression(lhs, rhs, op);
    } else if (op == "=" || op == "!=") {
        return evaluateEqualityExpression(lhs, rhs, op);
    } else if (op == "<" || op == "<=" || op == ">" || op == ">=") {
        return evaluateComparisonExpression(lhs, rhs, op);
    } else if (op == "&") {
        return evaluateStringConcat(lhs, rhs);
    } else if (op == "..") {
        return evaluateRangeExpression(lhs, rhs);
    } else if (op == "in") {
        return evaluateIncludesExpression(lhs, rhs);
    } else {
        throw JException("S0201", expr->position, "Unknown operator: " + op);
    }
}

std::any Jsonata::evaluateUnary(std::shared_ptr<Parser::Symbol> expr,
                                const std::any& input,
                                std::shared_ptr<Frame> environment) {
    // Handle array and object constructors based on expr->value like Java
    // implementation
    std::string exprValue;
    if (expr->value.has_value()) {
        try {
            exprValue = std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast&) {
            // If value is not a string, use id as fallback
            exprValue = expr->id;
        }
    } else {
        // Java: switch ((String)""+expr.value) - if value is empty, use empty
        // string
        exprValue = "";
    }

    if (exprValue == "[") {
        // Array constructor: [expr1, expr2, ...] - following Java
        // implementation
        std::any result = Utils::JList{};  // Start with empty list
                                           // wrapped in std::any

        int64_t idx = 0;
        for (const auto& item : expr->expressions) {
            // Note: Java sets isParallelCall flag here, but C++ Frame doesn't
            // have this field yet environment->isParallelCall = (idx > 0);

            auto value = evaluate(item, input, environment);

            // Java reference (line 653): if (value!=null) - only add non-null
            // values
            if (value.has_value()) {
                // Java reference lines 654-657: check item.value equals "["
                bool isNestedArray = false;
                if (item->value.has_value()) {
                    try {
                        std::string itemValue =
                            std::any_cast<std::string>(item->value);
                        isNestedArray = (itemValue == "[");
                    } catch (const std::bad_any_cast&) {
                        // Not a string value
                    }
                }

                if (isNestedArray) {
                    // Java line 655: ((List)result).add(value)
                    auto vec = Utils::arrayify(result);
                    vec.push_back(value);
                    result = vec;
                } else {
                    // Java line 657: result = Functions.append(result, value)
                    Utils::JList appendArgs = {result, value};
                    result = Functions::append(appendArgs);
                }
            }
            // If value is empty (undefined), skip it - matching Java behavior
            idx++;
        }

        // Handle consarray flag like Java (lines 661-666)
        if (expr->consarray) {
            // Java line 662-663: if (!(result instanceof JList)) result = new
            // JList((List)result);
            Utils::JList jlist;
            if (result.type() == typeid(Utils::JList)) {
                jlist = std::any_cast<Utils::JList>(result);
            } else {
                // Convert result to JList
                jlist = Utils::arrayify(result);
            }
            // Java line 665: ((JList)result).cons = true;
            jlist.cons = true;
            result = jlist;
        }

        return result;  // Return the result as-is (could be vector or
                        // JList with range)

    } else if (exprValue == "{") {
        // Object constructor: {key: value, ...}
        // Follow Java reference: call evaluateGroupExpression for proper
        // validation (T1003, D1009)
        return evaluateGroupExpression(expr, input, environment);

    } else {
        // Other unary operators (e.g., negation -, not !) - following Java
        // implementation exactly
        if (!expr->expression) {
            throw JException("T0410", expr->position,
                             "Missing operand for unary operator");
        }

        if (exprValue == "-") {
            // Unary minus - match Java evaluateUnary lines 630-644
            auto result = evaluate(expr->expression, input, environment);

            if (!result.has_value()) {
                // Java: if (result==null) result = null;
                return std::any{};
            } else if (Utils::isNumeric(result)) {
                // Java: result = Utils.convertNumber(
                // -((Number)result).doubleValue() );
                // Coerce to double and negate
                return Utils::convertNumber(-Utils::toDouble(result));
            } else {
                // Java: throw new JException("D1002", expr.position,
                // expr.value, result);
                throw JException("D1002", expr->position,
                                 "The numeric value following the unary "
                                 "operator cannot be negated");
            }
        } else if (exprValue == "!") {
            // Boolean negation - simple implementation
            auto operand = evaluate(expr->expression, input, environment);
            if (operand.type() == typeid(bool)) {
                return !std::any_cast<bool>(operand);
            } else if (!operand.has_value()) {
                return true;  // !null = true
            } else {
                return false;  // !anything_else = false (simplified)
            }
        } else {
            throw JException("T0410", expr->position,
                             "Unknown unary operator: " + exprValue);
        }
    }
}

std::any Jsonata::evaluateFunction(std::shared_ptr<Parser::Symbol> expr,
                                   const std::any& input,
                                   std::shared_ptr<Frame> environment) {
    // Call the context version with a special marker to indicate "no context
    // from chain operator" This distinguishes from null context which should be
    // passed as an argument
    static const std::string NO_CONTEXT_MARKER = "__NO_CONTEXT__";
    return evaluateFunctionWithContext(expr, input, environment,
                                       std::any{NO_CONTEXT_MARKER});
}

std::any Jsonata::evaluateFunctionWithContext(
    std::shared_ptr<Parser::Symbol> expr, const std::any& input,
    std::shared_ptr<Frame> environment, const std::any& applytoContext) {
    // Evaluate the procedure generically first (like Java implementation)
    // can't assume that expr.procedure is a lambda type directly
    // could be an expression that evaluates to a function (e.g. variable
    // reference, parens expr etc.)
    if (!expr->procedure) {
        throw JException("T0410", expr->position, "Missing function procedure");
    }

    auto proc = evaluate(expr->procedure, input, environment);

    // Get procedure name for error reporting
    std::any procName;
    if (expr->procedure->type == "path") {
        // Handle path-based function calls (not fully implemented yet)
        procName = expr->procedure->value;
    } else {
        procName = expr->procedure->value;
    }

    // Error if proc is null
    if (!proc.has_value()) {
        throw JException("T1006", expr->position, procName);
    }

    // Evaluate arguments - following Java pattern (lines 1605-1612)
    Utils::JList evaluatedArgs;

    // If there's a context (from chain operator), add it as first argument
    // Java reference lines 1607-1609: if (applytoContext != Utils.NONE)
    // evaluatedArgs.add(applytoContext) Check if this is the special "no
    // context" marker from normal function calls
    bool isNoContextMarker = false;
    if (applytoContext.has_value() &&
        applytoContext.type() == typeid(std::string)) {
        try {
            auto marker = std::any_cast<std::string>(applytoContext);
            isNoContextMarker = (marker == "__NO_CONTEXT__");
        } catch (const std::bad_any_cast&) {
            // Not a string marker
        }
    }

    // Add context unless it's the "no context" marker
    // Java reference lines 1607-1609: if (applytoContext != Utils.NONE)
    // evaluatedArgs.add(applytoContext) In Java, ALL functions in chain
    // operators get context - signature validation handles compatibility
    if (!isNoContextMarker) {
        evaluatedArgs.push_back(applytoContext);
    }

    // Then add the regular arguments
    for (const auto& arg : expr->arguments) {
        auto argValue = evaluate(arg, input, environment);
        evaluatedArgs.push_back(argValue);
    }

    try {
        // Check if proc is a JFunction
        if (proc.type() == typeid(JFunction)) {
            auto jfunc = std::any_cast<JFunction>(proc);
            if (jfunc.implementation) {
                // Special handling for higher-order functions: check for null
                // first argument
                // before signature validation to match Java implementation
                // (Functions.java lines 1600, 1629)
                if (procName.has_value()) {
                    try {
                        auto functionName =
                            std::any_cast<std::string>(procName);
                        // For higher-order functions, check if first arg
                        // (context from chain operator) is null
                        if ((functionName == "filter" ||
                             functionName == "single" ||
                             functionName == "map" ||
                             functionName == "reduce" ||
                             functionName == "sift" ||
                             functionName == "each") &&
                            !evaluatedArgs.empty() &&
                            !evaluatedArgs[0].has_value()) {
                            // Java: if (arr == null) { return null; }
                            return std::any{};
                        }
                    } catch (const std::bad_any_cast&) {
                        // Continue with normal processing if procName is not a
                        // string
                    }
                }

                // Validate function signature if present
                if (jfunc.signature) {
                    auto validatedArgs =
                        jfunc.signature->validate(evaluatedArgs, input);
                    return jfunc.implementation(validatedArgs, input,
                                                environment);
                } else {
                    return jfunc.implementation(evaluatedArgs, input,
                                                environment);
                }
            } else {
                throw JException("T0410", expr->position,
                                 "Function has no implementation");
            }
        }
        // If it's a string, try to look it up in the function registry
        else if (proc.type() == typeid(std::string)) {
            auto functionName = std::any_cast<std::string>(proc);
            return Functions::applyFunction(functionName, evaluatedArgs);
        }
        // Check if it's a lambda function (Java reference: lambda invocation)
        else if (proc.type() == typeid(std::shared_ptr<Parser::Symbol>)) {
            const auto& symbol =
                std::any_cast<std::shared_ptr<Parser::Symbol>>(proc);
            if (symbol && symbol->_jsonata_lambda) {
                // Call the apply method to handle lambda invocation
                return apply(proc, evaluatedArgs, input, environment);
            } else {
                throw JException("T1006", expr->position, procName);
            }
        } else {
            throw JException("T1006", expr->position, procName);
        }

    } catch (const std::bad_any_cast&) {
        throw JException("T0410", expr->position, "Function evaluation error");
    }
}

std::any Jsonata::evaluateRegex(std::shared_ptr<Parser::Symbol> expr,
                                const std::any& input,
                                std::shared_ptr<Frame> environment) {
    // Regex literal: /pattern/flags
    // In JSONata, regex evaluation returns a function that can be applied to
    // strings

    if (!expr->value.has_value()) {
        throw JException("T0410", expr->position, "Invalid regex literal");
    }

    try {
        // The tokenizer stores std::regex objects in the value
        return std::any_cast<std::regex>(expr->value);
    } catch (const std::bad_any_cast&) {
        throw JException("T0410", expr->position, "Invalid regex value");
    }
}

std::any Jsonata::flatten(const std::any& arg, Utils::JList* flattened) {
    // Initialize flattened if null
    Utils::JList localFlattened;
    if (!flattened) {
        flattened = &localFlattened;
    }

    if (arg.has_value()) {
        try {
            // Java reference line 748: if(arg instanceof List)
            if (Utils::isArray(arg)) {
                // Use arrayify to handle all array types uniformly
                auto vec = Utils::arrayify(arg);
                for (const auto& item : vec) {
                    flatten(item, flattened);
                }
            } else {
                flattened->push_back(arg);
            }
        } catch (const std::bad_any_cast&) {
            flattened->push_back(arg);
        }
    }

    return *flattened;
}

std::any Jsonata::evaluateWildcard(std::shared_ptr<Parser::Symbol> expr,
                                   const std::any& input) {
    Utils::JList results = Utils::createSequence();

    if (!input.has_value()) {
        return results;
    }

    std::any _input = input;
    if (_input.type() == typeid(Utils::JList)) {
        auto jlist = std::any_cast<Utils::JList>(_input);
        if (jlist.outerWrapper) {
            // Java calls ((JList)input).get(0) which returns the
            // ORIGINAL input that was wrapped For wrapped empty array
            // [], get(0) should return the original empty array []
            if (!jlist.empty()) {
                _input = jlist[0];
            }
        }
    }
    try {
        // Handle map/object input
        if (_input.type() ==
            typeid(nlohmann::ordered_map<std::string, std::any>)) {
            auto map =
                std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                    _input);
            for (const auto& [key, value] : map) {
                // Java reference line 713: if((value instanceof List))
                if (value.has_value() && Utils::isArray(value)) {
                    // Flatten arrays and append
                    auto flattened = flatten(value, nullptr);
                    auto flattenedVec = Utils::arrayify(flattened);
                    Utils::JList appendArgs = {results, flattenedVec};
                    results = Utils::arrayify(Functions::append(appendArgs));
                } else {
                    results.push_back(value);
                }
            }
        }
        // Handle array input
        else if (Utils::isArray(_input)) {
            auto vec = Utils::arrayify(_input);
            for (const auto& value : vec) {
                // Java reference line 723: if((value instanceof List))
                if (value.has_value() && Utils::isArray(value)) {
                    // Flatten arrays and append
                    auto flattened = flatten(value, nullptr);
                    auto flattenedVec = Utils::arrayify(flattened);
                    Utils::JList appendArgs = {results, flattenedVec};
                    results = Utils::arrayify(Functions::append(appendArgs));
                } else if (value.has_value() &&
                           value.type() ==
                               typeid(nlohmann::ordered_map<std::string,
                                                            std::any>)) {
                    // Recursively call evaluateWildcard on map objects
                    auto recursiveResult = evaluateWildcard(expr, value);
                    if (recursiveResult.has_value() &&
                        Utils::isArray(recursiveResult)) {
                        auto recursiveVec = Utils::arrayify(recursiveResult);
                        results.insert(results.end(), recursiveVec.begin(),
                                       recursiveVec.end());
                    }
                } else {
                    results.push_back(value);
                }
            }
        }
    } catch (const std::bad_any_cast&) {
        // Fall through
    }

    return results;
}

std::any Jsonata::evaluatePath(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment) {
    std::any current = input;

    // Java reference: if the first step is a variable reference ($...), then
    // the path is absolute Handle input sequence setup like Java (lines
    // 250-257)
    Utils::JList inputSequence;
    if (input.has_value() && Utils::isArray(input) && !expr->steps.empty() &&
        expr->steps[0]->type != "variable") {
        inputSequence = Utils::arrayify(input);
    } else {
        // If input is not an array or first step is variable, make it a
        // sequence
        inputSequence = Utils::createSequence(input);
    }

    std::any resultSequence;
    bool isTupleStream = false;
    std::optional<Utils::JList> tupleBindings;

    // Walk through each step in the path
    for (size_t i = 0; i < expr->steps.size(); ++i) {
        const auto& step = expr->steps[i];

        // Java reference lines 267-269: check for tuple step
        if (step->tuple.has_value()) {
            isTupleStream = true;
        }

        // Java reference lines 271-280: evaluate step based on conditions
        if (i == 0 && step->consarray) {
            // Java reference lines 271-273: if the first step is an explicit
            // array constructor, then just evaluate that (i.e. don't iterate
            // over a context array)
            resultSequence = evaluate(step, inputSequence, environment);
        } else {
            // Java reference lines 275-279: handle tuple vs regular steps
            if (isTupleStream) {
                tupleBindings = std::any_cast<Utils::JList>(evaluateTupleStep(
                    step, inputSequence, tupleBindings, environment));
            } else {
                // Regular step evaluation
                resultSequence = evaluateStep(step, inputSequence, environment,
                                              i == expr->steps.size() - 1);
            }
        }

        // Java reference lines 282-284: break if resultSequence is null or
        // empty
        if (!isTupleStream) {
            if (!resultSequence.has_value() ||
                (Utils::isArray(resultSequence) &&
                 Utils::arrayify(resultSequence).empty())) {
                break;
            }
        }

        // Java reference lines 286-288: update inputSequence if no focus
        if (step->focus.has_value()) {
            // Keep current inputSequence when there's a focus variable
        } else {
            if (resultSequence.has_value() && Utils::isArray(resultSequence)) {
                inputSequence = Utils::arrayify(resultSequence);
            }
        }
    }

    // Java reference lines 292-302: handle tuple stream results
    if (isTupleStream) {
        if (expr->tuple.has_value()) {
            // tuple stream is carrying ancestry information - keep this
            resultSequence = tupleBindings.value_or(Utils::JList{});
        } else {
            // Extract the @ values from tuple bindings
            Utils::JList result = Utils::createSequence();
            if (tupleBindings.has_value()) {
                for (const auto& tupleAny : *tupleBindings) {
                    auto tuple = std::any_cast<
                        nlohmann::ordered_map<std::string, std::any>>(tupleAny);
                    auto it = tuple.find("@");
                    if (it != tuple.end()) {
                        result.push_back(it->second);
                    }
                }
            }
            resultSequence = result;
        }
    }

    if (expr->keepSingletonArray) {
        Utils::JList jlist;

        // If we only got an ArrayList, convert it so we can set the
        // keepSingleton flag
        if (resultSequence.type() != typeid(Utils::JList)) {
            // Convert regular vector to JList
            try {
                jlist = Utils::arrayify(resultSequence);
            } catch (const std::bad_any_cast&) {
                // If it's not a vector, create a JList and add the item
                jlist.push_back(resultSequence);
            }
        }

        // if the array is explicitly constructed in the expression and marked
        // to promote singleton sequences to array
        if (resultSequence.type() == typeid(Utils::JList)) {
            jlist = std::any_cast<Utils::JList>(resultSequence);
            if (jlist.cons && !jlist.sequence) {
                jlist = Utils::createSequence(resultSequence);
            }
        }
        jlist.keepSingleton = true;
        resultSequence = jlist;
    }

    // Java reference lines 317-319: Apply group expression if present for path
    // expressions
    if (expr->group != nullptr) {
        resultSequence = evaluateGroupExpression(
            expr->group,
            isTupleStream ? std::any(tupleBindings.value_or(Utils::JList{}))
                          : resultSequence,
            environment);
    }

    return resultSequence.has_value() ? resultSequence : std::any{};
}

std::any Jsonata::evaluateCondition(std::shared_ptr<Parser::Symbol> expr,
                                    const std::any& input,
                                    std::shared_ptr<Frame> environment) {
    // Evaluate the condition
    auto conditionResult = evaluate(expr->condition, input, environment);

    // Check if condition is truthy using boolize (matches Java reference)
    if (boolize(conditionResult)) {
        return evaluate(expr->then_expr, input, environment);
    } else if (expr->else_expr) {
        return evaluate(expr->else_expr, input, environment);
    }

    return std::any{};  // No else expression - return undefined
}

// Binary operation evaluators
std::any Jsonata::evaluateNumericExpression(const std::any& lhs,
                                            const std::any& rhs,
                                            const std::string& op) {
    // Java reference lines 812-821: Check for non-numeric operands and throw
    // appropriate errors
    if (lhs.has_value() && !Utils::isNumeric(lhs)) {
        throw JException("T2001", -1, op, lhs);
    }
    if (rhs.has_value() && !Utils::isNumeric(rhs)) {
        throw JException("T2002", -1, op, rhs);
    }

    // Java reference lines 823-826: Handle null/undefined operands
    if (!lhs.has_value() || !rhs.has_value()) {
        // if either side is undefined, the result is undefined
        return std::any{};
    }

    try {
        double left = Utils::toDouble(lhs);
        double right = Utils::toDouble(rhs);

        // Java reference lines 832-848: Perform numeric operations
        double result;
        if (op == "+") {
            result = left + right;
        } else if (op == "-") {
            result = left - right;
        } else if (op == "*") {
            result = left * right;
        } else if (op == "/") {
            result = left / right;
        } else if (op == "%") {
            result = std::fmod(left, right);
        } else {
            throw JException("S0201", 0, "Unknown numeric operator: " + op);
        }

        // Java reference line 849: Convert and return result
        return Utils::convertNumber(result);
    } catch (const std::bad_any_cast&) {
        throw JException("T2001", 0, "Invalid numeric operands");
    }
}

std::any Jsonata::evaluateComparisonExpression(const std::any& lhs,
                                               const std::any& rhs,
                                               const std::string& op) {
    // Java reference lines 897-956: evaluateComparisonExpression

    // Java lines 901-902: type checks - check if operands are comparable
    // var lcomparable = lhs == null || lhs instanceof String || lhs instanceof
    // Number; var rcomparable = rhs == null || rhs instanceof String || rhs
    // instanceof Number;

    // Helper to check for JSON null marker (which is NOT comparable in Java as
    // NULL_VALUE)
    auto isJsonNull = [](const std::any& value) -> bool {
        return Utils::isNullValue(value);
    };

    // In Java: lhs == null means undefined (comparable), but NULL_VALUE is not
    // comparable. In C++: !has_value() = undefined (comparable),
    // Utils::NULL_VALUE = JSON null (NOT comparable)
    bool lcomparable =
        (!lhs.has_value()) ||
        (!isJsonNull(lhs) && lhs.type() == typeid(std::string)) ||
        Utils::isNumeric(lhs);
    bool rcomparable =
        (!rhs.has_value()) ||
        (rhs.type() == typeid(std::string) && !isJsonNull(rhs)) ||
        Utils::isNumeric(rhs);

    // Java lines 905-912: if either operand is not comparable, throw error
    if (!lcomparable || !rcomparable) {
        std::any nonComparable = lhs.has_value() ? lhs : rhs;
        throw JException("T2010", 0, op, nonComparable);
    }

    // Java lines 914-917: if either side is undefined, the result is undefined
    if (!lhs.has_value() || !rhs.has_value()) {
        return std::any{};
    }

    // Java lines 920-937: if operands are not of the same type
    if (lhs.type() != rhs.type()) {
        // Java lines 922-926: handle Number/Number comparisons with type
        // conversion
        if (Utils::isNumeric(lhs) && Utils::isNumeric(rhs)) {
            // Convert both to double for comparison (like Java converts to
            // doubleValue()) Continue to comparison below with converted values
        } else {
            // Java lines 930-936: different types that are not both numeric
            throw JException("T2009", 0, lhs, rhs);
        }
    }

    // Java lines 939-955: Cast to Comparable and perform comparison
    // Comparable _lhs = (Comparable)lhs;

    try {
        // Handle string comparison using compareTo logic (but not JSON null
        // markers)
        if (lhs.type() == typeid(std::string) &&
            rhs.type() == typeid(std::string) && !isJsonNull(lhs) &&
            !isJsonNull(rhs)) {
            std::string leftStr = std::any_cast<std::string>(lhs);
            std::string rightStr = std::any_cast<std::string>(rhs);

            // Java: _lhs.compareTo(rhs) < 0, <= 0, > 0, >= 0
            int64_t comparison = leftStr.compare(rightStr);

            switch (op[0]) {
                case '<':
                    if (op == "<") {
                        return comparison < 0;
                    } else {  // "<="
                        return comparison <= 0;
                    }
                case '>':
                    if (op == ">") {
                        return comparison > 0;
                    } else {  // ">="
                        return comparison >= 0;
                    }
            }
        }

        // Handle numeric comparison - Java type coercion logic
        if (Utils::isNumeric(lhs) && Utils::isNumeric(rhs)) {
            // Java lines 922-926: convert all to double like
            // ((Number)lhs).doubleValue()
            double left = Utils::toDouble(lhs);
            double right = Utils::toDouble(rhs);

            // Java lines 942-953: perform comparison
            switch (op[0]) {
                case '<':
                    if (op == "<") {
                        return left < right;
                    } else {  // "<="
                        return left <= right;
                    }
                case '>':
                    if (op == ">") {
                        return left > right;
                    } else {  // ">="
                        return left >= right;
                    }
            }
        }

        throw JException("T2010", 0, op, lhs);

    } catch (const std::bad_any_cast&) {
        throw JException("T2010", 0, op, lhs);
    }
}

// Helper function to implement deep equality like Java's equals() method
bool Jsonata::deepEquals(const std::any& lhs, const std::any& rhs) {
    // Java reference lines 859-888: Exact logic from evaluateEqualityExpression

    // Java lines 866-869: if either side is null/undefined, result is false
    if (!lhs.has_value() || !rhs.has_value()) {
        return false;
    }
    // Treat explicit JSON null literal (NULL_VALUE) as equal only to itself
    if (Utils::isNullValue(lhs) || Utils::isNullValue(rhs)) {
        return Utils::isNullValue(lhs) && Utils::isNullValue(rhs);
    }

    // Make copies for type conversion (following Java lines 874-877)
    std::any lhsConverted = lhs;
    std::any rhsConverted = rhs;

    // Java: if (lhs instanceof Number) lhs = ((Number)lhs).doubleValue();
    // Convert numeric types to double for comparison
    if (Utils::isNumeric(lhs)) {
        if (lhs.type() == typeid(int64_t)) {
            lhsConverted = static_cast<double>(std::any_cast<int64_t>(lhs));
        } else if (lhs.type() == typeid(uint64_t)) {
            lhsConverted = static_cast<double>(std::any_cast<uint64_t>(lhs));
        }
    }

    // Java: if (rhs instanceof Number) rhs = ((Number)rhs).doubleValue();
    if (Utils::isNumeric(rhs)) {
        if (rhs.type() == typeid(int64_t)) {
            rhsConverted = static_cast<double>(std::any_cast<int64_t>(rhs));
        } else if (rhs.type() == typeid(uint64_t)) {
            rhsConverted = static_cast<double>(std::any_cast<uint64_t>(rhs));
        }
    }

    // Handle array comparison - Java uses Collection.equals() which does deep
    // comparison
    if (Utils::isArray(lhsConverted) && Utils::isArray(rhsConverted)) {
        auto leftArray = Utils::arrayify(lhsConverted);
        auto rightArray = Utils::arrayify(rhsConverted);

        if (leftArray.size() != rightArray.size()) {
            return false;
        }

        for (size_t i = 0; i < leftArray.size(); i++) {
            if (!deepEquals(leftArray[i], rightArray[i])) {
                return false;
            }
        }
        return true;
    }

    // Handle object comparison - Java uses Map.equals() which compares all
    // key-value pairs
    if (lhsConverted.type() ==
            typeid(nlohmann::ordered_map<std::string, std::any>) &&
        rhsConverted.type() ==
            typeid(nlohmann::ordered_map<std::string, std::any>)) {
        const auto& leftMap =
            std::any_cast<const nlohmann::ordered_map<std::string, std::any>&>(
                lhsConverted);
        const auto& rightMap =
            std::any_cast<const nlohmann::ordered_map<std::string, std::any>&>(
                rhsConverted);

        if (leftMap.size() != rightMap.size()) {
            return false;
        }

        for (const auto& [key, value] : leftMap) {
            auto it = rightMap.find(key);
            if (it == rightMap.end() || !deepEquals(value, it->second)) {
                return false;
            }
        }
        return true;
    }

    // Basic type comparison after conversion
    if (lhsConverted.type() == rhsConverted.type()) {
        if (lhsConverted.type() == typeid(double)) {
            return std::any_cast<double>(lhsConverted) ==
                   std::any_cast<double>(rhsConverted);
        } else if (lhsConverted.type() == typeid(std::string)) {
            return std::any_cast<std::string>(lhsConverted) ==
                   std::any_cast<std::string>(rhsConverted);
        } else if (lhsConverted.type() == typeid(bool)) {
            return std::any_cast<bool>(lhsConverted) ==
                   std::any_cast<bool>(rhsConverted);
        }
    }

    return false;
}

std::any Jsonata::evaluateEqualityExpression(const std::any& lhs,
                                             const std::any& rhs,
                                             const std::string& op) {
    // Java reference lines 866-869: if either side is null/undefined, result is
    // false
    if (!lhs.has_value() || !rhs.has_value()) {
        return false;  // Both "=" and "!=" return false when either side is
                       // null
    }

    // Java reference lines 859-888: Use lhs.equals(rhs) behavior for non-null
    // values
    bool equal = deepEquals(lhs, rhs);

    // Java line 881/884: result = lhs.equals(rhs) for "=" or !lhs.equals(rhs)
    // for "!="
    return (op == "=") ? equal : !equal;
}

std::any Jsonata::evaluateBooleanExpression(const std::any& lhs,
                                            std::function<std::any()> rhs,
                                            const std::string& op) {
    bool leftBool = boolize(lhs);

    if (op == "and") {
        return leftBool && boolize(rhs());
    } else if (op == "or") {
        return leftBool || boolize(rhs());
    } else {
        throw JException("T0410", 0, "Unknown boolean operator");
    }
}

std::any Jsonata::evaluateStringConcat(const std::any& lhs,
                                       const std::any& rhs) {
    std::string leftStr = "";
    std::string rightStr = "";

    // If operand has value, convert to string; otherwise use empty string
    if (lhs.has_value()) {
        auto converted = Functions::string(lhs);
        if (converted) {
            leftStr = *converted;
        }
    }

    if (rhs.has_value()) {
        auto converted = Functions::string(rhs);
        if (converted) {
            rightStr = *converted;
        }
    }

    return leftStr + rightStr;
}

std::any Jsonata::evaluateRangeExpression(const std::any& lhs,
                                          const std::any& rhs) {
    // Java reference lines 1171-1184: Check operand types
    if (lhs.has_value() && !Utils::isIntegral(lhs)) {
        throw JException("T2003", -1, lhs);
    }
    if (rhs.has_value() && !Utils::isIntegral(rhs)) {
        throw JException("T2004", -1, rhs);
    }

    // Java reference lines 1186-1189: Handle null/undefined operands
    if (!lhs.has_value() || !rhs.has_value()) {
        // if either side is undefined, the result is undefined
        return std::any{};
    }

    try {
        // Java reference line 1191: Convert to long values
        int64_t start = Utils::toLong(lhs);
        int64_t end = Utils::toLong(rhs);

        // Java reference lines 1193-1196: Check range validity
        if (start > end) {
            // if the lhs is greater than the rhs, return undefined
            return std::any{};
        }

        // Java reference lines 1198-1208: Check size limit
        int64_t size = end - start + 1;
        if (size > 1e7) {
            throw JException("D2014", -1, size);
        }

        // Java reference line 1210: Return JList as range (lazy like Java)
        return Utils::JList(start, end);
    } catch (const std::bad_any_cast&) {
        throw JException("T2003", -1, "Invalid range operands");
    }
}

std::any Jsonata::evaluateIncludesExpression(const std::any& lhs,
                                             const std::any& rhs) {
    // Java reference lines 968-970: if either side is undefined, the result is
    // false
    if (!lhs.has_value() || !rhs.has_value()) {
        return false;
    }

    // Java reference lines 973-976: if(!(rhs instanceof List)) convert to array
    Utils::JList rhsArray;
    if (Utils::isArray(rhs)) {
        rhsArray = Utils::arrayify(rhs);
    } else {
        // Convert non-array rhs to single-element array
        rhsArray.push_back(rhs);
    }

    // Java reference lines 978-984: check for inclusion in array
    for (const auto& item : rhsArray) {
        auto equality = evaluateEqualityExpression(lhs, item, "=");
        if (std::any_cast<bool>(equality)) {
            return true;
        }
    }

    return false;
}

std::any Jsonata::evaluateDescendant(std::shared_ptr<Parser::Symbol> expr,
                                     const std::any& input,
                                     std::shared_ptr<Frame> environment) {
    // Port of Java evaluateDescendants function - exact logic match
    std::any result;
    Utils::JList resultSequence = Utils::createSequence();

    if (input.has_value() && input.type() != typeid(nullptr)) {
        // traverse all descendants of this object/array
        recurseDescendants(input, resultSequence);
        if (resultSequence.size() == 1) {
            result = resultSequence[0];
        } else {
            result = resultSequence;
        }
    }
    return result;
}

void Jsonata::recurseDescendants(const std::any& input,
                                 std::vector<std::any>& results) {
    // Port of Java recurseDescendants - this is the equivalent of //* in XPath

    // Check if input is not a List/vector - following Java logic exactly
    bool isVector = Utils::isArray(input);

    if (!isVector) {
        results.push_back(input);
    }

    // If input is a List/vector
    if (isVector) {
        auto vec = Utils::arrayify(input);
        for (const auto& member : vec) {
            recurseDescendants(member, results);
        }
    } else if (input.has_value()) {
        // Check if input is a Map (JSON object) - following existing patterns
        // in the codebase
        try {
            auto map =
                std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                    input);
            for (const auto& [key, value] : map) {
                recurseDescendants(value, results);
            }
        } catch (const std::bad_any_cast&) {
            // Not a map either - continue, following Java logic
        }
    }
}

// Static chainAST based on Java reference implementation
static std::shared_ptr<Parser::Symbol> chainAST = nullptr;

static std::shared_ptr<Parser::Symbol> getChainAST() {
    if (!chainAST) {
        // Only create on demand - exact Java logic
        auto parser = Jsonata::getCurrentParser();
        if (!parser) {
            parser = std::make_shared<Parser>();
        }
        chainAST =
            parser->parse("function($f, $g) { function($x){ $g($f($x)) } }");
    }
    return chainAST;
}

std::any Jsonata::evaluateApply(std::shared_ptr<Parser::Symbol> expr,
                                const std::any& input,
                                std::shared_ptr<Frame> environment) {
    // Function application: expr ~> func - exact Java implementation lines
    // 1512-1548
    if (!expr || !expr->lhs || !expr->rhs) {
        return std::any{};
    }

    // Java line 1516: var lhs = /* await */ evaluate(expr.lhs, input,
    // environment);
    auto lhs = evaluate(expr->lhs, input, environment);

    if (expr->rhs->type == "function") {
        // Java lines 1518-1521: this is a function invocation; invoke it with
        // lhs expression as the first argument
        return evaluateFunctionWithContext(expr->rhs, input, environment, lhs);
    } else {
        // Java lines 1523-1544
        auto func = evaluate(expr->rhs, input, environment);

        if (!isFunctionLike(func) && !isFunctionLike(lhs)) {
            throw JException("T2006", expr->position, func);
        }

        if (isFunctionLike(lhs)) {
            // Java lines 1534-1539: this is function chaining (func1 ~> func2)
            // λ($f, $g) { λ($x){ $g($f($x)) } }
            auto chain = evaluate(getChainAST(), std::any{}, environment);
            Utils::JList args = {lhs, func};
            return apply(chain, args, std::any{}, environment);
        } else {
            // Java lines 1541-1542
            Utils::JList args = {lhs};
            return apply(func, args, std::any{}, environment);
        }
    }
}

std::any Jsonata::evaluateRange(std::shared_ptr<Parser::Symbol> expr,
                                const std::any& input,
                                std::shared_ptr<Frame> environment) {
    // Range operator: start..end
    auto startResult = evaluate(expr->lhs, input, environment);
    auto endResult = evaluate(expr->rhs, input, environment);

    // Check left side operand type (following Java pattern)
    int64_t start = Utils::toLong(startResult);

    // Check right side operand type (following Java pattern)
    int64_t end = Utils::toLong(endResult);

    Utils::JList range;
    if (start <= end) {
        for (int64_t i = start; i <= end; i++) {
            range.push_back(i);
        }
    } else {
        for (int64_t i = start; i >= end; i--) {
            range.push_back(i);
        }
    }
    return range;
}

std::any Jsonata::evaluateBind(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment) {
    // Variable binding: $var := value
    auto value = evaluate(expr->rhs, input, environment);

    // Get variable name from lhs
    std::string varName;
    if (expr->lhs && expr->lhs->value.has_value()) {
        try {
            varName = std::any_cast<std::string>(expr->lhs->value);
        } catch (const std::bad_any_cast&) {
            throw JException("S0212", expr->lhs->position,
                             "Invalid variable name");
        }
    }

    // Bind the variable in the current environment
    environment->bind(varName, value);

    return value;
}

std::any Jsonata::evaluateLambda(std::shared_ptr<Parser::Symbol> expr,
                                 const std::any& input,
                                 std::shared_ptr<Frame> environment) {
    // Java reference implementation: Jsonata.java lines 1793-1810
    // make a function (closure)
    auto procedure = std::make_shared<Parser::Symbol>();

    // Java lines 1797-1802: Set all the properties exactly as in Java
    procedure->_jsonata_lambda = true;
    procedure->input = input;
    procedure->environment = environment;
    procedure->arguments = expr->arguments;
    procedure->signature = expr->signature;
    procedure->body = expr->body;

    // Java lines 1804-1805: if(expr.thunk == true) procedure.thunk = true;
    if (expr->thunk) {
        procedure->thunk = true;
    }

    return procedure;
}

std::any Jsonata::evaluateSort(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment) {
    // Sort operator: array^(expression)
    // This is equivalent to Java's evaluateSortExpression (lines 1306-1410)

    // Java line 1310: var lhs = (List)input;
    Utils::JList arrayToSort;
    bool isTupleSort = false;

    if (!input.has_value()) {
        return std::any{};
    }

    // Java line 1311: var isTupleSort = (input instanceof JList &&
    // ((JList)input).tupleStream) ? true : false;
    if (input.type() == typeid(Utils::JList)) {
        auto jlist = std::any_cast<Utils::JList>(input);
        arrayToSort = jlist;
        isTupleSort = jlist.tupleStream;
    } else if (Utils::isArray(input)) {
        arrayToSort = Utils::arrayify(input);
    } else {
        // Single item - convert to array
        arrayToSort.push_back(input);
    }

    if (arrayToSort.empty()) {
        return std::any{};
    }

    // If no sort terms, just return the array
    if (expr->terms.empty()) {
        return arrayToSort;
    }

    // Create comparator function equivalent to Java version
    auto comparator = [this, expr, environment, isTupleSort](
                          const std::any& a, const std::any& b) -> bool {
        int64_t comp = 0;

        // Iterate through all sort terms in priority order (like Java)
        for (size_t index = 0; comp == 0 && index < expr->terms.size();
             index++) {
            auto term = expr->terms[index];

            // Evaluate sort term in context of 'a'
            std::any contextA = a;
            std::shared_ptr<Frame> envA = environment;

            if (isTupleSort) {
                // Extract context from tuple map
                try {
                    if (a.type() ==
                        typeid(nlohmann::ordered_map<std::string, std::any>)) {
                        auto tupleMap = std::any_cast<
                            nlohmann::ordered_map<std::string, std::any>>(a);
                        auto it = tupleMap.find("@");
                        if (it != tupleMap.end()) {
                            contextA = it->second;
                            envA = createFrameFromTuple(environment, tupleMap);
                        }
                    }
                } catch (const std::bad_any_cast&) {
                    // Use original context if cast fails
                }
            }

            // Java line 1331: Object aa = /* await */ evaluate(term.expression,
            // context, env); Use _evaluate to avoid circular evaluation
            std::any aa = evaluate(term->expression, contextA, envA);

            // Evaluate sort term in context of 'b'
            std::any contextB = b;
            std::shared_ptr<Frame> envB = environment;

            if (isTupleSort) {
                try {
                    if (b.type() ==
                        typeid(nlohmann::ordered_map<std::string, std::any>)) {
                        auto tupleMap = std::any_cast<
                            nlohmann::ordered_map<std::string, std::any>>(b);
                        auto it = tupleMap.find("@");
                        if (it != tupleMap.end()) {
                            contextB = it->second;
                            envB = createFrameFromTuple(environment, tupleMap);
                        }
                    }
                } catch (const std::bad_any_cast&) {
                    // Use original context if cast fails
                }
            }

            // Java line 1340: Object bb = /* await */ evaluate(term.expression,
            // context, env); Use _evaluate to avoid circular evaluation
            std::any bb = evaluate(term->expression, contextB, envB);

            // Type checking and comparison (following Java logic exactly)

            // undefined/null should be last in sort order (Java lines
            // 1346-1354)
            if (!aa.has_value()) {
                comp = bb.has_value() ? 1 : 0;
                continue;
            }
            if (!bb.has_value()) {
                comp = -1;
                continue;
            }

            // Check for JSON NULL_VALUE markers (equivalent to Java's
            // NULL_VALUE)
            bool aa_is_null_value = false;
            bool bb_is_null_value = false;

            aa_is_null_value = Utils::isNullValue(aa);

            bb_is_null_value = Utils::isNullValue(bb);

            // Java logic: if aa or bb are not string or numeric values, then
            // throw an error (lines 1357-1365) Note: NULL_VALUE is neither
            // string nor numeric, so it should trigger T2008
            bool aa_valid =
                (aa.type() == typeid(std::string) && !aa_is_null_value) ||
                Utils::isNumeric(aa);
            bool bb_valid =
                (bb.type() == typeid(std::string) && !bb_is_null_value) ||
                Utils::isNumeric(bb);

            if (!aa_valid || !bb_valid) {
                throw JException("T2008", expr->position,
                                 "The expressions within an order-by clause "
                                 "must evaluate to numeric or string values");
            }

            // Check if both values are the same type
            bool sameType = false;
            if (Utils::isNumeric(aa) && Utils::isNumeric(bb)) {
                sameType = true;
            } else if (aa.type() == bb.type()) {
                sameType = true;
            }

            if (!sameType) {
                throw JException(
                    "T2007", expr->position,
                    "Type mismatch when comparing values in order-by clause");
            }

            // Perform comparison
            try {
                if (Utils::isNumeric(aa) && Utils::isNumeric(bb)) {
                    // Numeric comparison
                    double valA = Utils::toDouble(aa);
                    double valB = Utils::toDouble(bb);

                    if (valA == valB) {
                        comp = 0;
                    } else if (valA < valB) {
                        comp = -1;
                    } else {
                        comp = 1;
                    }
                } else if (aa.type() == typeid(std::string) &&
                           bb.type() == typeid(std::string)) {
                    // String comparison
                    auto strA = std::any_cast<std::string>(aa);
                    auto strB = std::any_cast<std::string>(bb);

                    if (strA == strB) {
                        comp = 0;
                    } else if (strA < strB) {
                        comp = -1;
                    } else {
                        comp = 1;
                    }
                }

                // Handle descending order (check both descending flag and
                // value)
                bool isDescending = term->descending;
                if (term->value.has_value()) {
                    try {
                        std::string valueStr =
                            std::any_cast<std::string>(term->value);
                        if (valueStr == "descending") {
                            isDescending = true;
                        }
                    } catch (const std::bad_any_cast&) {
                        // Not a string, ignore
                    }
                }

                if (isDescending) {
                    comp = -comp;
                }

            } catch (const std::bad_any_cast&) {
                throw JException(
                    "T2008", expr->position,
                    "Invalid values for comparison in sort expression");
            }
        }

        // Return true if a < b (for ascending sort)
        return comp < 0;
    };

    // Perform the sort using the custom comparator
    try {
        std::sort(arrayToSort.begin(), arrayToSort.end(), comparator);
    } catch (const JException&) {
        throw;  // Re-throw JSONata exceptions
    } catch (const std::exception& e) {
        throw JException("T2008", expr->position,
                         "Error during sort operation");
    }

    // Return sorted result - preserve input type if input was special type
    if (input.type() == typeid(Utils::JList)) {
        Utils::JList result(arrayToSort);
        if (isTupleSort) {
            result.tupleStream = true;
        }
        return result;
    } else {
        return arrayToSort;
    }
}

std::any Jsonata::evaluateTransform(std::shared_ptr<Parser::Symbol> expr,
                                    const std::any& input,
                                    std::shared_ptr<Frame> environment) {
    // Transform expression - exact Java implementation lines 1419-1493
    // create a transformer function and return it
    if (!expr) return std::any{};

    // Create a lambda that exactly matches the Java JFunctionCallable
    auto instance = this;
    auto transformExpr = expr;

    // Create a lambda function that implements the transform - exact Java logic
    // from lines 1421-1492
    std::function<std::any(const std::any&, const Utils::JList&)> transformer =
        [instance, transformExpr, environment](
            const std::any& _input, const Utils::JList& args) -> std::any {
        // Java line 1424: var obj = ((List)args).get(0);
        if (args.empty()) {
            return std::any{};
        }
        auto obj = args[0];

        // undefined inputs always return undefined (Java lines 1427-1429)
        if (!obj.has_value()) {
            return std::any{};
        }

        // this function returns a copy of obj with changes specified by the
        // pattern/operation Java line 1432: Object result =
        // Functions.functionClone(obj);
        auto result = Functions::functionClone(obj);

        // Java lines 1434-1487: pattern matching and transformation
        auto _matches =
            instance->evaluate(transformExpr->pattern, result, environment);

        if (_matches.has_value()) {
            Utils::JList matches;

            // Java lines 1436-1438: if(!(_matches instanceof List)) { _matches
            // = new ArrayList<>(List.of(_matches)); }
            try {
                matches = Utils::arrayify(_matches);
            } catch (const std::bad_any_cast&) {
                matches = {_matches};
            }

            // Java lines 1440-1486: for(var ii = 0; ii < matches.size(); ii++)
            for (auto& match : matches) {
                // evaluate the update value for each match (Java line 1443)
                auto update = instance->evaluate(transformExpr->update, match,
                                                 environment);

                // Java lines 1448-1460: update must be an object and merge it
                if (update.has_value()) {
                    if (update.type() ==
                            typeid(
                                nlohmann::ordered_map<std::string, std::any>) &&
                        match.type() ==
                            typeid(
                                nlohmann::ordered_map<std::string, std::any>)) {
                        // Java lines 1457-1459: merge the update
                        auto& matchMap = std::any_cast<
                            nlohmann::ordered_map<std::string, std::any>&>(
                            match);
                        const auto& updateMap =
                            std::any_cast<const nlohmann::ordered_map<
                                std::string, std::any>&>(update);

                        for (const auto& [prop, value] : updateMap) {
                            matchMap[prop] = value;
                        }
                    } else {
                        // Java lines 1451-1454: throw type error if update is
                        // not an object
                        throw JException(
                            "T2011", transformExpr->update->position, update);
                    }
                }

                // Java lines 1463-1484: delete, if specified
                if (transformExpr->delete_) {
                    auto deletions = instance->evaluate(transformExpr->delete_,
                                                        match, environment);
                    if (deletions.has_value()) {
                        // Java implementation handles array of strings -
                        // simplified for now Full implementation would match
                        // Java lines 1466-1484
                    }
                }
            }
        }

        return result;
    };

    // Java line 1492: return new JFunction(transformer, "<(oa):o>");
    // In C++, we return a function-like symbol
    auto funcSymbol = std::make_shared<Parser::Symbol>();
    funcSymbol->type = "lambda";
    funcSymbol->_jsonata_lambda = true;
    funcSymbol->_transform_expr = expr;
    funcSymbol->value = transformer;

    return funcSymbol;
}

std::any Jsonata::evaluateParent(std::shared_ptr<Parser::Symbol> expr,
                                 const std::any& input,
                                 std::shared_ptr<Frame> environment) {
    // Parent operator: % (refers to parent context)
    // Java reference: result = environment.lookup(expr.slot.label);
    if (expr->slot.has_value()) {
        try {
            auto slotSymbol =
                std::any_cast<std::shared_ptr<Parser::Symbol>>(expr->slot);
            if (slotSymbol && !slotSymbol->label.empty()) {
                auto result = environment->lookup(slotSymbol->label);
                if (result.has_value()) {
                    return result;
                }
            }
        } catch (const std::bad_any_cast&) {
            // Handle conversion error - slot is not a Symbol pointer
        }
    }

    // If no slot or label, return null like Java would for undefined lookup
    return std::any{};
}

std::any Jsonata::evaluateFilter(std::shared_ptr<Parser::Symbol> predicate,
                                 const std::any& input,
                                 std::shared_ptr<Frame> environment) {
    // Java reference lines 488-526: Apply filter predicate to input data
    Utils::JList results = Utils::createSequence();

    // Java lines 491-493: handle tuple stream flag
    Utils::JList inputSequence;
    if (input.type() == typeid(Utils::JList)) {
        inputSequence = std::any_cast<Utils::JList>(input);
        if (inputSequence.tupleStream) {
            results.tupleStream = true;
        }
    } else if (!Utils::isArray(input)) {
        inputSequence = Utils::createSequence(input);
    } else {
        // If input is an array, convert it to a sequence
        // This ensures we handle arrays like Java's JList
        inputSequence = Utils::arrayify(input);
    }

    if (predicate->type == "number") {
        // Index-based filtering - Java: if (predicate.type.equals("number"))
        try {
            if (!Utils::isNumber(predicate->value)) {
                return std::any{};  // Invalid predicate type
            }
            int64_t index = Utils::toLong(predicate->value);

            // Java: if (index < 0) index = ((List)input).size() + index;
            if (index < 0) {
                index = static_cast<int64_t>(inputSequence.size()) + index;
            }

            // Java: var item = index<((List)input).size() ?
            // ((List)input).get(index) : null;
            if (index >= 0 && index < static_cast<int64_t>(inputSequence.size())) {
                auto item = inputSequence[index];
                // Follow Java exactly: only add if item != null
                if (item.has_value()) {
                    // Java reference line 505: if(item instanceof List)
                    if (Utils::isArray(item)) {
                        results = Utils::arrayify(item);
                    } else {
                        results.push_back(item);
                    }
                }
            }
        } catch (const std::bad_any_cast&) {
            return std::any{};
        }
    } else {
        // Expression-based filtering - Java: for (int index = 0; index <
        // ((List)input).size(); index++)
        for (size_t index = 0; index < inputSequence.size(); index++) {
            auto item = inputSequence[index];
            std::any context = item;
            std::shared_ptr<Frame> env = environment;

            bool isTupleStream = false;
            if (input.type() == typeid(Utils::JList)) {
                auto jlist = std::any_cast<Utils::JList>(input);
                isTupleStream = jlist.tupleStream;
            }
            if (isTupleStream &&
                item.type() ==
                    typeid(nlohmann::ordered_map<std::string, std::any>)) {
                auto tupleMap =
                    std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                        item);
                auto it = tupleMap.find("@");
                if (it != tupleMap.end()) {
                    context = it->second;
                    env = createFrameFromTuple(environment, tupleMap);
                }
            }

            // Java: var res = /* await */ evaluate(predicate, context, env);
            auto res = evaluate(predicate, context, env);

            // Java reference lines 521-523: Handle numeric results as sequences
            if (Utils::isNumeric(res)) {
                res = Utils::createSequence(res);
            }

            // Java reference lines 524-538: Handle array of numbers case
            if (Utils::isArrayOfNumbers(res)) {
                try {
                    auto numArray = Utils::arrayify(res);
                    for (const auto& ires : numArray) {
                        // round it down (following Java logic)
                        int64_t ii = Utils::toLong(ires);

                        if (ii < 0) {
                            // count in from end of array
                            ii = static_cast<int64_t>(inputSequence.size()) + ii;
                        }
                        if (ii == static_cast<int64_t>(index)) {
                            results.push_back(item);
                        }
                    }
                } catch (const std::bad_any_cast&) {
                    // Fall through to truthy check
                    if (boolize(res)) {
                        results.push_back(item);
                    }
                }
            } else if (boolize(res)) {  // truthy
                results.push_back(item);
            }
        }
    }

    return results;
}

std::any Jsonata::evaluateBlock(std::shared_ptr<Parser::Symbol> expr,
                                const std::any& input,
                                std::shared_ptr<Frame> environment) {
    // Java reference lines 1253-1265: Block expression with proper variable
    // scoping
    std::any result;

    // create a new frame to limit the scope of variable assignments
    // TODO, only do this if the post-parse stage has flagged this as required
    auto frame = createFrame(environment);

    // invoke each expression in turn
    // only return the result of the last one
    for (const auto& ex : expr->expressions) {
        result = evaluate(ex, input, frame);
    }

    return result;
}

/* static */ bool Jsonata::boolize(const std::any& value) {
    // Java reference: boolize calls Functions.toBoolean and returns booledValue
    // == null ? false : booledValue
    auto result = Functions::toBoolean(value);
    return result.has_value() ? result.value() : false;
}

/* static */ std::any Jsonata::orderedJsonToAny(
    const nlohmann::ordered_json& j) {
    if (j.is_null()) return std::any{};
    if (j.is_boolean()) return std::any(j.get<bool>());
    if (j.is_number_integer()) {
        // Preserve sign by using int64_t; for large unsigned, capture as
        // uint64_t
        int64_t si = 0;
        try {
            si = j.get<int64_t>();
            return std::any(static_cast<int64_t>(si));
        } catch (...) {
        }
        uint64_t ui = j.get<uint64_t>();
        return std::any(static_cast<uint64_t>(ui));
    }
    if (j.is_number_unsigned()) {
        uint64_t ui = j.get<uint64_t>();
        return std::any(static_cast<uint64_t>(ui));
    }
    if (j.is_number_float()) return std::any(j.get<double>());
    if (j.is_string()) return std::any(j.get<std::string>());
    if (j.is_array()) {
        std::vector<std::any> out;
        out.reserve(j.size());
        for (const auto& el : j) out.push_back(orderedJsonToAny(el));
        return out;
    }
    if (j.is_object()) {
        nlohmann::ordered_map<std::string, std::any> m;
        for (auto it = j.begin(); it != j.end(); ++it) {
            m[it.key()] = orderedJsonToAny(it.value());
        }
        return m;
    }
    return std::any{};
}

/* static */ nlohmann::ordered_json Jsonata::anyToOrderedJson(
    const std::any& value) {
    if (!value.has_value()) return nlohmann::ordered_json();

    // Canonicalize numeric types: convert doubles with no fractional part to
    // integer types
    try {
        if (Utils::isNumeric(value)) {
            std::any canon = Utils::convertNumber(value);
            if (canon.type() != value.type()) {
                return anyToOrderedJson(canon);
            }
        }
    } catch (...) {
        // Ignore conversion errors here; fall back to original value
    }

    const std::type_info& type = value.type();
    if (type == typeid(bool))
        return nlohmann::ordered_json(std::any_cast<bool>(value));
    if (type == typeid(double))
        return nlohmann::ordered_json(std::any_cast<double>(value));
    if (type == typeid(int64_t))
        return nlohmann::ordered_json(std::any_cast<int64_t>(value));
    if (type == typeid(uint64_t))
        return nlohmann::ordered_json(std::any_cast<uint64_t>(value));
    if (type == typeid(std::string))
        return nlohmann::ordered_json(std::any_cast<std::string>(value));
    if (type == typeid(Utils::JList)) {
        nlohmann::ordered_json arr = nlohmann::ordered_json::array();
        const auto& jlist = std::any_cast<const Utils::JList&>(value);
        for (const auto& item : jlist) arr.push_back(anyToOrderedJson(item));
        return arr;
    }
    if (type == typeid(std::vector<std::any>)) {
        nlohmann::ordered_json arr = nlohmann::ordered_json::array();
        const auto& vec = std::any_cast<const std::vector<std::any>&>(value);
        for (const auto& item : vec) arr.push_back(anyToOrderedJson(item));
        return arr;
    }
    if (type == typeid(nlohmann::ordered_map<std::string, std::any>)) {
        nlohmann::ordered_json obj = nlohmann::ordered_json::object();
        const auto& map =
            std::any_cast<const nlohmann::ordered_map<std::string, std::any>&>(
                value);
        for (const auto& [k, v] : map) obj[k] = anyToOrderedJson(v);
        return obj;
    }
    if (type == typeid(std::shared_ptr<Parser::Symbol>)) {
        return nlohmann::ordered_json();
    }
    // Directly handle nlohmann::ordered_json returned from custom functions
    if (type == typeid(nlohmann::ordered_json)) {
        return std::any_cast<nlohmann::ordered_json>(value);
    }
    return nlohmann::ordered_json();
}

// Unordered JSON helpers (nlohmann::json)
/* static */ std::any Jsonata::jsonToAny(const nlohmann::json& j) {
    if (j.is_null()) return std::any{};
    if (j.is_boolean()) return std::any(j.get<bool>());
    if (j.is_number_integer()) {
        int64_t si = 0;
        try {
            si = j.get<int64_t>();
            return std::any(static_cast<int64_t>(si));
        } catch (...) {
        }
        uint64_t ui = j.get<uint64_t>();
        return std::any(static_cast<uint64_t>(ui));
    }
    if (j.is_number_unsigned()) {
        uint64_t ui = j.get<uint64_t>();
        return std::any(static_cast<uint64_t>(ui));
    }
    if (j.is_number_float()) return std::any(j.get<double>());
    if (j.is_string()) return std::any(j.get<std::string>());
    if (j.is_array()) {
        std::vector<std::any> out;
        out.reserve(j.size());
        for (const auto& el : j) out.push_back(jsonToAny(el));
        return out;
    }
    if (j.is_object()) {
        nlohmann::ordered_map<std::string, std::any> m;
        for (auto it = j.begin(); it != j.end(); ++it) {
            m[it.key()] = jsonToAny(it.value());
        }
        return m;
    }
    return std::any{};
}

/* static */ nlohmann::json Jsonata::anyToJson(const std::any& value) {
    if (!value.has_value()) return nlohmann::json();

    // Canonicalize numeric types similar to anyToOrderedJson
    try {
        if (Utils::isNumeric(value)) {
            std::any canon = Utils::convertNumber(value);
            if (canon.type() != value.type()) {
                return anyToJson(canon);
            }
        }
    } catch (...) {
        // Ignore conversion errors here; fall back to original value
    }

    const std::type_info& type = value.type();
    if (type == typeid(bool))
        return nlohmann::json(std::any_cast<bool>(value));
    if (type == typeid(double))
        return nlohmann::json(std::any_cast<double>(value));
    if (type == typeid(int64_t))
        return nlohmann::json(std::any_cast<int64_t>(value));
    if (type == typeid(uint64_t))
        return nlohmann::json(std::any_cast<uint64_t>(value));
    if (type == typeid(std::string))
        return nlohmann::json(std::any_cast<std::string>(value));

    if (type == typeid(Utils::JList)) {
        nlohmann::json arr = nlohmann::json::array();
        const auto& jlist = std::any_cast<const Utils::JList&>(value);
        for (const auto& item : jlist) arr.push_back(anyToJson(item));
        return arr;
    }
    if (type == typeid(std::vector<std::any>)) {
        nlohmann::json arr = nlohmann::json::array();
        const auto& vec = std::any_cast<const std::vector<std::any>&>(value);
        for (const auto& item : vec) arr.push_back(anyToJson(item));
        return arr;
    }
    if (type == typeid(nlohmann::ordered_map<std::string, std::any>)) {
        nlohmann::json obj = nlohmann::json::object();
        const auto& map =
            std::any_cast<const nlohmann::ordered_map<std::string, std::any>&>(
                value);
        for (const auto& [k, v] : map) obj[k] = anyToJson(v);
        return obj;
    }

    // Ignore function symbols
    if (type == typeid(std::shared_ptr<Parser::Symbol>)) {
        return nlohmann::json();
    }

    // Directly handle nlohmann::json returned from custom functions
    if (type == typeid(nlohmann::json)) {
        return std::any_cast<nlohmann::json>(value);
    }

    // Handle nlohmann::ordered_json without dump/parse by converting
    if (type == typeid(nlohmann::ordered_json)) {
        const auto& oj = std::any_cast<const nlohmann::ordered_json&>(value);

        // Recursive lambda to convert ordered_json to json
        std::function<nlohmann::json(const nlohmann::ordered_json&)> convert =
            [&](const nlohmann::ordered_json& j) -> nlohmann::json {
            if (j.is_null()) return nlohmann::json();
            if (j.is_boolean()) return nlohmann::json(j.get<bool>());
            if (j.is_number_integer()) {
                int64_t si = 0;
                try {
                    si = j.get<int64_t>();
                    return nlohmann::json(static_cast<int64_t>(si));
                } catch (...) {
                }
                uint64_t ui = j.get<uint64_t>();
                return nlohmann::json(static_cast<uint64_t>(ui));
            }
            if (j.is_number_unsigned())
                return nlohmann::json(j.get<uint64_t>());
            if (j.is_number_float())
                return nlohmann::json(j.get<double>());
            if (j.is_string())
                return nlohmann::json(j.get<std::string>());
            if (j.is_array()) {
                nlohmann::json arr = nlohmann::json::array();
                for (const auto& el : j) arr.push_back(convert(el));
                return arr;
            }
            if (j.is_object()) {
                nlohmann::json obj = nlohmann::json::object();
                for (auto it = j.begin(); it != j.end(); ++it) {
                    obj[it.key()] = convert(it.value());
                }
                return obj;
            }
            return nlohmann::json();
        };

        return convert(oj);
    }

    return nlohmann::json();
}

// Missing public API methods from Java

void Jsonata::assign(const std::string& name, const std::any& value) {
    if (environment_) {
        environment_->bind(name, value);
    }
}

void Jsonata::registerFunction(const std::string& name,
                               const JFunction& implementation) {
    if (environment_) {
        environment_->bind(name, std::any(implementation));
    }
}

void Jsonata::registerFunction(
    const std::string& name,
    std::function<std::any(const Utils::JList&)> implementation) {
    JFunction jfunc;
    jfunc.implementation = [implementation](const Utils::JList& args,
                                            const std::any& input,
                                            std::shared_ptr<Frame> env) {
        return implementation(args);
    };
    registerFunction(name, jfunc);
}

bool Jsonata::isValidateInput() const { return validateInput_; }

void Jsonata::setValidateInput(bool validateInput) {
    validateInput_ = validateInput;
}

std::vector<std::exception_ptr> Jsonata::getErrors() const { return errors_; }

std::shared_ptr<Frame> Jsonata::createFrameFromTuple(
    std::shared_ptr<Frame> environment, const std::any& tupleAny) {
    // Java reference lines 324-329: createFrameFromTuple implementation
    auto frame = createFrame(environment);
    auto tuple =
        std::any_cast<nlohmann::ordered_map<std::string, std::any>>(tupleAny);
    for (const auto& [key, value] : tuple) {
        frame->bind(key, value);
    }
    return frame;
}

std::any Jsonata::reduceTupleStream(const std::any& tupleStream) {
    // Java reference lines 1137-1160: reduceTupleStream implementation
    if (tupleStream.type() != typeid(Utils::JList)) {
        return tupleStream;
    }

    auto tuples = std::any_cast<Utils::JList>(tupleStream);

    if (tuples.empty()) {
        return std::any{};
    }

    nlohmann::ordered_map<std::string, std::any> result;
    // Java line 1144: result.putAll(tupleStream.get(0));
    result =
        std::any_cast<nlohmann::ordered_map<std::string, std::any>>(tuples[0]);

    // Java lines 1147-1158: merge remaining tuples
    for (size_t i = 1; i < tuples.size(); i++) {
        const auto& el =
            std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                tuples[i]);
        for (const auto& [prop, value] : el) {
            // Java line 1154: result.put(prop,
            // Functions.append(result.get(prop), el.get(prop)));
            Utils::JList appendArgs = {result[prop], value};
            result[prop] = Functions::append(appendArgs);
        }
    }

    return result;
}

void Jsonata::initializeErrorCodes() {
    // Error codes are already initialized in JException.cpp
    // This method is for any additional initialization needed
}

// Global functions
std::any jsonata_evaluate(const std::string& expression,
                          const std::any& input) {
    Jsonata evaluator;
    auto expr = evaluator.parse(expression);
    auto environment = evaluator.createFrame();
    return evaluator.evaluate(expr, input, environment);
}

// Thread-local instances (matching Java implementation)
thread_local Jsonata* Jsonata::currentInstance_ = nullptr;
thread_local std::unique_ptr<Jsonata> Jsonata::ownedInstance_ = nullptr;
thread_local std::shared_ptr<Parser> Jsonata::currentParser_ = nullptr;
thread_local std::any Jsonata::tls_input_;
thread_local std::shared_ptr<Frame> Jsonata::tls_environment_ = nullptr;

Jsonata* Jsonata::getCurrentInstance() { return currentInstance_; }

std::shared_ptr<Parser> Jsonata::getCurrentParser() {
    // Matches Java implementation: getParser() method in Jsonata.java lines
    // 2624-2630
    auto parser = currentParser_;
    if (parser != nullptr) {
        return parser;
    }

    // Create new parser and set it (matching Java synchronized block behavior)
    parser = std::make_shared<Parser>();
    currentParser_ = parser;
    return parser;
}

Jsonata* Jsonata::getPerThreadInstance() {
    // Matches Java implementation: getPerThreadInstance() method in
    // Jsonata.java lines 1561-1575
    Jsonata* threadInst = currentInstance_;
    // Fast path
    if (threadInst != nullptr) {
        return threadInst;
    }

    // In C++, we'll use a simple mutex for thread safety (equivalent to Java's
    // synchronized block)
    static std::mutex instanceMutex;
    std::lock_guard<std::mutex> lock(instanceMutex);

    threadInst = currentInstance_;
    if (threadInst == nullptr) {
        // Create and own new instance for this thread; it will be destroyed
        // automatically when the thread exits.
        ownedInstance_ = std::make_unique<Jsonata>(*this);
        currentInstance_ = ownedInstance_.get();
        threadInst = currentInstance_;
    }
    return threadInst;
}

/* static */ void Jsonata::clearPerThreadInstance() {
    // Release owned per-thread instance and clear raw pointer.
    ownedInstance_.reset();
    currentInstance_ = nullptr;
    // Also clear TLS evaluation context to drop references promptly.
    tls_input_.reset();
    tls_environment_.reset();
}

Jsonata::Jsonata(const std::string& jsonataExpression) {
    currentInstance_ = this;
    // Initialize environment like Java does: environment =
    // createFrame(staticFrame)
    environment_ = createFrame(getStaticFrame());

    // Parse the expression
    Parser parser;
    expression_ = parser.parse(jsonataExpression);
}

Jsonata::Jsonata(const Jsonata& other) {
    // Copy constructor matching Java implementation: Jsonata(Jsonata other)
    // this.ast = other.ast;
    // this.environment = other.environment;
    // this.timestamp = other.timestamp;
    expression_ = other.expression_;
    environment_ = other.environment_;
    timestamp_ = other.timestamp_;
}

nlohmann::ordered_json Jsonata::evaluate(const nlohmann::ordered_json& input) {
    return evaluate(input, nullptr);
}

nlohmann::ordered_json Jsonata::evaluate(const nlohmann::ordered_json& input,
                                         std::shared_ptr<Frame> bindings) {
    currentInstance_ = this;

    // Check for syntax errors (equivalent to Java's check for errors != null)
    if (!expression_) {
        throw JException("S0500", 0);  // Expression compilation failed
    }

    // Convert JSON input to std::any domain for the evaluator
    std::any anyInput = orderedJsonToAny(input);

    // Always evaluate in a fresh child frame of the shared environment,
    // then (optionally) copy provided bindings into it. This avoids
    // concurrent mutations of the shared environment.
    std::shared_ptr<Frame> exec_env = createFrame(environment_);
    if (bindings != nullptr) {
        for (const auto& [key, value] : bindings->getBindings()) {
            exec_env->bind(key, value);
        }
    }

    // TODO: capture timestamp for $now() and $millis() functions
    // timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
    //     std::chrono::system_clock::now().time_since_epoch()).count();

    // CRITICAL: if the input is a JSON array, wrap it in a singleton sequence
    // Java reference lines 2575-2579:
    // if((input instanceof List) && !Utils.isSequence(input)) {
    //     input = Utils.createSequence(input);
    //     ((JList)input).outerWrapper = true;
    // }
    std::any processedInput = anyInput;
    if (anyInput.has_value() && Utils::isArray(anyInput) &&
        !Utils::isSequence(anyInput)) {
        auto sequence = Utils::createSequence(anyInput);
        sequence.outerWrapper = true;
        processedInput = sequence;
    }

    // CRITICAL: put the processed input (which may be wrapped) into the
    // fresh execution environment as the root object "$"
    exec_env->bind("$", processedInput);

    if (validateInput_) {
        Functions::validateInput(processedInput);
    }

    std::any result;
    try {
        // Set thread-local input/environment for this evaluation
        tls_input_ = processedInput;
        tls_environment_ = exec_env;
        result = evaluate(expression_, processedInput, exec_env);
        // Clear TLS after evaluation to avoid dangling references
        tls_input_.reset();
        tls_environment_.reset();
        result = Utils::convertNulls(result);
        // Convert result back to nlohmann::ordered_json
        return anyToOrderedJson(result);
    } catch (const std::exception& err) {
        // TODO: populateMessage(err);
        throw;
    }
}

nlohmann::ordered_json Jsonata::evaluate(std::nullptr_t) {
    return evaluate(nlohmann::ordered_json());
}

nlohmann::ordered_json Jsonata::evaluate(std::nullptr_t,
                                         std::shared_ptr<Frame> bindings) {
    return evaluate(nlohmann::ordered_json(), bindings);
}

// Unordered nlohmann::json variants
nlohmann::json Jsonata::evaluate(const nlohmann::json& input) {
    return evaluate(input, nullptr);
}

nlohmann::json Jsonata::evaluate(const nlohmann::json& input,
                                 std::shared_ptr<Frame> bindings) {
    currentInstance_ = this;

    if (!expression_) {
        throw JException("S0500", 0);
    }

    // Convert JSON input to std::any domain for the evaluator
    std::any anyInput = jsonToAny(input);

    std::shared_ptr<Frame> exec_env = createFrame(environment_);
    if (bindings != nullptr) {
        for (const auto& [key, value] : bindings->getBindings()) {
            exec_env->bind(key, value);
        }
    }

    std::any processedInput = anyInput;
    if (anyInput.has_value() && Utils::isArray(anyInput) &&
        !Utils::isSequence(anyInput)) {
        auto sequence = Utils::createSequence(anyInput);
        sequence.outerWrapper = true;
        processedInput = sequence;
    }

    exec_env->bind("$", processedInput);

    if (validateInput_) {
        Functions::validateInput(processedInput);
    }

    std::any result;
    try {
        tls_input_ = processedInput;
        tls_environment_ = exec_env;
        result = evaluate(expression_, processedInput, exec_env);
        tls_input_.reset();
        tls_environment_.reset();
        result = Utils::convertNulls(result);
        // Convert result to nlohmann::json
        return anyToJson(result);
    } catch (const std::exception& err) {
        throw;
    }
}

nlohmann::json Jsonata::evaluateUnordered(std::nullptr_t) {
    return evaluate(nlohmann::json());
}

nlohmann::json Jsonata::evaluateUnordered(std::nullptr_t,
                                          std::shared_ptr<Frame> bindings) {
    return evaluate(nlohmann::json(), bindings);
}

std::any Jsonata::apply(const std::any& proc, const Utils::JList& args,
                        const std::any& input,
                        std::shared_ptr<Frame> environment) {
    // Java reference implementation: Jsonata.java lines 1674-1695
    auto result = applyInner(proc, args, input, environment);

    // Trampoline loop - this gets invoked as a result of tail-call optimization
    // Java lines 1676-1693: while(Functions.isLambda(result) &&
    // ((Symbol)result).thunk == true)
    while (Functions::isLambda(result)) {
        try {
            const auto& symbolResult =
                std::any_cast<std::shared_ptr<Parser::Symbol>>(result);
            if (symbolResult && symbolResult->thunk) {
                // the function returned a tail-call thunk
                // unpack it, evaluate its arguments, and apply the tail call

                // Java line 1680: var next = /* await */
                // evaluate(((Symbol)result).body.procedure,
                // ((Symbol)result).input, ((Symbol)result).environment);
                auto instance = getCurrentInstance();
                if (!instance) break;

                std::any symbolInput, symbolEnv;
                if (symbolResult->input.has_value())
                    symbolInput = symbolResult->input;
                if (symbolResult->environment.has_value())
                    symbolEnv = symbolResult->environment;

                std::shared_ptr<Frame> envFrame;
                try {
                    envFrame = std::any_cast<std::shared_ptr<Frame>>(symbolEnv);
                } catch (const std::bad_any_cast&) {
                    envFrame = environment;  // fallback
                }

                auto next = instance->evaluate(symbolResult->body->procedure,
                                               symbolInput, envFrame);

                // Java lines 1681-1686: handle variable references
                if (symbolResult->body->procedure->type == "variable") {
                    if (next.type() ==
                        typeid(std::shared_ptr<Parser::Symbol>)) {
                        auto nextSymbol =
                            std::any_cast<std::shared_ptr<Parser::Symbol>>(
                                next);
                        nextSymbol->token =
                            symbolResult->body->procedure->value;
                        nextSymbol->position =
                            symbolResult->body->procedure->position;
                    }
                }

                // Java lines 1687-1690: evaluate arguments
                Utils::JList evaluatedArgs;
                for (const auto& arg : symbolResult->body->arguments) {
                    evaluatedArgs.push_back(
                        instance->evaluate(arg, symbolInput, envFrame));
                }

                // Java line 1692: result = /* await */ applyInner(next,
                // evaluatedArgs, input, environment);
                result = applyInner(next, evaluatedArgs, input, environment);
            } else {
                break;  // Not a thunk, exit loop
            }
        } catch (const std::bad_any_cast&) {
            break;  // Not a symbol, exit loop
        }
    }

    return result;
}

std::any Jsonata::applyInner(const std::any& proc, const Utils::JList& args,
                             const std::any& input,
                             std::shared_ptr<Frame> environment) {
    if (!proc.has_value()) {
        return std::any{};
    }

    // Remove debug output

    try {
        // Validate arguments against signature if present - Java line 1709
        // Java line 1709: validatedArgs = validateArguments(proc, args, input);
        auto validatedArgs = validateArguments(proc, args, input);

        // Java line 1713: if (Functions.isLambda(proc))
        if (Functions::isLambda(proc)) {
            // Java line 1714: result = /* await */ applyProcedure(proc,
            // validatedArgs);
            auto result = applyProcedure(proc, validatedArgs);
            return result;
        }

        // Check if it's a regex pattern (Java lines 1757-1766)
        // Java: } else if (proc instanceof Pattern) {
        if (proc.type() == typeid(std::regex)) {
            auto regex = std::any_cast<std::regex>(proc);
            Utils::JList results;

            // Java: for (String s : (List<String>)validatedArgs)
            for (const auto& arg : validatedArgs) {
                if (arg.has_value() && arg.type() == typeid(std::string)) {
                    auto str = std::any_cast<std::string>(arg);
                    // Java: if (((Pattern)proc).matcher(s).find())
                    if (std::regex_search(str, regex)) {
                        // Java: _res.add(s);
                        results.push_back(str);
                    }
                }
            }
            // Java: result = _res;
            return results;
        }

        // Check if it's a JFunction (Java lines 1730-1743)
        if (proc.type() == typeid(JFunction)) {
            auto jfunc = std::any_cast<JFunction>(proc);
            if (jfunc.implementation) {
                // handling special case: when calling a function with args =
                // [undefined] Javascript will convert to undefined (without
                // array) - Java lines 1739-1741
                Utils::JList validatedArgs = args;
                if (args.size() == 1 && !args[0].has_value()) {
                    // validatedArgs = null; // In Java this sets to null, in
                    // C++ we keep the empty vector
                }

                // Call the function - Java line 1743: result =
                // ((JFunction)proc).call(input, (List)validatedArgs);
                return jfunc.implementation(validatedArgs, input, environment);
            }
        }

        return std::any{};
    } catch (const std::bad_any_cast&) {
        return std::any{};
    }
}

std::shared_ptr<Frame> Jsonata::getEnvironment() const { return environment_; }

Utils::JList Jsonata::validateArguments(const std::any& signature,
                                        const Utils::JList& args,
                                        const std::any& context) {
    // Java reference implementation: Jsonata.java lines 1865-1874
    Utils::JList validatedArgs = args;

    if (Utils::isFunction(signature)) {
        // Implement JFunction signature validation to match Java
        try {
            const auto& jfunc = std::any_cast<JFunction>(signature);
            if (jfunc.signature) {
                // Validate args against the JFunction's signature, using input
                // as context
                return jfunc.signature->validate(args, context);
            }
        } catch (const std::bad_any_cast&) {
            // Not a JFunction value; fall through
        }
        return validatedArgs;
    } else if (Functions::isLambda(signature)) {
        // Extract signature from lambda symbol
        try {
            const auto& symbol =
                std::any_cast<std::shared_ptr<Parser::Symbol>>(signature);
            if (symbol && !symbol->signature.empty()) {
                // Create Signature object and validate
                try {
                    utils::Signature sig(symbol->signature, "lambda");
                    validatedArgs = sig.validate(args, context);
                    // Debug: check if validation is working correctly
                    if (args.size() != validatedArgs.size()) {
                        // This is expected for optional parameters - args
                        // should be expanded
                    }
                } catch (const std::exception& e) {
                    // If signature construction fails, rethrow to see the error
                    throw std::runtime_error(
                        "Signature validation failed: " +
                        std::string(e.what()) +
                        " for signature: " + symbol->signature);
                }
            } else if (symbol) {
                // Lambda has no signature - use original args without
                // validation
            }
        } catch (const std::bad_any_cast&) {
            // Return original args if casting fails
        } catch (const std::exception& e) {
            // Don't catch signature validation exceptions - let them propagate
            // up like Java
            throw;
        }
        // Note: Don't catch signature validation exceptions - let them
        // propagate up like Java
    }

    return validatedArgs;
}

std::any Jsonata::applyProcedure(const std::any& _proc,
                                 const Utils::JList& args) {
    // Java reference implementation: Jsonata.java lines 1883-1899
    try {
        const auto& symbol =
            std::any_cast<std::shared_ptr<Parser::Symbol>>(_proc);
        if (!symbol) {
            return std::any{};
        }

        // Check if this is a transform function (special case)
        if (symbol->_transform_expr) {
            // This is a transform function - apply the transformation like Java
            // implementation
            if (args.empty()) {
                return std::any{};
            }

            auto obj = args[0];

            // undefined inputs always return undefined
            if (!obj.has_value()) {
                return std::any{};
            }

            // this returns a copy of obj with changes specified by the
            // pattern/operation
            auto result = Functions::functionClone(obj);

            auto transformExpr = symbol->_transform_expr;
            if (transformExpr) {
                auto pattern = transformExpr->pattern;
                auto update = transformExpr->update;
                auto delete_expr = transformExpr->delete_;

                // Get environment from closure
                std::shared_ptr<Frame> environment;
                if (symbol->value.has_value() &&
                    symbol->value.type() ==
                        typeid(nlohmann::ordered_map<std::string, std::any>)) {
                    auto closureMap = std::any_cast<
                        nlohmann::ordered_map<std::string, std::any>>(
                        symbol->value);
                    if (closureMap.find("environment") != closureMap.end()) {
                        try {
                            environment = std::any_cast<std::shared_ptr<Frame>>(
                                closureMap["environment"]);
                        } catch (const std::bad_any_cast&) {
                            auto instance = getCurrentInstance();
                            environment = instance ? instance->getEnvironment()
                                                   : std::make_shared<Frame>();
                        }
                    } else {
                        auto instance = getCurrentInstance();
                        environment = instance ? instance->getEnvironment()
                                               : std::make_shared<Frame>();
                    }
                } else {
                    auto instance = getCurrentInstance();
                    environment = instance ? instance->getEnvironment()
                                           : std::make_shared<Frame>();
                }

                // Apply pattern matching and transformation - exact Java logic
                // lines 1434-1487
                auto instance = getCurrentInstance();
                if (!instance) {
                    throw JException("No current Jsonata instance available");
                }
                auto _matches =
                    instance->evaluate(pattern, result, environment);

                if (_matches.has_value()) {
                    // Convert to array if not already array
                    Utils::JList matches;
                    if (Utils::isArray(_matches)) {
                        matches = Utils::arrayify(_matches);
                    } else {
                        matches.push_back(_matches);
                    }

                    // Helper lambdas for deep comparison and in-place
                    // update/delete on result
                    std::function<bool(const std::any&, const std::any&)>
                        deepEqualsAny =
                            [&](const std::any& a, const std::any& b) -> bool {
                        if (!a.has_value() && !b.has_value()) return true;
                        if (!a.has_value() || !b.has_value()) return false;
                        if (a.type() == typeid(std::string) &&
                            b.type() == typeid(std::string))
                            return std::any_cast<std::string>(a) ==
                                   std::any_cast<std::string>(b);
                        if (Utils::isNumeric(a) && Utils::isNumeric(b)) {
                            try {
                                double da = Utils::toDouble(a);
                                double db = Utils::toDouble(b);
                                return da == db;
                            } catch (...) { /* fallthrough */
                            }
                        }
                        if (a.type() == typeid(bool) &&
                            b.type() == typeid(bool))
                            return std::any_cast<bool>(a) ==
                                   std::any_cast<bool>(b);
                        if (a.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>) &&
                            b.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>)) {
                            const auto& ma =
                                std::any_cast<const nlohmann::ordered_map<
                                    std::string, std::any>&>(a);
                            const auto& mb =
                                std::any_cast<const nlohmann::ordered_map<
                                    std::string, std::any>&>(b);
                            if (ma.size() != mb.size()) return false;
                            for (const auto& [k, va] : ma) {
                                auto it = mb.find(k);
                                if (it == mb.end()) return false;
                                if (!deepEqualsAny(va, it->second))
                                    return false;
                            }
                            return true;
                        }
                        // Compare arrays (Utils::JList and
                        // std::vector<std::any>)
                        auto toList =
                            [&](const std::any& v) -> std::vector<std::any> {
                            if (v.type() == typeid(Utils::JList)) {
                                const auto& jl =
                                    std::any_cast<const Utils::JList&>(v);
                                return std::vector<std::any>(jl.begin(),
                                                             jl.end());
                            } else if (v.type() ==
                                       typeid(std::vector<std::any>)) {
                                return std::any_cast<
                                    const std::vector<std::any>&>(v);
                            } else {
                                return {};
                            }
                        };
                        auto la = toList(a);
                        auto lb = toList(b);
                        if (!la.empty() || !lb.empty()) {
                            if (la.size() != lb.size()) return false;
                            for (size_t i = 0; i < la.size(); ++i) {
                                if (!deepEqualsAny(la[i], lb[i])) return false;
                            }
                            return true;
                        }
                        return false;
                    };

                    std::function<bool(
                        std::any&, const std::any&,
                        const nlohmann::ordered_map<std::string, std::any>&)>
                        applyUpdateToFirstMatch =
                            [&](std::any& node, const std::any& target,
                                const nlohmann::ordered_map<
                                    std::string, std::any>& updateMap) -> bool {
                        if (node.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>) &&
                            target.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>)) {
                            if (deepEqualsAny(node, target)) {
                                auto& m = std::any_cast<nlohmann::ordered_map<
                                    std::string, std::any>&>(node);
                                for (const auto& [k, v] : updateMap) {
                                    m[k] = v;
                                }
                                return true;
                            }
                            auto& m = std::any_cast<
                                nlohmann::ordered_map<std::string, std::any>&>(
                                node);
                            for (auto it = m.begin(); it != m.end(); ++it) {
                                if (applyUpdateToFirstMatch(it->second, target,
                                                            updateMap))
                                    return true;
                            }
                            return false;
                        }
                        // Traverse arrays
                        if (node.type() == typeid(Utils::JList)) {
                            auto& jl = std::any_cast<Utils::JList&>(node);
                            for (auto& item : jl) {
                                if (applyUpdateToFirstMatch(item, target,
                                                            updateMap))
                                    return true;
                            }
                            return false;
                        }
                        if (node.type() == typeid(std::vector<std::any>)) {
                            auto& vec =
                                std::any_cast<std::vector<std::any>&>(node);
                            for (auto& item : vec) {
                                if (applyUpdateToFirstMatch(item, target,
                                                            updateMap))
                                    return true;
                            }
                            return false;
                        }
                        return false;
                    };

                    std::function<bool(std::any&, const std::any&,
                                       const std::vector<std::string>&)>
                        applyDeleteToFirstMatch =
                            [&](std::any& node, const std::any& target,
                                const std::vector<std::string>& deletions)
                        -> bool {
                        if (node.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>) &&
                            target.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>)) {
                            if (deepEqualsAny(node, target)) {
                                auto& m = std::any_cast<nlohmann::ordered_map<
                                    std::string, std::any>&>(node);
                                for (const auto& key : deletions) {
                                    m.erase(key);
                                }
                                return true;
                            }
                            auto& m = std::any_cast<
                                nlohmann::ordered_map<std::string, std::any>&>(
                                node);
                            for (auto it = m.begin(); it != m.end(); ++it) {
                                if (applyDeleteToFirstMatch(it->second, target,
                                                            deletions))
                                    return true;
                            }
                            return false;
                        }
                        if (node.type() == typeid(Utils::JList)) {
                            auto& jl = std::any_cast<Utils::JList&>(node);
                            for (auto& item : jl) {
                                if (applyDeleteToFirstMatch(item, target,
                                                            deletions))
                                    return true;
                            }
                            return false;
                        }
                        if (node.type() == typeid(std::vector<std::any>)) {
                            auto& vec =
                                std::any_cast<std::vector<std::any>&>(node);
                            for (auto& item : vec) {
                                if (applyDeleteToFirstMatch(item, target,
                                                            deletions))
                                    return true;
                            }
                            return false;
                        }
                        return false;
                    };

                    // Process each match
                    for (size_t i = 0; i < matches.size(); ++i) {
                        auto match = matches[i];
                        // Evaluate update in the context of the match
                        auto updateValue =
                            instance->evaluate(update, match, environment);
                        if (updateValue.has_value()) {
                            if (updateValue.type() !=
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>)) {
                                throw JException("T2011", update->position,
                                                 updateValue);
                            }
                            const auto& updateMap =
                                std::any_cast<const nlohmann::ordered_map<
                                    std::string, std::any>&>(updateValue);
                            // Apply update to the first occurrence of match
                            // within result
                            applyUpdateToFirstMatch(result, match, updateMap);
                            // Align with Java semantics: in Java, 'match' is a
                            // direct reference into 'result' and thus reflects
                            // updates. Since our 'match' is a copy, merge the
                            // update into the local 'match' so subsequent
                            // delete uses a structure that deep-equals the
                            // updated node in 'result'.
                            if (match.type() ==
                                typeid(nlohmann::ordered_map<std::string,
                                                             std::any>)) {
                                auto& matchMap =
                                    std::any_cast<nlohmann::ordered_map<
                                        std::string, std::any>&>(match);
                                for (const auto& [k, v] : updateMap) {
                                    matchMap[k] = v;
                                }
                            }
                        }

                        if (delete_expr) {
                            auto deletions = instance->evaluate(
                                delete_expr, match, environment);
                            if (deletions.has_value()) {
                                auto val = deletions;
                                if (!Utils::isArray(deletions)) {
                                    std::vector<std::any> tmp;
                                    tmp.push_back(deletions);
                                    deletions = tmp;
                                }
                                auto deletionVec = Utils::arrayify(deletions);
                                std::vector<std::string> deletionList;
                                for (const auto& del : deletionVec) {
                                    if (del.type() != typeid(std::string)) {
                                        throw JException("T2012",
                                                         delete_expr->position,
                                                         val);
                                    }
                                    deletionList.push_back(
                                        std::any_cast<std::string>(del));
                                }
                                // Apply deletions to the first occurrence of
                                // match within result
                                applyDeleteToFirstMatch(result, match,
                                                        deletionList);
                            }
                        }
                    }
                }
            }

            return result;
        }

        // Java line 1887: var env = createFrame(proc.environment);
        std::shared_ptr<Frame> env;
        try {
            auto symbolEnv =
                std::any_cast<std::shared_ptr<Frame>>(symbol->environment);
            auto instance = getCurrentInstance();
            env = instance ? instance->createFrame(symbolEnv)
                           : std::make_shared<Frame>();
        } catch (const std::bad_any_cast&) {
            // Fallback if environment is not a Frame - this shouldn't normally
            // happen
            auto instance = getCurrentInstance();
            env = instance ? instance->createFrame(instance->getEnvironment())
                           : std::make_shared<Frame>();
        }

        // Java lines 1888-1891: bind arguments to parameter names
        for (size_t i = 0; i < symbol->arguments.size() && i < args.size();
             ++i) {
            if (symbol->arguments[i] &&
                symbol->arguments[i]->value.has_value()) {
                try {
                    auto paramName =
                        std::any_cast<std::string>(symbol->arguments[i]->value);
                    env->bind(paramName, args[i]);
                } catch (const std::bad_any_cast&) {
                    // Skip if not a string value
                }
            }
        }

        // Java lines 1892-1894: evaluate the body
        if (symbol->body) {
            std::any inputValue;
            if (symbol->input.has_value()) {
                inputValue = symbol->input;
            } else {
                inputValue = std::any{};
            }
            auto instance = getCurrentInstance();
            if (!instance) {
                throw JException("No current Jsonata instance available");
            }
            auto result = instance->evaluate(symbol->body, inputValue, env);
            return result;
        }

        return std::any{};
    } catch (const std::bad_any_cast&) {
        return std::any{};
    }
}

// Java reference lines 383-400: evaluateStages implementation
std::any Jsonata::evaluateStages(
    const std::vector<std::shared_ptr<Parser::Symbol>>& stages,
    const std::any& input, std::shared_ptr<Frame> environment) {
    std::any result = input;

    // Java lines 384-399: process each stage by type
    for (const auto& stage : stages) {
        if (!stage) continue;

        if (stage->type == "filter") {
            // Java line 389: result = /* await */ evaluateFilter(stage.expr,
            // result, environment);
            if (stage->expr.has_value()) {
                try {
                    auto filterExpr =
                        std::any_cast<std::shared_ptr<Parser::Symbol>>(
                            stage->expr);
                    result = evaluateFilter(filterExpr, result, environment);
                } catch (const std::bad_any_cast&) {
                    // Skip invalid filter expression
                }
            }
        } else if (stage->type == "index") {
            // Java lines 391-396: handle index stage for tuple streams
            try {
                // Handle tuple stream: vector<map<string, any>>
                if (result.type() == typeid(Utils::JList)) {
                    auto tupleList = std::any_cast<Utils::JList>(result);
                    for (size_t ee = 0; ee < tupleList.size(); ee++) {
                        if (stage->value.has_value()) {
                            std::string stageValue =
                                std::any_cast<std::string>(stage->value);
                            auto tuple = std::any_cast<
                                nlohmann::ordered_map<std::string, std::any>>(
                                tupleList[ee]);
                            tuple[stageValue] = static_cast<int64_t>(ee);
                            tupleList[ee] = std::any(tuple);
                        }
                    }
                    result = tupleList;
                } else {
                    // Handle regular list: vector<any>
                    auto resultList = Utils::arrayify(result);
                    for (size_t ee = 0; ee < resultList.size(); ee++) {
                        if (resultList[ee].type() ==
                            typeid(
                                nlohmann::ordered_map<std::string, std::any>)) {
                            auto tuple = std::any_cast<
                                nlohmann::ordered_map<std::string, std::any>>(
                                resultList[ee]);
                            if (stage->value.has_value()) {
                                std::string stageValue =
                                    std::any_cast<std::string>(stage->value);
                                tuple[stageValue] = static_cast<int64_t>(ee);
                                resultList[ee] = tuple;
                            }
                        }
                    }
                    result = resultList;
                }
            } catch (const std::bad_any_cast&) {
                // Skip if not the expected format
            }
        }
    }

    return result;
}

std::any Jsonata::evaluateStep(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment,
                               bool lastStep) {
    if (!expr || !input.has_value()) return std::any{};

    // Convert input to vector if needed
    Utils::JList inputSequence;
    if (Utils::isArray(input)) {
        inputSequence = Utils::arrayify(input);
    } else {
        inputSequence.push_back(input);
    }

    // Java reference lines 342-347: handle sort expressions specially
    if (expr->type == "sort") {
        auto result = evaluateSort(expr, input, environment);
        if (!expr->stages.empty()) {
            result = evaluateStages(expr->stages, result, environment);
        }
        return result;
    }

    // Java reference lines 350-362: evaluate expression for each item
    Utils::JList result = Utils::createSequence();
    for (const auto& item : inputSequence) {
        auto res = evaluate(expr, item, environment);

        // Apply stages (predicates) - Java lines 354-358
        if (!expr->stages.empty()) {
            for (const auto& stage : expr->stages) {
                if (stage && stage->expr.has_value()) {
                    try {
                        auto stageExpr =
                            std::any_cast<std::shared_ptr<Parser::Symbol>>(
                                stage->expr);
                        res = evaluateFilter(stageExpr, res, environment);
                    } catch (const std::bad_any_cast&) {
                        // Skip invalid stage
                    }
                }
            }
        }

        if (res.has_value()) {
            result.push_back(res);
        }
    }

    // Java reference lines 364-378: flatten the results
    Utils::JList resultSequence = Utils::createSequence();
    // Java reference line 365: if(lastStep && ((List)result).size()==1 &&
    // (((List)result).get(0) instanceof List) &&
    // !Utils.isSequence(((List)result).get(0)))
    if (lastStep && result.size() == 1 && Utils::isArray(result[0]) &&
        !Utils::isSequence(result[0])) {
        // Special case for last step with single array result
        auto vec = Utils::arrayify(result[0]);
        resultSequence = vec;
    } else {
        // Flatten the sequence
        for (const auto& res : result) {
            // Java reference line 370: if (!(res instanceof List) || (res
            // instanceof JList && ((JList)res).cons))
            if (!Utils::isArray(res)) {
                // it's not an array - just push into the result sequence
                resultSequence.push_back(res);
            } else if (res.type() == typeid(Utils::JList)) {
                auto jlist = std::any_cast<Utils::JList>(res);
                if (jlist.cons) {
                    // res is a JList with cons - push the whole JList
                    resultSequence.push_back(jlist);
                } else {
                    // res is a JList without cons - flatten it into the parent
                    // sequence
                    resultSequence.insert(resultSequence.end(), jlist.begin(),
                                          jlist.end());
                }
            } else {
                // res is a sequence - flatten it into the parent sequence
                auto nestedVec = Utils::arrayify(res);
                resultSequence.insert(resultSequence.end(), nestedVec.begin(),
                                      nestedVec.end());
            }
        }
    }

    return resultSequence.empty() ? std::any{} : std::any(resultSequence);
}

std::any Jsonata::evaluateTupleStep(
    std::shared_ptr<Parser::Symbol> expr, const Utils::JList& input,
    const std::optional<Utils::JList>& tupleBindings,
    std::shared_ptr<Frame> environment) {
    if (!expr) return std::any{};

    Utils::JList result;
    // Java reference lines 411-429: handle sort expressions
    if (expr->type == "sort") {
        if (tupleBindings.has_value()) {
            // Java line 414: sort existing tuple bindings
            std::any tupleBindingsAny = *tupleBindings;
            auto sortResult = evaluateSort(expr, tupleBindingsAny, environment);
            if (sortResult.has_value() &&
                sortResult.type() == typeid(Utils::JList)) {
                result = std::any_cast<Utils::JList>(sortResult);
            }
        } else {
            // Java lines 416-424: sort input and create tuples with index
            // binding
            std::any inputAny = input;
            auto sorted = evaluateSort(expr, inputAny, environment);
            result.sequence = true;
            result.tupleStream = true;
            if (sorted.has_value() && Utils::isArray(sorted)) {
                auto sortedVec = Utils::arrayify(sorted);
                for (size_t i = 0; i < sortedVec.size(); ++i) {
                    nlohmann::ordered_map<std::string, std::any> tuple;
                    tuple["@"] = sortedVec[i];
                    if (expr->index.has_value()) {
                        try {
                            std::string indexVar =
                                std::any_cast<std::string>(expr->index);
                            tuple[indexVar] = static_cast<int64_t>(i);
                        } catch (...) {
                            // Invalid index variable
                        }
                    }
                    result.push_back(std::any(tuple));
                }
            }
        }

        // Java lines 425-428: handle stages
        if (!expr->stages.empty()) {
            auto stagesResult =
                evaluateStages(expr->stages, result, environment);
            if (stagesResult.has_value() &&
                stagesResult.type() == typeid(Utils::JList)) {
                result = std::any_cast<Utils::JList>(stagesResult);
            }
        }

        return result;
    }

    // Java reference lines 431-436: create result sequence
    result.sequence = true;
    result.tupleStream = true;

    // Use existing tuple bindings or create from input
    auto bindings = tupleBindings.value_or(Utils::JList{});
    if (!tupleBindings.has_value() && !input.empty()) {
        // Java line 435: create initial tuple bindings (only when tupleBindings
        // is null, not just empty)
        for (const auto& item : input) {
            if (!item.has_value()) {
                continue;
            }
            nlohmann::ordered_map<std::string, std::any> tuple;
            tuple["@"] = item;
            bindings.push_back(std::any(tuple));
        }
    }

    // Java reference lines 438-472: process each tuple binding
    for (const auto& bindingAny : bindings) {
        auto binding =
            std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                bindingAny);
        // Create frame from tuple - Java line 439
        auto stepEnv = createFrameFromTuple(environment, bindingAny);

        // Evaluate expression with tuple's @ value - Java line 440
        auto atIt = binding.find("@");
        if (atIt == binding.end()) continue;

        auto res = evaluate(expr, atIt->second, stepEnv);

        if (res.has_value()) {
            // Convert res to vector if not already
            Utils::JList resVec;
            if (!Utils::isArray(res)) {
                resVec.push_back(res);
            } else if (res.type() == typeid(Utils::JList)) {
                resVec = std::any_cast<Utils::JList>(res);
            } else {
                resVec = Utils::arrayify(res);
            }

            // Java lines 449-469: create output tuples
            for (size_t i = 0; i < resVec.size(); ++i) {
                nlohmann::ordered_map<std::string, std::any> tuple(
                    binding);  // Copy existing bindings

                if (resVec.tupleStream) {
                    // cast resVec[i] to a map and overwrite existing keys
                    // (match Java Map.putAll semantics)
                    auto resTuple = std::any_cast<
                        nlohmann::ordered_map<std::string, std::any>>(
                        resVec[i]);
                    for (const auto& kv : resTuple) {
                        tuple[kv.first] = kv.second;
                    }
                } else {
                    if (expr->focus.has_value()) {
                        // Java lines 456-458: bind focus variable
                        try {
                            std::string focusVar =
                                std::any_cast<std::string>(expr->focus);
                            tuple[focusVar] = resVec[i];
                            tuple["@"] = binding.at("@");  // Keep original @
                        } catch (...) {
                            // Invalid focus variable
                        }
                    } else {
                        // Java line 460: update @ value
                        tuple["@"] = resVec[i];
                    }

                    if (expr->index.has_value()) {
                        // Java line 463: bind index variable
                        try {
                            std::string indexVar =
                                std::any_cast<std::string>(expr->index);
                            tuple[indexVar] = static_cast<int64_t>(i);
                        } catch (...) {
                            // Invalid index variable
                        }
                    }

                    // Java line 465-467: bind ancestor context - CRITICAL FOR
                    // PARENT OPERATOR
                    if (expr->ancestor) {
                        tuple[expr->ancestor->label] = binding.at("@");
                    }
                }

                result.push_back(tuple);
            }
        }
    }

    // Java lines 474-476: handle stages
    if (!expr->stages.empty()) {
        auto stagesResult = evaluateStages(expr->stages, result, environment);
        if (stagesResult.has_value() &&
            stagesResult.type() == typeid(Utils::JList)) {
            result = std::any_cast<Utils::JList>(stagesResult);
        }
    }

    return result;
}

std::any Jsonata::evaluateGroupExpression(std::shared_ptr<Parser::Symbol> expr,
                                          const std::any& input,
                                          std::shared_ptr<Frame> environment) {
    if (!expr) return std::any{};

    // Port exact Java implementation: Jsonata.java lines 1051-1134
    nlohmann::ordered_map<std::string, std::any> result;

    // C++ equivalent of Java's LinkedHashMap<Object,GroupEntry>
    struct GroupEntry {
        std::any data;
        int64_t exprIndex;
    };
    nlohmann::ordered_map<std::string, GroupEntry> groups;

    // Java line 1054: var reduce = (_input instanceof JList) &&
    // ((JList)_input).tupleStream ? true : false; For now, simplify this - the
    // main case is non-tuple streams
    bool reduce = false;
    if (input.type() == typeid(Utils::JList)) {
        auto jlist = std::any_cast<Utils::JList>(input);
        reduce = jlist.tupleStream;
    }

    // Java lines 1056-1059: group the input sequence by "key" expression
    Utils::JList inputVec;
    if (!Utils::isArray(input)) {
        inputVec = Utils::createSequence(input);
    } else {
        // Match Java semantics for any array-like input
        // Convert to JList via Utils::arrayify to support both JList and
        // std::vector inputs
        inputVec = Utils::arrayify(input);
    }

    // Java lines 1062-1064: if the array is empty, add an undefined entry to
    // enable literal JSON object to be generated
    if (inputVec.empty()) {
        inputVec.push_back(std::any{});
    }

    // Java lines 1066-1103: Process each item and each key-value pair
    for (size_t itemIndex = 0; itemIndex < inputVec.size(); itemIndex++) {
        auto item = inputVec[itemIndex];
        // Java line 1068: var env = reduce ? createFrameFromTuple(environment,
        // (Map)item) : environment;
        auto env =
            reduce
                ? createFrameFromTuple(
                      environment,
                      std::any_cast<
                          nlohmann::ordered_map<std::string, std::any>>(item))
                : environment;

        for (size_t pairIndex = 0; pairIndex < expr->lhsObject.size();
             pairIndex++) {
            auto pair = expr->lhsObject[pairIndex];

            // Java line 1071: var key = evaluate(pair[0], reduce ?
            // ((Map)item).get("@") : item, env);
            std::any keyContext = item;
            if (reduce) {
                auto itemMap =
                    std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                        item);
                auto atIt = itemMap.find("@");
                keyContext =
                    (atIt != itemMap.end()) ? atIt->second : std::any{};
            }
            auto key = evaluate(pair.first, keyContext, env);

            // Java lines 1072-1079: key has to be a string - T1003 validation
            // Java: if (key!=null && !(key instanceof String)) throw
            // JException("T1003")
            if (key.has_value() && key.type() != typeid(std::string)) {
                throw JException(
                    "T1003", expr->position,
                    "Key in object structure must evaluate to a string");
            }

            // If key is null, skip processing (Java line 1081: if (key !=
            // null))
            if (!key.has_value()) {
                continue;
            }

            // Key is validated to be a string at this point
            std::string keyStr = std::any_cast<std::string>(key);

            // Java lines 1081-1101: Process non-null keys
            GroupEntry entry;
            entry.data = item;
            entry.exprIndex = static_cast<int64_t>(pairIndex);

            auto groupsIt = groups.find(keyStr);
            if (groupsIt != groups.end()) {
                // Java lines 1084-1094: a value already exists in this slot
                if (groupsIt->second.exprIndex != static_cast<int64_t>(pairIndex)) {
                    // Java lines 1086-1093: this key has been generated by
                    // another expression in this group when multiple key
                    // expressions evaluate to the same key, then error D1009
                    // must be thrown
                    throw JException(
                        "D1009", expr->position,
                        "Multiple key definitions evaluate to same key");
                }

                // Java line 1097: append it as an array
                Utils::JList appendArgs = {groupsIt->second.data, item};
                groupsIt->second.data = Functions::append(appendArgs);
            } else {
                groups[keyStr] = entry;
            }
        }
    }

    // Java lines 1105-1125: iterate over the groups to evaluate the "value"
    // expression
    int64_t idx = 0;
    for (const auto& kvp : groups) {
        const auto& keyStr = kvp.first;
        const auto& entry = kvp.second;
        auto context = entry.data;
        auto env = environment;

        // Java lines 1112-1117: if (reduce) handle tuple reduction and create
        // frame
        if (reduce) {
            auto tuple = reduceTupleStream(entry.data);
            auto tupleMap =
                std::any_cast<nlohmann::ordered_map<std::string, std::any>>(
                    tuple);
            auto atIt = tupleMap.find("@");
            context = (atIt != tupleMap.end()) ? atIt->second : std::any{};
            tupleMap.erase("@");  // Java line 1115: ((Map)tuple).remove("@");
            env = createFrameFromTuple(environment, tupleMap);
        }

        // Java line 1118: env.isParallelCall = idx > 0;
        env->isParallelCall = (idx > 0);

        // Java line 1120: Object res =
        // evaluate(expr.lhsObject.get(entry.exprIndex)[1], context, env);
        auto res =
            evaluate(expr->lhsObject[entry.exprIndex].second, context, env);

        // Java lines 1121-1122: if (res!=null) result.put(e.getKey(), res);
        if (res.has_value()) {
            result[keyStr] = res;
        }

        idx++;
    }

    return result;
}

std::any Jsonata::evaluateTransformExpression(
    std::shared_ptr<Parser::Symbol> expr, const std::any& input,
    std::shared_ptr<Frame> environment) {
    if (!expr) return std::any{};

    // Create a transformer function - based on Java implementation lines
    // 1419-1436 This returns a function that can be called later with apply()

    auto transformer = std::make_shared<Parser::Symbol>();
    transformer->type = "lambda";
    transformer->_jsonata_lambda = true;
    transformer->_transform_expr = expr;  // Store the transform expression

    // Create a dummy argument for the function signature
    auto dummyArg = std::make_shared<Parser::Symbol>();
    dummyArg->value = std::string("obj");
    transformer->arguments.push_back(dummyArg);

    // Store the closure environment
    transformer->value = nlohmann::ordered_map<std::string, std::any>{
        {"input", input}, {"environment", environment}};

    return transformer;
}

std::any Jsonata::evaluateApplyExpression(std::shared_ptr<Parser::Symbol> expr,
                                          const std::any& input,
                                          std::shared_ptr<Frame> environment) {
    if (!expr) return std::any{};

    // Apply the Object on the RHS using the sequence on the LHS as the first
    // argument Based on Java implementation lines 1512-1548

    if (!expr->lhs || !expr->rhs) {
        return std::any{};
    }

    auto lhs = evaluate(expr->lhs, input, environment);

    if (expr->rhs->type == "function") {
        // This is a function invocation; invoke it with lhs expression as the
        // first argument
        return evaluateFunctionWithContext(expr->rhs, input, environment, lhs);
    } else {
        auto func = evaluate(expr->rhs, input, environment);

        if (!isFunctionLike(func) && !isFunctionLike(lhs)) {
            throw JException("T2006", expr->position, func);
        }

        if (isFunctionLike(lhs)) {
            // This is function chaining (func1 ~> func2)
            // λ($f, $g) { λ($x){ $g($f($x)) } }
            auto chain = evaluate(getChainAST(), std::any{}, environment);
            Utils::JList args = {lhs, func};
            return apply(chain, args, std::any{}, environment);
        } else {
            Utils::JList args = {lhs};
            return apply(func, args, std::any{}, environment);
        }
    }
}

bool Jsonata::isFunctionLike(const std::any& o) const {
    // Java reference line 1551: return Utils.isFunction(o) ||
    // Functions.isLambda(o) || (o instanceof Pattern);
    if (Utils::isFunction(o) || Functions::isLambda(o)) {
        return true;
    }

    // Check if it's a regex pattern (C++ equivalent of "o instanceof Pattern")
    if (o.has_value() && o.type() == typeid(std::regex)) {
        return true;
    }

    return false;
}

std::any Jsonata::evaluatePartialApplication(
    std::shared_ptr<Parser::Symbol> expr, const std::any& input,
    std::shared_ptr<Frame> environment) {
    // Following Java implementation: Jsonata.java lines 1820-1856
    // Following Java implementation: Jsonata.java lines 1820-1856
    if (!expr) return std::any{};

    // Evaluate the arguments (Java lines 1824-1832)
    Utils::JList evaluatedArgs;
    for (size_t ii = 0; ii < expr->arguments.size(); ii++) {
        auto arg = expr->arguments[ii];
        if (arg && arg->type == "operator" &&
            std::any_cast<std::string>(arg->value) == "?") {
            // Keep placeholder arguments as-is (Java line 1828)
            evaluatedArgs.push_back(arg);
        } else {
            // Evaluate non-placeholder arguments (Java line 1830)
            evaluatedArgs.push_back(evaluate(arg, input, environment));
        }
    }

    // Lookup the procedure (Java line 1834)
    auto proc = evaluate(expr->procedure, input, environment);

    // Check if procedure exists and handle error cases (Java lines 1835-1841)
    if (!proc.has_value() && expr->procedure &&
        expr->procedure->type == "path" && !expr->procedure->steps.empty() &&
        environment
            ->lookup(
                std::any_cast<std::string>(expr->procedure->steps[0]->value))
            .has_value()) {
        throw JException(
            "T1007", expr->position,
            std::any_cast<std::string>(expr->procedure->steps[0]->value));
    }

    std::any result;
    // Validate procedure type (Java lines 1842-1854)

    if (Functions::isLambda(proc)) {
        // Java line 1843: partialApplyProcedure
        result = partialApplyProcedure(
            std::any_cast<std::shared_ptr<Parser::Symbol>>(proc),
            evaluatedArgs);
    } else if (Utils::isFunction(proc)) {
        // Java line 1845: partialApplyNativeFunction
        // Get function name from the procedure expression
        std::string functionName;
        if (expr->procedure && expr->procedure->type == "path" &&
            !expr->procedure->steps.empty()) {
            functionName =
                std::any_cast<std::string>(expr->procedure->steps[0]->value);
        } else if (expr->procedure && expr->procedure->value.has_value()) {
            functionName = std::any_cast<std::string>(expr->procedure->value);
        }
        result = partialApplyNativeFunction(proc, evaluatedArgs, functionName);
    } else {
        // Java lines 1849-1854
        std::string procName = "unknown";
        if (expr->procedure && expr->procedure->type == "path" &&
            !expr->procedure->steps.empty()) {
            procName =
                std::any_cast<std::string>(expr->procedure->steps[0]->value);
        } else if (expr->procedure) {
            procName = std::any_cast<std::string>(expr->procedure->value);
        }
        throw JException("T1008", expr->position, procName);
    }

    return result;
}

std::any Jsonata::partialApplyProcedure(std::shared_ptr<Parser::Symbol> proc,
                                        const Utils::JList& args) {
    // Following Java implementation: Jsonata.java lines 1907-1931
    // Create a closure, bind the supplied parameters and return a function that
    // takes the remaining (?) parameters

    auto env = createFrame(
        proc->environment.has_value()
            ? std::any_cast<std::shared_ptr<Frame>>(proc->environment)
            : getEnvironment());
    std::vector<std::shared_ptr<Parser::Symbol>> unboundArgs;

    size_t index = 0;
    for (auto param : proc->arguments) {
        std::any arg = (index < args.size()) ? args[index] : std::any{};

        // Check if argument is placeholder or missing (Java lines 1916-1922)
        bool isPlaceholder = false;
        if (!arg.has_value()) {
            isPlaceholder = true;
        } else if (arg.type() == typeid(std::shared_ptr<Parser::Symbol>)) {
            auto argSymbol =
                std::any_cast<std::shared_ptr<Parser::Symbol>>(arg);
            if (argSymbol && argSymbol->type == "operator" &&
                std::any_cast<std::string>(argSymbol->value) == "?") {
                isPlaceholder = true;
            }
        }

        if (isPlaceholder) {
            unboundArgs.push_back(param);
        } else {
            env->bind(std::any_cast<std::string>(param->value), arg);
        }
        index++;
    }

    // Create new procedure (Java lines 1923-1930)
    auto procedure = std::make_shared<Parser::Symbol>("");
    procedure->_jsonata_lambda = true;
    procedure->input = proc->input;
    procedure->environment = env;
    procedure->arguments = unboundArgs;
    procedure->body = proc->body;

    // Copy signature from original procedure to maintain signature validation
    // This is critical for partial application to work correctly with
    // signatures
    procedure->signature = proc->signature;

    return procedure;
}

std::any Jsonata::partialApplyNativeFunction(const std::any& native,
                                             const Utils::JList& args,
                                             const std::string& functionName) {
    // Following Java implementation: Jsonata.java lines 1939-1974
    // Create a lambda function that wraps and invokes the native function

    // Get the number of arguments for this function from its signature
    int64_t numberOfArgs = 2;  // Default for most functions
    if (native.type() == typeid(JFunction)) {
        auto jfunc = std::any_cast<JFunction>(native);
        if (jfunc.signature) {
            numberOfArgs = jfunc.signature->getNumberOfArgs();
        }
    }

    // Build the parameter list and argument list (Java lines 1947-1956)
    std::vector<std::string> sigArgs;
    std::vector<std::string> callArgs;
    for (int64_t i = 0; i < numberOfArgs; i++) {
        std::string argName = "$" + std::string(1, static_cast<char>('a' + i));
        sigArgs.push_back(argName);

        // Check if this argument is provided or needs to be a parameter
        if (i < args.size() && args[i].has_value()) {
            // Check if it's a placeholder
            if (args[i].type() == typeid(std::shared_ptr<Parser::Symbol>)) {
                auto argSymbol =
                    std::any_cast<std::shared_ptr<Parser::Symbol>>(args[i]);
                if (argSymbol && argSymbol->type == "operator" &&
                    std::any_cast<std::string>(argSymbol->value) == "?") {
                    callArgs.push_back(argName);  // Use parameter name
                } else {
                    callArgs.push_back(argName);  // For now, treat as parameter
                }
            } else {
                callArgs.push_back(argName);  // Will be bound later
            }
        } else {
            callArgs.push_back(argName);  // Use parameter name
        }
    }

    // Build the body string (Java lines 1958-1959)
    std::string body = "function(";
    for (size_t i = 0; i < sigArgs.size(); i++) {
        if (i > 0) body += ", ";
        body += sigArgs[i];
    }
    body += "){$" + functionName + "(";
    for (size_t i = 0; i < callArgs.size(); i++) {
        if (i > 0) body += ", ";
        body += callArgs[i];
    }
    body += ") }";

    // Parse the body to create AST (Java line 1969)
    Parser parser;
    auto bodyAST = parser.parse(body);

    // Use partialApplyProcedure with the parsed body (Java line 1972)
    return partialApplyProcedure(
        std::any_cast<std::shared_ptr<Parser::Symbol>>(bodyAST), args);
}

}  // namespace jsonata
