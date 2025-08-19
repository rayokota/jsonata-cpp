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
 * © Copyright IBM Corp. 2016, 2017 All Rights Reserved
 *   Project name: JSONata
 *   This project is licensed under the MIT License, see LICENSE
 */
#pragma once

#include <any>
#include <chrono>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include "jsonata/Parser.h"

// Forward declarations
namespace jsonata {
class JException;
namespace utils {
class Signature;
}
namespace json {
struct JsonValue;
}
}  // namespace jsonata

namespace jsonata {

// Forward declaration for callback types
class Frame;
using EntryCallback = std::function<void(
    std::shared_ptr<Parser::Symbol>, const std::any&, std::shared_ptr<Frame>)>;
using ExitCallback =
    std::function<void(std::shared_ptr<Parser::Symbol>, const std::any&,
                       std::shared_ptr<Frame>, const std::any&)>;

/**
 * Frame class for variable bindings and scope management
 */
class Frame {
  private:
    std::shared_ptr<Frame> parent_;
    nlohmann::ordered_map<std::string, std::any> bindings_;
    std::chrono::time_point<std::chrono::steady_clock> timestamp_;
    int64_t timeout_;
    int64_t recursionDepth_;
    EntryCallback entryCallback_;
    ExitCallback exitCallback_;
    std::unique_ptr<class Timebox> timebox_;

  public:
    bool isParallelCall = false;

  public:
    // Constructors
    Frame();
    Frame(std::shared_ptr<Frame> parent);

    // Variable binding and lookup
    void bind(const std::string& name, const std::any& value);
    std::any lookup(const std::string& name) const;

    // Runtime bounds
    void setRuntimeBounds(int64_t timeout, int64_t maxRecursionDepth);

    // Evaluation callbacks
    void setEvaluateEntryCallback(EntryCallback callback);
    void setEvaluateExitCallback(ExitCallback callback);

    // Parent access
    std::shared_ptr<Frame> getParent() const { return parent_; }

    // Bindings access
    const nlohmann::ordered_map<std::string, std::any>& getBindings() const {
        return bindings_;
    }
};

/**
 * Function types for JSONata functions
 */
class JFunction {
  public:
    std::function<std::any(const Utils::JList&, const std::any&,
                           std::shared_ptr<Frame>)>
        implementation;
    std::shared_ptr<utils::Signature> signature;

    JFunction() = default;
    JFunction(std::function<std::any(const Utils::JList&, const std::any&,
                                     std::shared_ptr<Frame>)>
                  impl)
        : implementation(impl) {}

    virtual ~JFunction() = default;
};

/**
 * Main Jsonata evaluator class
 */
class Jsonata {
  public:
    // Constructors
    Jsonata();
    Jsonata(const std::string& jsonataExpression);
    Jsonata(const Jsonata& other);  // Copy constructor for per-thread instances

    // Main evaluation methods (ordered JSON variants)
    nlohmann::ordered_json evaluate(const nlohmann::ordered_json& input);
    nlohmann::ordered_json evaluate(const nlohmann::ordered_json& input,
                                    std::shared_ptr<Frame> bindings);
    nlohmann::ordered_json evaluate(std::nullptr_t);
    nlohmann::ordered_json evaluate(std::nullptr_t,
                                    std::shared_ptr<Frame> bindings);

    // Main evaluation methods (unordered nlohmann::json variants)
    nlohmann::json evaluate(const nlohmann::json& input);
    nlohmann::json evaluate(const nlohmann::json& input,
                            std::shared_ptr<Frame> bindings);
    nlohmann::json evaluateUnordered(std::nullptr_t);
    nlohmann::json evaluateUnordered(std::nullptr_t,
                                     std::shared_ptr<Frame> bindings);
    std::any evaluate(std::shared_ptr<Parser::Symbol> expr,
                      const std::any& input,
                      std::shared_ptr<Frame> environment);

    // Environment access
    std::shared_ptr<Frame> getEnvironment() const;

    // Factory methods
    static Jsonata jsonata(const std::string& expression);

    // Instance methods (matching Java reference)
    std::shared_ptr<Frame> createFrame();
    std::shared_ptr<Frame> createFrame(
        std::shared_ptr<Frame> enclosingEnvironment);

    // Parse expression
    std::shared_ptr<Parser::Symbol> parse(const std::string& expression);

    // Debug helper
    std::shared_ptr<Parser::Symbol> getExpression() const {
        return expression_;
    }

