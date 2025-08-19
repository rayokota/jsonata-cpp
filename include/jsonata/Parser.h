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
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "jsonata/JException.h"
#include "jsonata/Tokenizer.h"
#include "jsonata/utils/Signature.h"

namespace jsonata {

// Forward declarations
class Frame;
class Signature;

/**
 * Parser implementing the 'Top down operator precedence' algorithm
 * developed by Vaughan R Pratt and builds on the framework described
 * by Douglas Crockford.
 */
class Parser {
  public:
    // AST Node representation
    class Symbol : public std::enable_shared_from_this<Symbol> {
      public:
        std::string id;
        std::string type;
        std::any value;
        std::any token;
        int64_t lbp = 0;
        int64_t bp = 0;
        int64_t position = 0;

        // Expression structure
        std::shared_ptr<Symbol> lhs;
        std::shared_ptr<Symbol> rhs;
        std::shared_ptr<Symbol> expression;  // for unary
        std::shared_ptr<Symbol> condition;   // for conditional
        std::shared_ptr<Symbol> then_expr;   // for ternary
        std::shared_ptr<Symbol> else_expr;   // for ternary

        // List structures
        std::vector<std::shared_ptr<Symbol>> expressions;
        std::vector<std::shared_ptr<Symbol>> arguments;
        std::vector<std::shared_ptr<Symbol>> steps;  // for path expressions
        std::vector<std::shared_ptr<Symbol>> terms;
        std::vector<std::shared_ptr<Symbol>> predicate;

        // Lambda function fields
        std::shared_ptr<Symbol> body;  // function body
        std::string signature;         // function signature
        std::string variable;          // variable name for binding

        // Additional attributes
        std::shared_ptr<Symbol> ancestor;
        std::shared_ptr<Symbol> seekingParent;
        std::vector<std::shared_ptr<Symbol>>
            seekingParentList;  // For complex parent tracking
        std::any slot;
        bool descending = false;
        std::string name;              // Function name for function calls
        Parser* parser = nullptr;      // Parser reference for proper parsing
        bool _jsonata_lambda = false;  // Lambda function flag like Java
        std::shared_ptr<Symbol> _transform_expr =
            nullptr;  // Transform expression for transform functions

        // Function-related
        std::shared_ptr<Symbol> procedure;

        // Array/object handling
        bool keepArray = false;
        bool consarray = false;
        bool keepSingletonArray = false;
        int64_t level = 0;
        std::any focus;
        std::any tuple;
        bool thunk = false;
        std::any input;
        std::any environment;

        // Complex structures
        std::shared_ptr<Symbol> group;
        std::any index;
        std::string label;
        std::shared_ptr<Symbol> nextFunction;
        std::vector<std::shared_ptr<Symbol>> stages;

        // Transform operations
        std::shared_ptr<Symbol> pattern;
        std::shared_ptr<Symbol> update;
        std::shared_ptr<Symbol> delete_;

        // Object pairs (key-value)
        std::vector<std::pair<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>
            lhsObject;
        std::vector<std::pair<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>
            rhsObject;
        std::vector<std::shared_ptr<Symbol>> rhsTerms;

        // Additional fields
        std::any
            expr;  // Used for filter expressions and other nested expressions

        // Error handling
        std::shared_ptr<JException> error;

        // Partial application support (matching Java implementation)
        std::any nativeFunction;            // For partialApplyNativeFunction
        std::vector<std::any> partialArgs;  // For partialApplyNativeFunction

        // Constructors
        Symbol() = default;
        Symbol(const std::string& id);
        Symbol(const std::string& id, int64_t bp);

        // Virtual methods for different symbol types
        virtual std::shared_ptr<Symbol> nud();
        virtual std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left);
        virtual std::shared_ptr<Symbol> create();

        virtual ~Symbol() = default;

        std::string toString() const;
    };

