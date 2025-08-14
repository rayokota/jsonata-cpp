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
#include "jsonata/Parser.h"

#include <iostream>
#include <sstream>

#include "jsonata/Utils.h"

namespace jsonata {

// Thread-local parser instance for symbol access
thread_local Parser* Parser::currentParser_ = nullptr;

// Symbol implementations
Parser::Symbol::Symbol(const std::string& id) : id(id), value(id), bp(0) {
    // Initialize all the new fields to prevent segfaults
    lbp = 0;
    position = 0;
    keepArray = false;
    consarray = false;
    keepSingletonArray = false;
    level = 0;
    thunk = false;
    descending = false;
    _jsonata_lambda = false;
    parser = nullptr;
}

Parser::Symbol::Symbol(const std::string& id, int64_t bp)
    : id(id), value(id), bp(bp) {
    // Initialize all the new fields to prevent segfaults
    lbp = 0;
    position = 0;
    keepArray = false;
    consarray = false;
    keepSingletonArray = false;
    level = 0;
    thunk = false;
    descending = false;
    _jsonata_lambda = false;
    parser = nullptr;
}

std::shared_ptr<Parser::Symbol> Parser::Symbol::nud() {
    // Default implementation - error for symbols that shouldn't be used as
    // prefix
    throw JException("S0211", position, value);
}

std::shared_ptr<Parser::Symbol> Parser::Symbol::led(
    std::shared_ptr<Symbol> left) {
    // Default implementation - error for symbols that shouldn't be used as
    // infix Java throws generic Error("led not implemented"), but we'll use a
    // parser error
    throw JException("S0201", position, value);
}

std::shared_ptr<Parser::Symbol> Parser::Symbol::create() {
    // Create a copy of this symbol
    auto copy = std::make_shared<Symbol>();
    copy->id = this->id;
    copy->type = this->type;
    copy->value = this->value;
    copy->bp = this->bp;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::Terminal::create() {
    // Create a copy of this terminal
    auto copy = std::make_shared<Terminal>(this->id);
    copy->type = this->type;
    copy->value = this->value;
    copy->bp = this->bp;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::string Parser::Symbol::toString() const {
    std::ostringstream oss;
    oss << "Symbol{id='" << id << "', type='" << type << "', value=";
    try {
        if (value.has_value()) {
            if (value.type() == typeid(std::string)) {
                oss << "\"" << std::any_cast<std::string>(value) << "\"";
            } else if (value.type() == typeid(double)) {
                oss << std::any_cast<double>(value);
            } else if (value.type() == typeid(int64_t)) {
                oss << std::any_cast<int64_t>(value);
            } else if (value.type() == typeid(uint64_t)) {
                oss << std::any_cast<uint64_t>(value);
            } else {
                oss << "[object]";
            }
        } else {
            oss << "null";
        }
    } catch (const std::bad_any_cast&) {
        oss << "[unknown]";
    }
    oss << ", position=" << position << "}";
    return oss.str();
}

// Terminal implementation
Parser::Terminal::Terminal(const std::string& id) : Symbol(id, 0) {}

std::shared_ptr<Parser::Symbol> Parser::Terminal::nud() {
    // For terminals, just return this symbol as-is
    return shared_from_this();
}

// Infix implementation
Parser::Infix::Infix(const std::string& id) : Symbol(id) {
    // Get binding power from tokenizer if available
    auto it = Tokenizer::operators.find(id);
    if (it != Tokenizer::operators.end()) {
        bp = it->second;
    }
}

Parser::Infix::Infix(const std::string& id, int64_t bp) : Symbol(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::Infix::create() {
    // This should only be called for base Infix symbols
    // Derived classes should override this method to create their own type
    auto copy = std::make_shared<Infix>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::Infix::led(
    std::shared_ptr<Symbol> left) {
    // For basic infix operators: left <op> right
    // Each operator must parse its own RHS (matches Java reference pattern)
    lhs = left;
    type = "binary";
    value = id;  // Store the operator string

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Parse the right-hand side using this operator's binding power
    rhs = parser->expression(bp);

    return shared_from_this();
}

// Prefix implementation
Parser::Prefix::Prefix(const std::string& id) : Symbol(id) {}

Parser::Prefix::Prefix(const std::string& id, int64_t bp) : Symbol(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::Prefix::create() {
    // Create a copy of this prefix symbol, preserving the derived type
    auto copy = std::make_shared<Prefix>(this->id);
    copy->type = this->type;
    copy->value = this->value;
    copy->bp = this->bp;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::Prefix::nud() {
    // For prefix operators: <op> expression
    type = "unary";
    value = id;  // Store the operator string

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    if (id == "-") {
        // Unary minus with high binding power (70) to handle cases like -5 * 2
        expression = parser->expression(70);
    } else if (id == "[") {
        // Array constructor - the expressions will be set by parser
        // Parse array elements: expressions = [expr1, expr2, ...]
        // Note: Array constructor handling is done elsewhere in the parser
    } else {
        // For other prefix operators, parse with high binding power
        expression = parser->expression(70);
    }

    return shared_from_this();
}

// InfixAndPrefix implementation - handles operators that can be both infix and
// prefix (like -)
Parser::InfixAndPrefix::InfixAndPrefix(const std::string& id) : Infix(id) {
    prefix = std::make_shared<Prefix>(id);
}

Parser::InfixAndPrefix::InfixAndPrefix(const std::string& id, int64_t bp)
    : Infix(id, bp) {
    prefix = std::make_shared<Prefix>(id);
}

std::shared_ptr<Parser::Symbol> Parser::InfixAndPrefix::create() {
    // Create a copy of this InfixAndPrefix symbol, ensuring each copy has its
    // own prefix
    auto copy = std::make_shared<InfixAndPrefix>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;

    // IMPORTANT: Create a new Prefix instance for the copy (as per Java clone()
    // method)
    copy->prefix = std::make_shared<Prefix>(this->id);

    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::InfixAndPrefix::nud() {
    // Delegate to the prefix behavior for unary operations
    // This is called when the symbol appears in prefix position (e.g., -5)
    return prefix->nud();
}

// BooleanInfix implementation - handles and/or/in operators that can be both
// infix and prefix (names)
Parser::BooleanInfix::BooleanInfix(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::BooleanInfix::create() {
    auto copy = std::make_shared<BooleanInfix>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::BooleanInfix::nud() {
    // Allow these operators to be used as terminals (field names)
    // This matches the Java implementation: @Override Symbol nud() { return
    // this; }
    return shared_from_this();
}

// InfixR implementation
Parser::InfixR::InfixR(const std::string& id, int64_t bp) : Symbol(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::InfixR::led(
    std::shared_ptr<Symbol> left) {
    // Right-associative infix operator
    lhs = left;
    // Note: Need access to parser's expression method
    // This is a simplified version - right associative would call
    // parser.expression(bp - 1)
    type = "binary";
    return std::static_pointer_cast<Symbol>(shared_from_this());
}

// FunctionCall implementation
Parser::FunctionCall::FunctionCall(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::FunctionCall::create() {
    // Create a copy of this function call symbol, preserving the derived type
    auto copy = std::make_shared<FunctionCall>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::FunctionCall::led(
    std::shared_ptr<Symbol> left) {
    // Enhanced function call processing with lambda support
    // Based on Java Parser.java lines 553-612

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // left is what we are trying to invoke
    procedure = left;
    type = "function";
    arguments.clear();

    // Parse function arguments
    if (parser->node_->id != ")") {
        while (true) {
            if (parser->node_->type == "operator" && parser->node_->id == "?") {
                // Partial function application
                type = "partial";
                arguments.push_back(parser->node_);
                parser->advance("?");
            } else {
                arguments.push_back(parser->expression(0));
            }
            if (parser->node_->id != ",") break;
            parser->advance(",");
        }
    }
    parser->advance(")", true);

    // Check if this is a lambda function definition
    if (left && left->type == "name") {
        std::string leftValue;
        try {
            leftValue = std::any_cast<std::string>(left->value);
        } catch (const std::bad_any_cast&) {
            leftValue = "";
        }

        // Check for 'function' or λ (Unicode lambda)
        if (leftValue == "function" || leftValue == "\u03BB") {
            // All arguments must be VARIABLE tokens
            for (const auto& arg : arguments) {
                if (!arg || arg->type != "variable") {
                    return parser->handleError(
                        JException("S0208", arg ? arg->position : position,
                                   arg ? arg->value : std::any{}));
                }
            }

            type = "lambda";

            // Parse function signature if present: <type>
            if (parser->node_->id == "<") {
                int64_t depth = 1;
                std::string sig = "<";
                while (depth > 0 && parser->node_->id != "{" &&
                       parser->node_->id != "(end)") {
                    auto tok = parser->advance();
                    if (tok->id == ">") {
                        depth--;
                    } else if (tok->id == "<") {
                        depth++;
                    }
                    // Java reference: line 601 - sig += tok.value;
                    // Always use tok.value like Java, not tok.id
                    try {
                        std::string tokValue =
                            std::any_cast<std::string>(tok->value);
                        sig += tokValue;
                    } catch (const std::bad_any_cast&) {
                        // If value is not a string, convert it to string
                        // representation This follows Java behavior more
                        // closely than using tok->id
                        sig += tok->id;
                    }
                }
                parser->advance(">");

                // Create signature object
                try {
                    signature = sig;
                    // Note: In Java this creates a Signature object, in C++ we
                    // store the string The actual Signature object can be
                    // created later when needed
                } catch (const std::exception& e) {
                    // If signature parsing fails, continue without signature
                }
            }

            // Parse function body: { expression }
            parser->advance("{");
            body = parser->expression(0);
            parser->advance("}");
        }
    }

    return shared_from_this();
}

std::shared_ptr<Parser::Symbol> Parser::FunctionCall::nud() {
    // Block expression: (expr; expr; expr)
    // This is the nud() behavior from the Java anonymous class
    type = "block";

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Parse expressions separated by semicolons
    while (parser->node_->id != ")") {
        expressions.push_back(parser->expression(0));
        if (parser->node_->id == ";") {
            parser->advance(";");
        } else {
            break;
        }
    }
    parser->advance(")",
                    true);  // Match Java: advance(")", true) for prefix context

    return shared_from_this();
}

// ArrayConstructor implementation
Parser::ArrayConstructor::ArrayConstructor(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::ArrayConstructor::create() {
    auto copy = std::make_shared<ArrayConstructor>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;  // Copy parser reference
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::ArrayConstructor::nud() {
    // Array constructor: [expr1, expr2, ...]
    type = "unary";  // JSONata uses "unary" type for array literals

    // Access the current parser instance
    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Parse array elements: [expr1, expr2, ...] until "]"
    if (parser->node_->id != "]") {
        do {
            auto item = parser->expression(0);
            // Java reference (lines 646-653): Check for range operator
            if (parser->node_->id == "..") {
                // range operator
                auto range = std::make_shared<Symbol>();
                range->type = "binary";
                range->value = std::string("..");
                range->position = parser->node_->position;
                range->lhs = item;
                parser->advance("..");
                range->rhs = parser->expression(0);
                item = range;
            }
            expressions.push_back(item);
            if (parser->node_->id == ",") {
                parser->advance(",");
            } else {
                break;
            }
        } while (true);
    }
    parser->advance("]");

    return shared_from_this();
}

std::shared_ptr<Parser::Symbol> Parser::ArrayConstructor::led(
    std::shared_ptr<Symbol> left) {
    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    if (parser->node_->id == "]") {
        // Empty predicate means maintain singleton arrays in the output
        // Follow Java implementation: set keepArray flag and return left
        // unchanged
        auto step = left;
        while (step && step->type == "binary" && step->value.has_value()) {
            try {
                std::string stepValue = std::any_cast<std::string>(step->value);
                if (stepValue == "[") {
                    step = step->lhs;
                } else {
                    break;
                }
            } catch (const std::bad_any_cast&) {
                break;
            }
        }
        if (step) {
            step->keepArray = true;
        }
        parser->advance("]");
        return left;
    } else {
        // Array filter/predicate: expr[predicate] or array index: arr[0]
        // Follow Java pattern: keep as "binary" type with "[" value for
        // processAST handling
        lhs = left;
        type = "binary";           // Keep as binary like Java, not "filter"
        value = std::string("[");  // Set value to "[" like Java

        // Parse the expression inside brackets
        rhs = parser->expression(0);
        parser->advance("]");

        return shared_from_this();
    }
}

// ObjectConstructor implementation
Parser::ObjectConstructor::ObjectConstructor(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::ObjectConstructor::create() {
    auto copy = std::make_shared<ObjectConstructor>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::ObjectConstructor::nud() {
    // Object constructor: {key: value, ...}
    // Use the dedicated objectParser method for proper parsing
    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    return parser->objectParser(nullptr);  // null = prefix form
}

std::shared_ptr<Parser::Symbol> Parser::ObjectConstructor::led(
    std::shared_ptr<Symbol> left) {
    // Object transform: expr{key: value}
    // Use the dedicated objectParser method for proper parsing
    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    return parser->objectParser(left);  // left = infix form
}

// TernaryConditional implementation
Parser::TernaryConditional::TernaryConditional(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::TernaryConditional::create() {
    auto copy = std::make_shared<TernaryConditional>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::TernaryConditional::led(
    std::shared_ptr<Symbol> left) {
    // Ternary conditional: condition ? then : else
    // Based on Java Parser.java lines 794-808 - else clause is optional
    type = "condition";
    condition = left;

    // Parse the 'then' expression
    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    then_expr = parser->expression(0);

    // Check if there's an else clause (Java: if (node.id.equals(":")))
    if (parser->node_->id == ":") {
        // else condition
        parser->advance(":");
        else_expr = parser->expression(0);
    }
    // Note: else_expr remains null if no else clause - this matches Java
    // behavior

    return shared_from_this();
}

// ElvisOperator implementation
Parser::ElvisOperator::ElvisOperator(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::ElvisOperator::create() {
    auto copy = std::make_shared<ElvisOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::ElvisOperator::led(
    std::shared_ptr<Symbol> left) {
    // Elvis operator: value ?: default
    type = "condition";
    condition = left;
    then_expr = left;  // Same as condition for elvis

    // Parse the default value
    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    else_expr = parser->expression(0);

    return shared_from_this();
}

// DescendantOperator implementation
Parser::DescendantOperator::DescendantOperator(const std::string& id, int64_t bp)
    : Prefix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::DescendantOperator::create() {
    auto copy = std::make_shared<DescendantOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::DescendantOperator::nud() {
    // Descendant operator: ** (deep search)
    type = "descendant";
    return shared_from_this();
}

// FunctionApplication implementation
Parser::FunctionApplication::FunctionApplication(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::FunctionApplication::create() {
    auto copy = std::make_shared<FunctionApplication>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::FunctionApplication::led(
    std::shared_ptr<Symbol> left) {
    // Function application: expr ~> func
    type = "apply";
    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    rhs = parser->expression(bp);
    return shared_from_this();
}

// NullCoalescing implementation
Parser::NullCoalescing::NullCoalescing(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::NullCoalescing::create() {
    auto copy = std::make_shared<NullCoalescing>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::NullCoalescing::led(
    std::shared_ptr<Symbol> left) {
    // Java implementation lines 499-513: creates complex condition with exists
    // function
    type = "condition";

    // Create the condition: exists(left)
    auto c = std::make_shared<Symbol>();
    condition = c;
    c->type = "function";
    c->value = std::string("(");

    // Create the procedure: variable "exists"
    auto p = std::make_shared<Symbol>();
    c->procedure = p;
    p->type = "variable";
    p->value = std::string("exists");

    // Set arguments to [left]
    c->arguments = {left};

    // then = left, else = expression(0)
    then_expr = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    else_expr =
        parser->expression(0);  // Java uses expression(0), not expression(bp)
    return shared_from_this();
}

// RangeOperator implementation
Parser::RangeOperator::RangeOperator(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::RangeOperator::create() {
    auto copy = std::make_shared<RangeOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::RangeOperator::led(
    std::shared_ptr<Symbol> left) {
    // Range operator: start..end
    type = "range";
    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    rhs = parser->expression(bp);
    return shared_from_this();
}

// VariableBinding implementation
Parser::VariableBinding::VariableBinding(const std::string& id, int64_t bp)
    : InfixR(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::VariableBinding::create() {
    auto copy = std::make_shared<VariableBinding>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::VariableBinding::led(
    std::shared_ptr<Symbol> left) {
    // Variable binding: $var := value
    // Java reference lines 743-758

    if (left->type != "variable") {
        throw JException("S0212", left->position, left->value);
    }

    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Java: this.rhs = expression(Tokenizer.operators.get(":=") - 1); //
    // subtract 1 from bindingPower for right associative operators
    rhs = parser->expression(
        bp - 1);  // Right associative: subtract 1 from binding power

    // Java: this.type = "binary"; - Let processAST convert to "bind"
    type = "binary";

    return shared_from_this();
}

// LambdaFunction implementation
Parser::LambdaFunction::LambdaFunction(const std::string& id, int64_t bp)
    : Symbol(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::LambdaFunction::create() {
    auto copy = std::make_shared<LambdaFunction>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::LambdaFunction::nud() {
    // Lambda function: function($x, $y) { $x + $y }
    type = "lambda";

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Parse parameters: ($x, $y) or ()
    if (parser->node_->id == "(") {
        parser->advance("(");

        // Parse parameter list
        if (parser->node_->id != ")") {
            do {
                if (parser->node_->type == "variable") {
                    arguments.push_back(parser->node_);
                    parser->advance("", false);
                } else {
                    throw JException("S0202", parser->node_->position,
                                     parser->node_->value);
                }

                if (parser->node_->id == ",") {
                    parser->advance(",");
                } else {
                    break;
                }
            } while (true);
        }
        parser->advance(")");
    }

    // Parse function body: { expression }
    if (parser->node_->id == "{") {
        parser->advance("{");

        // Parse the body expression
        body = parser->expression(0);

        parser->advance("}");
    } else {
        throw JException("S0202", parser->node_->position,
                         "Expected '{' after function parameters");
    }

    return shared_from_this();
}

// SortOperator implementation
Parser::SortOperator::SortOperator(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::SortOperator::create() {
    auto copy = std::make_shared<SortOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::SortOperator::led(
    std::shared_ptr<Symbol> left) {
    // Sort operator: array^(expression)
    type = "sort";
    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    parser->advance("(");

    // Parse sort terms
    do {
        auto term = std::make_shared<Symbol>("term");

        // Check for sort direction
        if (parser->node_->id == "<") {
            parser->advance("<");
            term->descending = false;  // Explicitly set ascending
        } else if (parser->node_->id == ">") {
            parser->advance(">");
            term->descending = true;  // Set descending flag
        } else {
            term->descending = false;  // Default to ascending
        }

        term->expression = parser->expression(0);
        terms.push_back(term);

        if (parser->node_->id == ",") {
            parser->advance(",");
        } else {
            break;
        }
    } while (true);

    parser->advance(")");
    return shared_from_this();
}

// PredicateFilter implementation (for [expression] syntax)
Parser::PredicateFilter::PredicateFilter(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::PredicateFilter::create() {
    auto copy = std::make_shared<PredicateFilter>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::PredicateFilter::led(
    std::shared_ptr<Symbol> left) {
    // Predicate filter: array[condition]
    type = "filter";
    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Parse the filter expression
    rhs = parser->expression(0);
    parser->advance("]");

    return shared_from_this();
}

// VariableReference implementation (for $ context variable)
Parser::VariableReference::VariableReference(const std::string& id, int64_t bp)
    : Symbol(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::VariableReference::create() {
    auto copy = std::make_shared<VariableReference>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::VariableReference::nud() {
    // Java reference: preserve tokenizer value (empty string for standalone $)
    // Lines 319-325: node.value = value; (value from tokenizer)
    type = "variable";
    // Don't override value - keep what tokenizer provided (empty string for
    // "$")
    return shared_from_this();
}

// TransformOperator implementation (for | update/delete patterns)
Parser::TransformOperator::TransformOperator(const std::string& id, int64_t bp)
    : Prefix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::TransformOperator::create() {
    auto copy = std::make_shared<TransformOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

// ParentOperator implementation (for % ancestor navigation)
Parser::ParentOperator::ParentOperator(const std::string& id, int64_t bp)
    : Symbol(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::ParentOperator::create() {
    auto copy = std::make_shared<ParentOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::ParentOperator::nud() {
    // Parent operator: % (refers to parent context)
    type = "parent";
    return shared_from_this();
}

// Missing implementations for new classes

// FocusBinding implementation
Parser::FocusBinding::FocusBinding(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::FocusBinding::create() {
    auto copy = std::make_shared<FocusBinding>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::FocusBinding::led(
    std::shared_ptr<Symbol> left) {
    // Focus binding: expr @ variable
    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    rhs = parser->expression(bp);
    if (!rhs || rhs->type != "variable") {
        throw JException("S0214", rhs ? rhs->position : position, "@");
    }

    type = "binary";
    return shared_from_this();
}

// IndexBinding implementation
Parser::IndexBinding::IndexBinding(const std::string& id, int64_t bp)
    : Infix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::IndexBinding::create() {
    auto copy = std::make_shared<IndexBinding>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::IndexBinding::led(
    std::shared_ptr<Symbol> left) {
    // Index binding: expr # variable
    lhs = left;

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    rhs = parser->expression(bp);
    if (!rhs || rhs->type != "variable") {
        throw JException("S0214", rhs ? rhs->position : position, "#");
    }

    type = "binary";
    return shared_from_this();
}

// WildcardOperator implementation
Parser::WildcardOperator::WildcardOperator(const std::string& id, int64_t bp)
    : InfixAndPrefix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::WildcardOperator::create() {
    auto copy = std::make_shared<WildcardOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::WildcardOperator::nud() {
    // Wildcard: * (field wildcard - single level)
    type = "wildcard";
    return shared_from_this();
}

std::shared_ptr<Parser::Symbol> Parser::WildcardOperator::led(
    std::shared_ptr<Symbol> left) {
    // Multiplication: left * right
    lhs = left;
    type = "binary";

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    rhs = parser->expression(bp);
    return shared_from_this();
}

// ModulusParentOperator implementation
Parser::ModulusParentOperator::ModulusParentOperator(const std::string& id,
                                                     int64_t bp)
    : InfixAndPrefix(id, bp) {}

std::shared_ptr<Parser::Symbol> Parser::ModulusParentOperator::create() {
    auto copy = std::make_shared<ModulusParentOperator>(this->id, this->bp);
    copy->type = this->type;
    copy->value = this->value;
    copy->lbp = this->lbp;
    copy->position = this->position;
    copy->parser = this->parser;
    return copy;
}

std::shared_ptr<Parser::Symbol> Parser::ModulusParentOperator::nud() {
    // Parent operator: % (refers to parent context)
    type = "parent";
    return shared_from_this();
}

std::shared_ptr<Parser::Symbol> Parser::ModulusParentOperator::led(
    std::shared_ptr<Symbol> left) {
    // Modulus: left % right
    lhs = left;
    type = "binary";

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    rhs = parser->expression(bp);
    return shared_from_this();
}

// TransformOperator nud implementation (to fix vtable)
std::shared_ptr<Parser::Symbol> Parser::TransformOperator::nud() {
    // Transform operator: | pattern | update | delete

    type = "transform";

    Parser* parser = Parser::getCurrentParser();
    if (!parser) {
        throw JException("T0410", position, "No parser context available");
    }

    // Parse the transform pattern
    pattern = parser->expression(0);
    parser->advance("|");
    update = parser->expression(0);
    if (parser->node_->id == ",") {
        parser->advance(",");
        delete_ = parser->expression(0);
    }
    parser->advance("|");

    return shared_from_this();
}

// Parser implementation
Parser::Parser() : recover_(false) {
    currentParser_ = this;  // Set current parser for symbol access
    initializeSymbols();
}

void Parser::initializeSymbols() {
    // Register terminal symbols
    registerSymbol(std::make_shared<Terminal>("(end)"));
    registerSymbol(std::make_shared<Terminal>("(name)"));
    registerSymbol(std::make_shared<Terminal>("(literal)"));
    registerSymbol(std::make_shared<Terminal>("(regex)"));

    // Register basic symbols - exactly matching Java Parser.java lines 443-449
    registerSymbol(
        std::make_shared<Symbol>(":"));  // Java: register(new Symbol(":"));
    registerSymbol(
        std::make_shared<Symbol>(";"));  // Java: register(new Symbol(";"));
    registerSymbol(
        std::make_shared<Symbol>(","));  // Java: register(new Symbol(","));
    registerSymbol(
        std::make_shared<Symbol>(")"));  // Java: register(new Symbol(")"));
    registerSymbol(
        std::make_shared<Symbol>("]"));  // Java: register(new Symbol("]"));
    registerSymbol(
        std::make_shared<Symbol>("}"));  // Java: register(new Symbol("}"));
    registerSymbol(std::make_shared<Symbol>(
        ".."));  // Java: register(new Symbol("..")); // range operator

    // Register special literals
    registerSymbol(std::make_shared<Symbol>("(literal)"));
    registerSymbol(std::make_shared<Symbol>("(regex)"));
    registerSymbol(std::make_shared<Symbol>("(end)"));

    // Register operators with proper binding powers from tokenizer
    registerSymbol(std::make_shared<Infix>(
        ".", Tokenizer::operators.at(".")));  // map operator
    // Binary/infix operators with various precedences
    registerSymbol(std::make_shared<Infix>("+", Tokenizer::operators.at("+")));
    registerSymbol(
        std::make_shared<InfixAndPrefix>("-", Tokenizer::operators.at("-")));
    registerSymbol(
        std::make_shared<WildcardOperator>("*", Tokenizer::operators.at("*")));
    registerSymbol(std::make_shared<Infix>("/", Tokenizer::operators.at("/")));
    registerSymbol(std::make_shared<ModulusParentOperator>(
        "%", Tokenizer::operators.at("%")));
    registerSymbol(std::make_shared<Infix>("=", Tokenizer::operators.at("=")));
    registerSymbol(
        std::make_shared<Infix>("!=", Tokenizer::operators.at("!=")));
    registerSymbol(std::make_shared<Infix>("<", Tokenizer::operators.at("<")));
    registerSymbol(
        std::make_shared<Infix>("<=", Tokenizer::operators.at("<=")));
    registerSymbol(std::make_shared<Infix>(">", Tokenizer::operators.at(">")));
    registerSymbol(
        std::make_shared<Infix>(">=", Tokenizer::operators.at(">=")));
    registerSymbol(std::make_shared<Infix>("&", Tokenizer::operators.at("&")));
    registerSymbol(
        std::make_shared<BooleanInfix>("and", Tokenizer::operators.at("and")));
    registerSymbol(
        std::make_shared<BooleanInfix>("or", Tokenizer::operators.at("or")));
    registerSymbol(
        std::make_shared<BooleanInfix>("in", Tokenizer::operators.at("in")));
    // NOTE: Specialized operators like "..", ":=", "^", etc. are registered
    // later Do NOT register generic versions of these here!

    // Function call - "(" as infix operator (also handles block expressions via
    // nud())
    registerSymbol(
        std::make_shared<FunctionCall>("(", Tokenizer::operators.at("(")));

    // Array constructor - "[" as both prefix and infix
    registerSymbol(
        std::make_shared<ArrayConstructor>("[", Tokenizer::operators.at("[")));

    // Object constructor - "{" as both prefix and infix
    registerSymbol(
        std::make_shared<ObjectConstructor>("{", Tokenizer::operators.at("{")));

    // Conditional operators
    registerSymbol(std::make_shared<TernaryConditional>(
        "?", Tokenizer::operators.at("?")));
    registerSymbol(
        std::make_shared<ElvisOperator>("?:", Tokenizer::operators.at("?:")));

    // Advanced operators
    registerSymbol(std::make_shared<DescendantOperator>(
        "**", Tokenizer::operators.at("**")));
    registerSymbol(std::make_shared<FunctionApplication>(
        "~>", Tokenizer::operators.at("~>")));
    registerSymbol(
        std::make_shared<NullCoalescing>("??", Tokenizer::operators.at("??")));

    // Error handling symbol - Java line 516
    registerSymbol(std::make_shared<InfixR>("(error)", 10));

    // Transform operator
    registerSymbol(std::make_shared<TransformOperator>(
        "|",
        0));  // Transform operator - binding power 0 to prevent led() calls

    // Binding operators (range operator ".." already registered as Symbol
    // above)
    registerSymbol(
        std::make_shared<VariableBinding>(":=", Tokenizer::operators.at(":=")));

    // Lambda functions and sorting
    // NOTE: "function" should NOT be registered as a separate symbol
    // Lambda parsing is handled in FunctionCall::led() when "function" is the
    // left operand
    registerSymbol(
        std::make_shared<SortOperator>("^", Tokenizer::operators.at("^")));

    // NOTE: Variables like "$" are NOT registered as symbols in Java
    // They are handled by the tokenizer when it sees the "$" prefix
    // The C++ tokenizer should handle this the same way

    // Focus and index binding operators (from Java @ and # operators)
    registerSymbol(
        std::make_shared<FocusBinding>("@", Tokenizer::operators.at("@")));
    registerSymbol(
        std::make_shared<IndexBinding>("#", Tokenizer::operators.at("#")));

    // NOTE: Terminal symbols ], }, ) already registered above as Symbol objects
    // This matches the Java implementation exactly
}

void Parser::registerSymbol(std::shared_ptr<Symbol> symbol) {
    if (!symbol) return;

    auto it = symbolTable_.find(symbol->id);
    if (it != symbolTable_.end()) {
        if (it->second->lbp == 0) {
            // Existing symbol with no binding power, update it
            it->second->lbp = symbol->bp;
            it->second->bp = symbol->bp;
        }
    } else {
        // Store the symbol directly, not a copy
        // This preserves the derived class type (Infix, Prefix, etc.)
        symbol->lbp = symbol->bp;
        symbol->parser = this;  // Set parser reference
        symbolTable_[symbol->id] = symbol;
    }
}

std::shared_ptr<Parser::Symbol> Parser::parse(const std::string& source,
                                              bool recover) {
    source_ = source;
    recover_ = recover;
    lexer_ = std::make_unique<Tokenizer>(source);

    // Start parsing (Java: advance() defaults to prefix=false)
    advance("");
    auto expr = expression(0);
    if (node_->id != "(end)") {
        throw JException("S0201", node_->position, node_->value);
    }

    expr = processAST(expr);

    // Check for parent/ancestor issues like in Parser.java
    if (expr && (expr->type == "parent" || !expr->seekingParentList.empty())) {
        // error - trying to derive ancestor at top level
        throw JException("S0217", expr->position, expr->type);
    }

    if (!errors_.empty()) {
        // Store errors in the expression
        // expr->errors = errors_; // Would need to implement errors storage
    }

    return expr;
}

std::shared_ptr<Parser::Symbol> Parser::advance(const std::string& expectedId,
                                                bool infix) {
    if (!expectedId.empty() && node_->id != expectedId) {
        std::string code = node_->id == "(end)" ? "S0203" : "S0202";
        JException err(code, node_->position, expectedId, node_->value);
        return handleError(err);
    }

    auto nextToken = lexer_->next(infix);
    if (!nextToken) {
        node_ = symbolTable_["(end)"];
        node_->position = static_cast<int64_t>(source_.length());
        return node_;
    }

    std::shared_ptr<Symbol> symbol;

    // Determine symbol type based on token type
    switch (nextToken->type[0]) {  // Using first character for switch
        case 'n':                  // "name" or "number"
            if (nextToken->type == "name") {
                symbol = symbolTable_["(name)"];
            } else if (nextToken->type == "number") {
                symbol = symbolTable_["(literal)"];
            }
            break;
        case 'v':  // "variable" or "value"
            if (nextToken->type == "variable") {
                // Variables should use the same symbol as names (like Java
                // implementation) Both "name" and "variable" tokens use the
                // "(name)" symbol
                symbol = symbolTable_["(name)"];
            } else if (nextToken->type == "value") {
                symbol = symbolTable_["(literal)"];
            }
            break;
        case 'o':  // "operator"
            if (nextToken->type == "operator") {
                try {
                    std::string opValue =
                        std::any_cast<std::string>(nextToken->value);
                    auto it = symbolTable_.find(opValue);
                    if (it != symbolTable_.end()) {
                        symbol = it->second;
                    } else {
                        throw JException("S0204", nextToken->position,
                                         nextToken->value);
                    }
                } catch (const std::bad_any_cast&) {
                    throw JException("S0204", nextToken->position,
                                     nextToken->value);
                }
            }
            break;
        case 's':  // "string"
            symbol = symbolTable_["(literal)"];
            break;
        case 'r':  // "regex"
            symbol = symbolTable_["(regex)"];
            break;
        default:
            throw JException("S0205", nextToken->position, nextToken->value);
    }

    if (!symbol) {
        throw JException("S0205", nextToken->position, nextToken->value);
    }

    node_ = symbol->create();
    node_->value = nextToken->value;
    node_->type = nextToken->type;
    node_->position = nextToken->position;

    return node_;
}

std::shared_ptr<Parser::Symbol> Parser::expression(int64_t rbp) {
    // Java reference: expression(int rbp) method (lines 329-341)
    // Let each operator's led() method handle its own right-hand side parsing
    auto t = node_;
    // Java: advance(null, true) here to set prefix=true for nud contexts
    advance("", true);
    auto left = t->nud();

    while (rbp < node_->lbp) {
        t = node_;
        advance("", false);
        left = t->led(left);
    }

    return left;
}

std::shared_ptr<Parser::Symbol> Parser::handleError(const JException& err) {
    if (recover_) {
        auto errorSymbol = std::make_shared<Symbol>();
        errorSymbol->id = "(error)";
        errorSymbol->type = "(error)";
        errorSymbol->error = std::make_shared<JException>(err);
        errors_.push_back(std::make_shared<JException>(err));
        return errorSymbol;
    } else {
        throw err;
    }
}

std::vector<Tokenizer::Token> Parser::remainingTokens() {
    std::vector<Tokenizer::Token> remaining;

    if (node_->id != "(end)") {
        Tokenizer::Token t;
        t.type = node_->type;
        t.value = node_->value;
        t.position = node_->position;
        remaining.push_back(t);
    }

    auto nextToken = lexer_->next(false);
    while (nextToken) {
        remaining.push_back(*nextToken);
        nextToken = lexer_->next(false);
    }

    return remaining;
}

// Helper function implementations first

std::shared_ptr<Parser::Symbol> Parser::tailCallOptimize(
    std::shared_ptr<Symbol> expr) {
    if (!expr) return expr;

    if (expr->type == "function" && expr->predicate.empty()) {
        auto thunk = std::make_shared<Symbol>();
        thunk->type = "lambda";
        thunk->thunk = true;
        thunk->arguments.clear();
        thunk->position = expr->position;
        thunk->body = expr;
        return thunk;
    } else if (expr->type == "condition") {
        // Analyze both branches
        expr->then_expr = tailCallOptimize(expr->then_expr);
        if (expr->else_expr) {
            expr->else_expr = tailCallOptimize(expr->else_expr);
        }
        return expr;
    } else if (expr->type == "block") {
        // Only the last expression in the block
        auto length = expr->expressions.size();
        if (length > 0) {
            expr->expressions[length - 1] =
                tailCallOptimize(expr->expressions[length - 1]);
        }
        return expr;
    } else {
        return expr;
    }
}

std::shared_ptr<Parser::Symbol> Parser::seekParent(
    std::shared_ptr<Symbol> node, std::shared_ptr<Symbol> slot) {
    if (!node || !slot) return slot;

    if (node->type == "name" || node->type == "wildcard") {
        slot->level--;
        if (slot->level == 0) {
            if (!node->ancestor) {
                node->ancestor = slot;
            } else {
                // Java: reuse the existing label
                // ancestry.get((int)slot.index).slot.label =
                // node.ancestor.label;
                try {
                    int64_t slotIndex = Utils::toLong(slot->index);
                    if (slotIndex >= 0 && slotIndex < ancestry_.size()) {
                        auto existingLabel = node->ancestor->label;
                        auto ancestrySlot =
                            std::any_cast<std::shared_ptr<Symbol>>(
                                ancestry_[slotIndex]->slot);
                        ancestrySlot->label = existingLabel;
                    }
                } catch (const std::bad_any_cast&) {
                    // Handle conversion error
                }
                node->ancestor = slot;
            }
            node->tuple = true;
        }
    } else if (node->type == "parent") {
        slot->level++;
    } else if (node->type == "block") {
        // Look in last expression in the block
        if (!node->expressions.empty() && node->expressions.back()) {
            node->tuple = true;
            slot = seekParent(node->expressions.back(), slot);
        }
    } else if (node->type == "path") {
        // Last step in path
        node->tuple = true;
        int64_t index = node->steps.size() - 1;
        if (index >= 0 && node->steps[index]) {
            slot = seekParent(node->steps[index], slot);
            index--;
            while (slot && slot->level > 0 && index >= 0 &&
                   node->steps[index]) {
                // Check previous steps
                slot = seekParent(node->steps[index], slot);
                index--;
            }
        }
    } else {
        // Error - can't derive ancestor
        throw JException("S0217", node->position, node->type);
    }
    return slot;
}

void Parser::pushAncestry(std::shared_ptr<Symbol> result,
                          std::shared_ptr<Symbol> value) {
    if (!value || !result) return;

    if (!value->seekingParentList.empty() || value->type == "parent") {
        auto slots = value->seekingParentList;
        if (value->type == "parent") {
            // Add parent slot if available
            if (value->slot.has_value()) {
                try {
                    auto parentSlot =
                        std::any_cast<std::shared_ptr<Symbol>>(value->slot);
                    if (parentSlot) {
                        slots.push_back(parentSlot);
                    }
                } catch (const std::bad_any_cast&) {
                    // Handle conversion error
                }
            }
        }

        if (result->seekingParentList.empty()) {
            result->seekingParentList = slots;
        } else {
            result->seekingParentList.insert(result->seekingParentList.end(),
                                             slots.begin(), slots.end());
        }
    }
}

void Parser::resolveAncestry(std::shared_ptr<Symbol> path) {
    if (!path || path->steps.empty()) return;

    int64_t index = path->steps.size() - 1;
    auto lastStep = path->steps[index];
    if (!lastStep) return;

    auto slots = lastStep->seekingParentList;

    if (lastStep->type == "parent") {
        // Add parent slot if available
        if (lastStep->slot.has_value()) {
            try {
                auto parentSlot =
                    std::any_cast<std::shared_ptr<Symbol>>(lastStep->slot);
                if (parentSlot) {
                    slots.push_back(parentSlot);
                }
            } catch (const std::bad_any_cast&) {
                // Handle conversion error
            }
        }
    }

    for (auto slot : slots) {
        if (!slot) continue;
        index = path->steps.size() - 2;
        while (slot && slot->level > 0) {
            if (index < 0) {
                if (path->seekingParentList.empty()) {
                    path->seekingParentList = {slot};
                } else {
                    path->seekingParentList.push_back(slot);
                }
                break;
            }

            // Try previous step
            if (index >= 0 && index < path->steps.size() &&
                path->steps[index]) {
                auto step = path->steps[index--];

                // Multiple contiguous steps that bind the focus should be
                // skipped
                while (index >= 0 && step && step->focus.has_value() &&
                       index < path->steps.size() && path->steps[index] &&
                       path->steps[index]->focus.has_value()) {
                    step = path->steps[index--];
                }
                if (step) {
                    slot = seekParent(step, slot);
                }
            } else {
                break;
            }
        }
    }
}

std::shared_ptr<Parser::Symbol> Parser::processAST(
    std::shared_ptr<Symbol> expr) {
    if (!expr) return nullptr;

    if (dbg_) {
        std::string valueStr = "null";
        if (expr->value.has_value()) {
            try {
                valueStr = std::any_cast<std::string>(expr->value);
            } catch (const std::bad_any_cast&) {
                valueStr = "[object]";
            }
        }
        std::cout << " > processAST type=" << expr->type << " value='"
                  << valueStr << "'" << std::endl;
    }

    std::string exprType = expr->type.empty() ? "(null)" : expr->type;
    std::shared_ptr<Symbol> result;

    if (exprType == "binary") {
        std::string opValue;
        try {
            opValue = std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast&) {
            opValue = "";
        }

        if (opValue == ".") {
            auto lstep = processAST(expr->lhs);

            if (lstep && lstep->type == "path") {
                result = lstep;
            } else {
                result = std::make_shared<Symbol>();
                result->type = "path";
                if (lstep) {
                    result->steps = {lstep};
                }
            }

            if (lstep && lstep->type == "parent") {
                // Java: result.seekingParent = new
                // ArrayList<>(Arrays.asList(lstep.slot));
                if (lstep->slot.has_value()) {
                    try {
                        auto parentSlot =
                            std::any_cast<std::shared_ptr<Symbol>>(lstep->slot);
                        if (parentSlot) {
                            result->seekingParentList = {parentSlot};
                        }
                    } catch (const std::bad_any_cast&) {
                        // Handle conversion error
                    }
                }
            }

            auto rest = processAST(expr->rhs);
            if (rest && rest->type == "function" && rest->procedure &&
                rest->procedure->type == "path" &&
                !rest->procedure->steps.empty() &&
                rest->procedure->steps[0]->type == "name" &&
                !result->steps.empty() &&
                result->steps.back()->type == "function") {
                // Next function in chain of functions - will override a
                // thenable
                result->steps.back()->nextFunction = rest->procedure->steps[0];
            }

            if (rest && rest->type == "path") {
                result->steps.insert(result->steps.end(), rest->steps.begin(),
                                     rest->steps.end());
            } else {
                if (rest && !rest->predicate.empty()) {
                    rest->stages = rest->predicate;
                    rest->predicate.clear();
                }
                if (rest) {
                    result->steps.push_back(rest);
                }
            }

            // Java: propagate any seekingParent from rest to result
            // Commenting out temporarily to check if this causes S0217
            // regression if (rest) {
            //     pushAncestry(result, rest);
            // }

            // Convert string literals to names in path steps
            for (auto& step : result->steps) {
                if (step->type == "number" || step->type == "value") {
                    throw JException("S0213", step->position, step->value);
                }
                if (step->type == "string") {
                    step->type = "name";
                }
            }

            // Check for keepArray flag
            bool hasKeepArray = false;
            for (const auto& step : result->steps) {
                if (step->keepArray) {
                    hasKeepArray = true;
                    break;
                }
            }
            if (hasKeepArray) {
                result->keepSingletonArray = true;
            }

            // Special handling for first and last steps
            if (!result->steps.empty()) {
                auto firstStep = result->steps[0];
                if (firstStep->type == "unary") {
                    try {
                        std::string firstValue =
                            std::any_cast<std::string>(firstStep->value);
                        if (firstValue == "[") {
                            firstStep->consarray = true;
                        }
                    } catch (const std::bad_any_cast&) {
                    }
                }

                auto lastStep = result->steps.back();
                if (lastStep->type == "unary") {
                    try {
                        std::string lastValue =
                            std::any_cast<std::string>(lastStep->value);
                        if (lastValue == "[") {
                            lastStep->consarray = true;
                        }
                    } catch (const std::bad_any_cast&) {
                    }
                }
            }

            resolveAncestry(result);

        } else if (opValue == "[") {
            // Predicated step
            if (!expr->lhs) return nullptr;
            result = processAST(expr->lhs);
            auto step = result;
            std::string type = "predicate";

            if (result && result->type == "path") {
                if (!result->steps.empty()) {
                    step = result->steps.back();
                    type = "stages";
                }
            }

            if (step && step->group) {
                throw JException("S0209", expr->position);
            }

            // Initialize containers if needed - matches Java lines 1073-1079
            if (type == "stages") {
                // Java: if (step.stages==null) step.stages = new ArrayList<>();
                // C++ vectors are already initialized, so no action needed
            } else {
                // Java: if (step.predicate==null) step.predicate = new
                // ArrayList<>(); C++ vectors are already initialized, so no
                // action needed
            }

            auto predicate = expr->rhs ? processAST(expr->rhs) : nullptr;
            if (predicate && !predicate->seekingParentList.empty()) {
                for (auto slot : predicate->seekingParentList) {
                    if (slot && slot->level == 1) {
                        seekParent(step, slot);
                    } else if (slot) {
                        slot->level--;
                    }
                }
                pushAncestry(step, predicate);
            }

            auto s = std::make_shared<Symbol>();
            s->type = "filter";
            s->expr = predicate;
            s->position = expr->position;

            // Preserve keepArray flag
            if (expr->keepArray && step) {
                step->keepArray = true;
            }

            if (step) {
                if (type == "stages") {
                    step->stages.push_back(s);
                } else {
                    step->predicate.push_back(s);
                }
            }

        } else if (opValue == "{") {
            // Group-by
            result = processAST(expr->lhs);
            if (result && result->group) {
                throw JException("S0210", expr->position);
            }

            // Object constructor - process each pair
            if (result) {
                result->group = std::make_shared<Symbol>();
                result->group->lhsObject.clear();

                for (const auto& pair : expr->rhsObject) {
                    auto processedKey = processAST(pair.first);
                    auto processedValue = processAST(pair.second);
                    result->group->lhsObject.emplace_back(processedKey,
                                                          processedValue);
                }
                result->group->position = expr->position;
            }

        } else if (opValue == "^") {
            // Order-by
            result = processAST(expr->lhs);
            if (!result || result->type != "path") {
                auto newResult = std::make_shared<Symbol>();
                newResult->type = "path";
                newResult->steps = {result};
                result = newResult;
            }

            auto sortStep = std::make_shared<Symbol>();
            sortStep->type = "sort";
            sortStep->position = expr->position;

            for (const auto& term : expr->rhsTerms) {
                auto processedExpr = processAST(term->expression);
                pushAncestry(sortStep, processedExpr);

                auto termResult = std::make_shared<Symbol>();
                termResult->descending = term->descending;
                termResult->expression = processedExpr;
                sortStep->terms.push_back(termResult);
            }

            result->steps.push_back(sortStep);
            resolveAncestry(result);

        } else if (opValue == ":=") {
            result = std::make_shared<Symbol>();
            result->type = "bind";
            result->value = expr->value;
            result->position = expr->position;
            result->lhs = processAST(expr->lhs);
            result->rhs = processAST(expr->rhs);
            pushAncestry(result, result->rhs);

        } else if (opValue == "@") {
            if (!expr->lhs || !expr->rhs) return nullptr;
            result = processAST(expr->lhs);
            auto step = result;
            if (result && result->type == "path") {
                if (!result->steps.empty()) {
                    step = result->steps.back();
                }
            }

            // Check for existing predicates/stages
            if (step && (!step->stages.empty() || !step->predicate.empty())) {
                throw JException("S0215", expr->position);
            }

            // Check for sort
            if (step && step->type == "sort") {
                throw JException("S0216", expr->position);
            }

            if (expr->keepArray && step) {
                step->keepArray = true;
            }

            if (step && expr->rhs) {
                step->focus = expr->rhs->value;
                step->tuple = true;
            }

        } else if (opValue == "#") {
            if (!expr->lhs || !expr->rhs) return nullptr;
            result = processAST(expr->lhs);
            auto step = result;
            if (result && result->type == "path") {
                if (!result->steps.empty()) {
                    step = result->steps.back();
                }
            } else {
                auto newResult = std::make_shared<Symbol>();
                newResult->type = "path";
                if (result) {
                    newResult->steps = {result};
                }
                result = newResult;
                if (step && !step->predicate.empty()) {
                    step->stages = step->predicate;
                    step->predicate.clear();
                }
            }

            if (step && expr->rhs) {
                if (step->stages.empty()) {
                    step->index = expr->rhs->value;
                } else {
                    auto indexSymbol = std::make_shared<Symbol>();
                    indexSymbol->type = "index";
                    indexSymbol->value = expr->rhs->value;
                    indexSymbol->position = expr->position;
                    step->stages.push_back(indexSymbol);
                }
                step->tuple = true;
            }

        } else if (opValue == "~>") {
            result = std::make_shared<Symbol>();
            result->type = "apply";
            result->value = expr->value;
            result->position = expr->position;
            result->lhs = processAST(expr->lhs);
            result->rhs = processAST(expr->rhs);
            result->keepArray = (result->lhs && result->lhs->keepArray) ||
                                (result->rhs && result->rhs->keepArray);

        } else {
            // Default binary processing
            result = std::make_shared<Symbol>();
            result->type = expr->type;
            result->value = expr->value;
            result->position = expr->position;
            result->lhs = processAST(expr->lhs);
            result->rhs = processAST(expr->rhs);
            pushAncestry(result, result->lhs);
            pushAncestry(result, result->rhs);
        }

    } else if (exprType == "unary") {
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->value = expr->value;
        result->position = expr->position;

        std::string exprValue;
        try {
            exprValue = std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast&) {
            exprValue = "";
        }

        if (exprValue == "[") {
            // Array constructor - process each item
            for (const auto& item : expr->expressions) {
                auto processedValue = processAST(item);
                pushAncestry(result, processedValue);
                result->expressions.push_back(processedValue);
            }
        } else if (exprValue == "{") {
            // Object constructor - process each pair
            for (const auto& pair : expr->lhsObject) {
                auto processedKey = processAST(pair.first);
                auto processedValue = processAST(pair.second);
                pushAncestry(result, processedKey);
                pushAncestry(result, processedValue);
                result->lhsObject.emplace_back(processedKey, processedValue);
            }
        } else {
            // All other unary expressions
            result->expression = processAST(expr->expression);

            // Special case: unary minus on a number
            if (exprValue == "-" && result->expression &&
                result->expression->type == "number") {
                result = result->expression;
                try {
                    double numValue = std::any_cast<double>(result->value);
                    result->value = Utils::convertNumber(-numValue);
                } catch (const std::bad_any_cast&) {
                    try {
                        int64_t intValue = Utils::toLong(result->value);
                        result->value = Utils::convertNumber(-intValue);
                    } catch (const std::bad_any_cast&) {
                        // Handle other numeric types
                    }
                }
            } else {
                pushAncestry(result, result->expression);
            }
        }

    } else if (exprType == "function" || exprType == "partial") {
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->name = expr->name;
        result->value = expr->value;
        result->position = expr->position;

        for (const auto& arg : expr->arguments) {
            auto processedArg = processAST(arg);
            pushAncestry(result, processedArg);
            result->arguments.push_back(processedArg);
        }
        result->procedure = processAST(expr->procedure);

    } else if (exprType == "lambda") {
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->arguments = expr->arguments;
        result->signature = expr->signature;
        result->position = expr->position;
        auto body = processAST(expr->body);
        result->body = tailCallOptimize(body);

    } else if (exprType == "condition") {
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->position = expr->position;
        result->condition = processAST(expr->condition);
        pushAncestry(result, result->condition);
        result->then_expr = processAST(expr->then_expr);
        pushAncestry(result, result->then_expr);
        if (expr->else_expr) {
            result->else_expr = processAST(expr->else_expr);
            pushAncestry(result, result->else_expr);
        }

    } else if (exprType == "transform") {
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->position = expr->position;
        result->pattern = processAST(expr->pattern);
        result->update = processAST(expr->update);
        if (expr->delete_) {
            result->delete_ = processAST(expr->delete_);
        }

    } else if (exprType == "sort") {
        // Sort operator: array^(expression)
        // LHS is the array to be ordered, terms define the sorting criteria
        result = processAST(expr->lhs);
        if (!result || result->type != "path") {
            // Wrap in a path if not already one
            auto pathResult = std::make_shared<Symbol>();
            pathResult->type = "path";
            // Only add result to steps if it's not null
            if (result) {
                pathResult->steps.push_back(result);
            }
            result = pathResult;
        }

        auto sortStep = std::make_shared<Symbol>();
        sortStep->type = "sort";
        sortStep->position = expr->position;

        // Process each sort term
        for (const auto& term : expr->terms) {
            if (term && term->expression) {
                auto processedExpr = processAST(term->expression);
                if (processedExpr) {
                    pushAncestry(sortStep, processedExpr);

                    auto termResult = std::make_shared<Symbol>();
                    termResult->descending = term->descending;
                    termResult->expression = processedExpr;
                    sortStep->terms.push_back(termResult);
                }
            }
        }

        result->steps.push_back(sortStep);
        resolveAncestry(result);

    } else if (exprType == "block") {
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->position = expr->position;

        for (const auto& item : expr->expressions) {
            auto part = processAST(item);
            pushAncestry(result, part);
            if (part && (part->consarray ||
                         (part->type == "path" && !part->steps.empty() &&
                          part->steps[0]->consarray))) {
                result->consarray = true;
            }
            result->expressions.push_back(part);
        }

    } else if (exprType == "name") {
        result = std::make_shared<Symbol>();
        result->type = "path";
        result->steps = {expr};
        if (expr->keepArray) {
            result->keepSingletonArray = true;
        }

    } else if (exprType == "parent") {
        result = std::make_shared<Symbol>();
        result->type = "parent";

        auto slotSymbol = std::make_shared<Symbol>();
        slotSymbol->label = "!" + std::to_string(ancestorLabel_++);
        slotSymbol->level = 1;
        slotSymbol->index = static_cast<int64_t>(ancestorIndex_++);
        result->slot = slotSymbol;

        ancestry_.push_back(result);

    } else if (exprType == "apply") {
        // Function application expression - process LHS and RHS
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->value = expr->value;
        result->position = expr->position;
        result->lhs = processAST(expr->lhs);
        result->rhs = processAST(expr->rhs);
        result->keepArray = (result->lhs && result->lhs->keepArray) ||
                            (result->rhs && result->rhs->keepArray);
        pushAncestry(result, result->lhs);
        pushAncestry(result, result->rhs);

    } else if (exprType == "range") {
        // Range expressions are handled as binary expressions
        result = std::make_shared<Symbol>();
        result->type = expr->type;
        result->value = expr->value;
        result->position = expr->position;
        result->lhs = processAST(expr->lhs);
        result->rhs = processAST(expr->rhs);
        pushAncestry(result, result->lhs);
        pushAncestry(result, result->rhs);

    } else if (exprType == "string" || exprType == "number" ||
               exprType == "value" || exprType == "wildcard" ||
               exprType == "descendant" || exprType == "variable" ||
               exprType == "regex") {
        result = expr;

    } else if (exprType == "operator") {
        // Handle special operators that might be used as names
        std::string opValue;
        try {
            opValue = std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast&) {
            opValue = "";
        }

        if (opValue == "and" || opValue == "or" || opValue == "in") {
            expr->type = "name";
            result = processAST(expr);
        } else if (opValue == "?") {
            // Partial application
            result = expr;
        } else {
            throw JException("S0201", expr->position, expr->value);
        }

    } else if (exprType == "error") {
        result = expr;
        if (expr->lhs) {
            result = processAST(expr->lhs);
        }

    } else {
        // Unknown expression type
        std::string code = "S0206";
        if (expr->id == "(end)") {
            code = "S0207";
        }
        auto err = JException(code, expr->position, expr->value);
        if (recover_) {
            errors_.push_back(std::make_shared<JException>(err));
            auto ret = std::make_shared<Symbol>();
            ret->type = "error";
            ret->error = std::make_shared<JException>(err);
            result = ret;
        } else {
            throw err;
        }
    }

    // Preserve keepArray flag from original expression (Java lines 1393-1396)
    if (expr && expr->keepArray && result) {
        result->keepArray = true;
    }

    return result;
}

std::shared_ptr<Parser::Symbol> Parser::objectParser(
    std::shared_ptr<Symbol> left) {
    std::shared_ptr<Symbol> res;

    // Create appropriate symbol type based on whether we have a left operand
    if (left != nullptr) {
        // Binary infix form: expr{key: value}
        res = std::make_shared<Symbol>();
        res->id = "{";
        res->type = "binary";
        res->value = std::string("{");  // Set value field like Java reference
        res->lhs = left;
    } else {
        // Unary prefix form: {key: value}
        res = std::make_shared<Symbol>();
        res->id = "{";
        res->type = "unary";
        res->value = std::string("{");  // Set value field like Java reference
    }

    res->position = node_->position;

    // Parse object pairs: {key: value, key: value, ...}
    std::vector<std::pair<std::shared_ptr<Symbol>, std::shared_ptr<Symbol>>>
        pairs;

    if (node_->id != "}") {
        while (true) {
            // Parse key expression
            auto key = expression(0);
            advance(":");
            // Parse value expression
            auto value = expression(0);

            // Store the key-value pair
            pairs.emplace_back(key, value);

            if (node_->id != ",") {
                break;
            }
            advance(",");
        }
    }
    advance("}", true);

    // Store the pairs based on form type
    if (left == nullptr) {
        // Unary prefix form
        res->lhsObject = pairs;
    } else {
        // Binary infix form
        res->rhsObject = pairs;
    }

    return res;
}

}  // namespace jsonata