    // Lambda function application support
    std::any apply(const std::any& lambda, const Utils::JList& args,
                   const std::any& input, std::shared_ptr<Frame> environment);
    std::any applyInner(const std::any& proc, const Utils::JList& args,
                        const std::any& input,
                        std::shared_ptr<Frame> environment);
    std::any applyProcedure(const std::any& proc, const Utils::JList& args);
    Utils::JList validateArguments(const std::any& signature,
                                   const Utils::JList& args,
                                   const std::any& context);

    // Partial application support (matching Java implementation)
    std::any partialApplyProcedure(std::shared_ptr<class Parser::Symbol> proc,
                                   const Utils::JList& args);
    std::any partialApplyNativeFunction(const std::any& native,
                                        const Utils::JList& args,
                                        const std::string& functionName);

    // Thread-local instances (matching Java implementation)
    static thread_local Jsonata* currentInstance_;
    // Owns the per-thread clone created by getPerThreadInstance(); ensures
    // deletion on thread exit and avoids manual cleanup.
    static thread_local std::unique_ptr<Jsonata> ownedInstance_;
    // Optional: explicitly clear the per-thread instance before thread exit.
    static void clearPerThreadInstance();

    std::shared_ptr<Frame> environment_;
    static thread_local std::shared_ptr<class Parser> currentParser_;
    static thread_local std::any tls_input_;
    static thread_local std::shared_ptr<Frame> tls_environment_;
    static Jsonata* getCurrentInstance();
    static std::shared_ptr<class Parser> getCurrentParser();
    Jsonata* getPerThreadInstance();

    // Public accessors for thread-local context (used by Functions)
    const std::any& getCurrentInput() const { return tls_input_; }
    std::shared_ptr<Frame> getCurrentEnvironment() const {
        return tls_environment_;
    }

    // Missing public API methods from Java
    void assign(const std::string& name, const std::any& value);
    void registerFunction(const std::string& name,
                          const JFunction& implementation);
    void registerFunction(
        const std::string& name,
        std::function<std::any(const Utils::JList&)> implementation);

    // Type-safe function registration overloads (equivalent to Java's Fn0<R>,
    // Fn1<A,R>, etc.)
    template <typename R>
    void registerFunction(const std::string& name,
                          std::function<R()> implementation);

    template <typename A, typename R>
    void registerFunction(const std::string& name,
                          std::function<R(A)> implementation);

    template <typename A, typename B, typename R>
    void registerFunction(const std::string& name,
                          std::function<R(A, B)> implementation);

    template <typename A, typename B, typename C, typename R>
    void registerFunction(const std::string& name,
                          std::function<R(A, B, C)> implementation);

    template <typename A, typename B, typename C, typename D, typename R>
    void registerFunction(const std::string& name,
                          std::function<R(A, B, C, D)> implementation);

  public:
    // Function-like detection (Java reference: line 1551)
    bool isFunctionLike(const std::any& o) const;

    // Input validation control
    bool isValidateInput() const;
    void setValidateInput(bool validateInput);

    // Error handling
    std::vector<std::exception_ptr> getErrors() const;

    // Utility methods
    static bool boolize(const std::any& value);

  public:
    std::unique_ptr<Parser> parser_;

    static std::shared_ptr<Frame> staticFrame_;
    static std::shared_ptr<Frame> getStaticFrame();
    static void initializeBuiltinFunctions(std::shared_ptr<Frame> frame);

    // New members for string constructor
    std::shared_ptr<Parser::Symbol> expression_;

    bool validateInput_ = false;
    std::vector<std::exception_ptr> errors_;
    int64_t timestamp_ = 0;

    // Current evaluation context is stored in thread-local variables

    // GroupEntry structure for object grouping (matches Java implementation)
    struct GroupEntry {
        std::any data;
        size_t exprIndex;
    };

    // Core evaluation method
    std::any _evaluate(std::shared_ptr<Parser::Symbol> expr,
                       const std::any& input,
                       std::shared_ptr<Frame> environment);