    // Specific symbol types
    class Terminal : public Symbol {
      public:
        Terminal(const std::string& id);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class Infix : public Symbol {
      public:
        Infix(const std::string& id);
        Infix(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class Prefix : public Symbol {
      public:
        Prefix(const std::string& id);
        Prefix(const std::string& id, int64_t bp);  // Add bp constructor
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class InfixAndPrefix : public Infix {
      public:
        std::shared_ptr<Prefix> prefix;

        InfixAndPrefix(const std::string& id);
        InfixAndPrefix(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class BooleanInfix : public Infix {
      public:
        BooleanInfix(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class InfixR : public Symbol {
      public:
        InfixR(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
    };

    class FunctionCall : public Infix {
      public:
        FunctionCall(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> nud() override;  // For block expressions
        std::shared_ptr<Symbol> create() override;
    };

    class ArrayConstructor : public Infix {
      public:
        ArrayConstructor(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class ObjectConstructor : public Infix {
      public:
        ObjectConstructor(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class TernaryConditional : public Infix {
      public:
        TernaryConditional(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class ElvisOperator : public Infix {
      public:
        ElvisOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class DescendantOperator : public Prefix {
      public:
        DescendantOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class FunctionApplication : public Infix {
      public:
        FunctionApplication(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class NullCoalescing : public Infix {
      public:
        NullCoalescing(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class RangeOperator : public Infix {
      public:
        RangeOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class VariableBinding : public InfixR {
      public:
        VariableBinding(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class FocusBinding : public Infix {
      public:
        FocusBinding(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class IndexBinding : public Infix {
      public:
        IndexBinding(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class WildcardOperator : public InfixAndPrefix {
      public:
        WildcardOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;  // For wildcard
        std::shared_ptr<Symbol> led(
            std::shared_ptr<Symbol> left) override;  // For multiplication
        std::shared_ptr<Symbol> create() override;
    };

    class ModulusParentOperator : public InfixAndPrefix {
      public:
        ModulusParentOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;  // For parent navigation
        std::shared_ptr<Symbol> led(
            std::shared_ptr<Symbol> left) override;  // For modulus
        std::shared_ptr<Symbol> create() override;
    };

    class LambdaFunction : public Symbol {
      public:
        LambdaFunction(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class SortOperator : public Infix {
      public:
        SortOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class PredicateFilter : public Infix {
      public:
        PredicateFilter(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> led(std::shared_ptr<Symbol> left) override;
        std::shared_ptr<Symbol> create() override;
    };

    class VariableReference : public Symbol {
      public:
        VariableReference(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class TransformOperator : public Prefix {
      public:
        TransformOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

    class ParentOperator : public Symbol {
      public:
        ParentOperator(const std::string& id, int64_t bp);
        std::shared_ptr<Symbol> nud() override;
        std::shared_ptr<Symbol> create() override;
    };

  private:
    std::string source_;
    bool recover_;
    bool dbg_ = false;

    std::shared_ptr<Symbol> node_;
    std::unique_ptr<Tokenizer> lexer_;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbolTable_;
    std::vector<std::shared_ptr<std::exception>> errors_;

    // Make parser instance accessible to symbols
    static thread_local Parser* currentParser_;

  public:
    // Constructor
    Parser();

    // Static method to get current parser instance
    static Parser* getCurrentParser() { return currentParser_; }

    // Main parsing method
    std::shared_ptr<Symbol> parse(const std::string& source,
                                  bool recover = false);

    // Get parsing errors
    const std::vector<std::shared_ptr<std::exception>>& getErrors() const {
        return errors_;
    }

  private:
    // Core parsing methods
    std::shared_ptr<Symbol> expression(int64_t rbp = 0);

    // Helper methods
    void registerSymbol(std::shared_ptr<Symbol> symbol);
    std::shared_ptr<Symbol> advance(const std::string& expectedId = "",
                                    bool infix = false);
    void initializeSymbols();
    std::shared_ptr<Symbol> handleError(const JException& e);

    // AST processing
    std::shared_ptr<Symbol> processAST(std::shared_ptr<Symbol> expr);
    std::shared_ptr<Symbol> tailCallOptimize(std::shared_ptr<Symbol> expr);
    std::shared_ptr<Symbol> seekParent(std::shared_ptr<Symbol> node,
                                       std::shared_ptr<Symbol> slot);
    void pushAncestry(std::shared_ptr<Symbol> result,
                      std::shared_ptr<Symbol> value);
    void resolveAncestry(std::shared_ptr<Symbol> path);

    // Object constructor parsing
    std::shared_ptr<Symbol> objectParser(std::shared_ptr<Symbol> left);

    // Token processing
    std::vector<Tokenizer::Token> remainingTokens();

    // Static variables for ancestry tracking
    int64_t ancestorLabel_ = 0;
    int64_t ancestorIndex_ = 0;
    std::vector<std::shared_ptr<Symbol>> ancestry_;
};

}  // namespace jsonata