    // Expression evaluation methods
    std::any evaluateLiteral(std::shared_ptr<Parser::Symbol> expr);
    std::any evaluateName(std::shared_ptr<Parser::Symbol> expr,
                          const std::any& input,
                          std::shared_ptr<Frame> environment);
    std::any evaluateVariable(std::shared_ptr<Parser::Symbol> expr,
                              const std::any& input,
                              std::shared_ptr<Frame> environment);
    std::any evaluateBinary(std::shared_ptr<Parser::Symbol> expr,
                            const std::any& input,
                            std::shared_ptr<Frame> environment);
    std::any evaluateUnary(std::shared_ptr<Parser::Symbol> expr,
                           const std::any& input,
                           std::shared_ptr<Frame> environment);
    std::any evaluateFunction(std::shared_ptr<Parser::Symbol> expr,
                              const std::any& input,
                              std::shared_ptr<Frame> environment);
    std::any evaluateFunctionWithContext(std::shared_ptr<Parser::Symbol> expr,
                                         const std::any& input,
                                         std::shared_ptr<Frame> environment,
                                         const std::any& applytoContext);
    std::any evaluateRegex(std::shared_ptr<Parser::Symbol> expr,
                           const std::any& input,
                           std::shared_ptr<Frame> environment);
    std::any evaluateWildcard(std::shared_ptr<Parser::Symbol> expr,
                              const std::any& input);
    std::any flatten(const std::any& arg, Utils::JList* flattened = nullptr);
    std::any evaluateFilter(std::shared_ptr<Parser::Symbol> predicate,
                            const std::any& input,
                            std::shared_ptr<Frame> environment);
    std::any evaluatePath(std::shared_ptr<Parser::Symbol> expr,
                          const std::any& input,
                          std::shared_ptr<Frame> environment);
    std::any evaluateCondition(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment);
    std::any evaluateDescendant(std::shared_ptr<Parser::Symbol> expr,
                                const std::any& input,
                                std::shared_ptr<Frame> environment);
    void recurseDescendants(const std::any& input,
                            std::vector<std::any>& results);
    std::any evaluateApply(std::shared_ptr<Parser::Symbol> expr,
                           const std::any& input,
                           std::shared_ptr<Frame> environment);
    std::any evaluateRange(std::shared_ptr<Parser::Symbol> expr,
                           const std::any& input,
                           std::shared_ptr<Frame> environment);
    std::any evaluateBind(std::shared_ptr<Parser::Symbol> expr,
                          const std::any& input,
                          std::shared_ptr<Frame> environment);
    std::any evaluateLambda(std::shared_ptr<Parser::Symbol> expr,
                            const std::any& input,
                            std::shared_ptr<Frame> environment);
    std::any evaluateSort(std::shared_ptr<Parser::Symbol> expr,
                          const std::any& input,
                          std::shared_ptr<Frame> environment);
    std::any evaluateTransform(std::shared_ptr<Parser::Symbol> expr,
                               const std::any& input,
                               std::shared_ptr<Frame> environment);
    std::any evaluateParent(std::shared_ptr<Parser::Symbol> expr,
                            const std::any& input,
                            std::shared_ptr<Frame> environment);
    std::any evaluateBlock(std::shared_ptr<Parser::Symbol> expr,
                           const std::any& input,
                           std::shared_ptr<Frame> environment);

    // Binary operation evaluators
    std::any evaluateNumericExpression(const std::any& lhs, const std::any& rhs,
                                       const std::string& op);
    std::any evaluateComparisonExpression(const std::any& lhs,
                                          const std::any& rhs,
                                          const std::string& op);
    bool deepEquals(const std::any& lhs, const std::any& rhs);
    std::any evaluateEqualityExpression(const std::any& lhs,
                                        const std::any& rhs,
                                        const std::string& op);
    std::any evaluateBooleanExpression(const std::any& lhs,
                                       std::function<std::any()> rhs,
                                       const std::string& op);
    std::any evaluateStringConcat(const std::any& lhs, const std::any& rhs);
    std::any evaluateRangeExpression(const std::any& lhs, const std::any& rhs);
    std::any evaluateIncludesExpression(const std::any& lhs,
                                        const std::any& rhs);

    // Missing advanced evaluation methods from Java
    std::any evaluateStages(
        const std::vector<std::shared_ptr<Parser::Symbol>>& stages,
        const std::any& input, std::shared_ptr<Frame> environment);
    std::any evaluateStep(std::shared_ptr<Parser::Symbol> expr,
                          const std::any& input,
                          std::shared_ptr<Frame> environment,
                          bool lastStep = false);
    std::any evaluateTupleStep(std::shared_ptr<Parser::Symbol> expr,
                               const Utils::JList& input,
                               const std::optional<Utils::JList>& tupleBindings,
                               std::shared_ptr<Frame> environment);
    std::any evaluateGroupExpression(std::shared_ptr<Parser::Symbol> expr,
                                     const std::any& input,
                                     std::shared_ptr<Frame> environment);
    std::any evaluateTransformExpression(std::shared_ptr<Parser::Symbol> expr,
                                         const std::any& input,
                                         std::shared_ptr<Frame> environment);
    std::any evaluateApplyExpression(std::shared_ptr<Parser::Symbol> expr,
                                     const std::any& input,
                                     std::shared_ptr<Frame> environment);
    std::any evaluatePartialApplication(std::shared_ptr<Parser::Symbol> expr,
                                        const std::any& input,
                                        std::shared_ptr<Frame> environment);

    std::shared_ptr<Frame> createFrameFromTuple(
        std::shared_ptr<Frame> environment, const std::any& tupleAny);
    std::any reduceTupleStream(const std::any& tupleStream);

    // Conversion helpers between engine types and JSON
    // Ordered variants preserve insertion order using nlohmann::ordered_json
    static std::any orderedJsonToAny(const nlohmann::ordered_json& j);
    static nlohmann::ordered_json anyToOrderedJson(const std::any& value);
    // Unordered variants using nlohmann::json (key order may not be preserved)
    static std::any jsonToAny(const nlohmann::json& j);
    static nlohmann::json anyToJson(const std::any& value);

    // Error code mappings
    void initializeErrorCodes();
};

// Global static methods
std::any jsonata_evaluate(const std::string& expression, const std::any& input);

// Template implementations (must be in header for templates)

template <typename R>
void Jsonata::registerFunction(const std::string& name,
                               std::function<R()> implementation) {
    auto wrapper = [implementation](const std::vector<std::any>&) -> std::any {
        if constexpr (std::is_void_v<R>) {
            implementation();
            return std::any{};
        } else {
            return std::any(implementation());
        }
    };
    registerFunction(name, wrapper);
}

template <typename A, typename R>
void Jsonata::registerFunction(const std::string& name,
                               std::function<R(A)> implementation) {
    auto wrapper = [implementation,
                    name](const std::vector<std::any>& args) -> std::any {
        if (args.empty()) {
            throw JException("S0410", -1,
                             "Function " + name + " expects 1 argument, got 0");
        }
        A arg = std::any_cast<A>(args[0]);
        if constexpr (std::is_void_v<R>) {
            implementation(arg);
            return std::any{};
        } else {
            return std::any(implementation(arg));
        }
    };
    registerFunction(name, wrapper);
}

template <typename A, typename B, typename R>
void Jsonata::registerFunction(const std::string& name,
                               std::function<R(A, B)> implementation) {
    auto wrapper = [implementation,
                    name](const std::vector<std::any>& args) -> std::any {
        if (args.size() < 2) {
            throw JException("S0410", -1,
                             "Function " + name + " expects 2 arguments, got " +
                                 std::to_string(args.size()));
        }
        A arg1 = std::any_cast<A>(args[0]);
        B arg2 = std::any_cast<B>(args[1]);
        if constexpr (std::is_void_v<R>) {
            implementation(arg1, arg2);
            return std::any{};
        } else {
            return std::any(implementation(arg1, arg2));
        }
    };
    registerFunction(name, wrapper);
}

template <typename A, typename B, typename C, typename R>
void Jsonata::registerFunction(const std::string& name,
                               std::function<R(A, B, C)> implementation) {
    auto wrapper = [implementation,
                    name](const std::vector<std::any>& args) -> std::any {
        if (args.size() < 3) {
            throw JException("S0410", -1,
                             "Function " + name + " expects 3 arguments, got " +
                                 std::to_string(args.size()));
        }
        A arg1 = std::any_cast<A>(args[0]);
        B arg2 = std::any_cast<B>(args[1]);
        C arg3 = std::any_cast<C>(args[2]);
        if constexpr (std::is_void_v<R>) {
            implementation(arg1, arg2, arg3);
            return std::any{};
        } else {
            return std::any(implementation(arg1, arg2, arg3));
        }
    };
    registerFunction(name, wrapper);
}

template <typename A, typename B, typename C, typename D, typename R>
void Jsonata::registerFunction(const std::string& name,
                               std::function<R(A, B, C, D)> implementation) {
    auto wrapper = [implementation,
                    name](const std::vector<std::any>& args) -> std::any {
        if (args.size() < 4) {
            throw JException("S0410", -1,
                             "Function " + name + " expects 4 arguments, got " +
                                 std::to_string(args.size()));
        }
        A arg1 = std::any_cast<A>(args[0]);
        B arg2 = std::any_cast<B>(args[1]);
        C arg3 = std::any_cast<C>(args[2]);
        D arg4 = std::any_cast<D>(args[3]);
        if constexpr (std::is_void_v<R>) {
            implementation(arg1, arg2, arg3, arg4);
            return std::any{};
        } else {
            return std::any(implementation(arg1, arg2, arg3, arg4));
        }
    };
    registerFunction(name, wrapper);
}

}  // namespace jsonata
