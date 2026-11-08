#include <gtest/gtest.h>
#include <jsonata/Functions.h>
#include <jsonata/JException.h>
#include <jsonata/Jsonata.h>
#include <jsonata/utils/Signature.h>
#include <nlohmann/json.hpp>
#include <functional>
#include <memory>
#include <string>

// Partial application of native functions follows jsonata-js: the partial is
// a lambda whose body is the native function itself, so applying it calls the
// native directly, without looking it up by name and without validating its
// signature again. Ported from jsonata-java (PartialTest, OldPrsTest and
// EachMergeTest).
namespace jsonata {
namespace {

using nlohmann::ordered_json;

ordered_json eval(const std::string& expression,
                  const ordered_json& input = nullptr) {
    return Jsonata(expression).evaluate(input);
}

double num(const std::any& value) { return Utils::toDouble(value); }

JFunction addFunction(const char* signature) {
    JFunction add([](const Utils::JList& args, const std::any&,
                     std::shared_ptr<Frame>) -> std::any {
        return num(args[0]) + num(args[1]);
    });
    if (signature) {
        add.signature = std::make_shared<utils::Signature>(signature, "add");
    }
    return add;
}

// jsonata-java #79
TEST(PartialApplicationTest, eachEmptyArray) {
    EXPECT_TRUE(eval("[] ~> $each(?, function($v) { $v })",
                     ordered_json::object())
                    .is_null());
}

TEST(PartialApplicationTest, eachArrayWithData) {
    EXPECT_EQ(eval("[123, 321] ~> $each(?, function($v) { $v })",
                   ordered_json::object()),
              ordered_json::parse("[123, 321]"));
}

// Like JS for..in, $each iterates array indexes; the signature is not
// checked for a partially applied native
TEST(PartialApplicationTest, eachArray) {
    EXPECT_EQ(eval("[5,6] ~> $each(?, function($v,$k) { $k })"),
              ordered_json::parse(R"(["0", "1"])"));
}

// The signature is still checked when the function is called directly
TEST(PartialApplicationTest, eachArrayDirect) {
    try {
        eval("$each([5,6], function($v,$k) { $k })");
        FAIL() << "Expected JException";
    } catch (const JException& ex) {
        EXPECT_EQ(ex.getError(), "T0410");
    }
}

TEST(PartialApplicationTest, mergeWithArray) {
    EXPECT_EQ(eval("$ ~> $merge(?)",
                   ordered_json::parse(R"([[], {"test": "data"}])")),
              ordered_json::parse(R"({"test": "data"})"));
}

// jsonata-java #86, #87
TEST(PartialApplicationTest, mapAppend) {
    EXPECT_EQ(eval("{'systems': 'asd2'}.$map($, $append(?))"),
              ordered_json::parse(R"({"systems": "asd2"})"));
    ordered_json input = ordered_json::parse(R"({"systems": "asd"})");
    EXPECT_EQ(eval("$map($, $append(?))", input), input);
}

// The native is called without validating its signature, but arguments it
// can't handle are still reported as a signature error, not ignored. The
// error names the native, not the higher-order function calling it.
TEST(PartialApplicationTest, argumentOfWrongType) {
    for (const char* expression :
         {"$substring(?, 1)(123)", "$map([1, 2], $substring(?, 1))"}) {
        try {
            eval(expression);
            ADD_FAILURE() << "Expected JException for " << expression;
        } catch (const JException& ex) {
            EXPECT_EQ(ex.getError(), "T0410") << expression;
            EXPECT_EQ(std::any_cast<std::string>(ex.getExpected()), "substring")
                << expression;
        }
    }
}

TEST(PartialApplicationTest, missingArgsBound) {
    EXPECT_EQ(eval("$substring(?, 1)('hello')"), "ello");
}

// jsonata-java #80
TEST(PartialApplicationTest, mapInPipe) {
    ordered_json input = ordered_json::parse(R"({"buildings": {
        "home": {"address": "home address", "period": "11 month on 12",
                 "presentation": {"first": {"widget": {"data": "home widget"}}}},
        "work": {"address": "work address",
                 "presentation": {"first": {"widget": {"data": "work widget"}}}},
        "vacancy": {"address": "vacancy address", "period": "1 month on 12",
                    "presentation": {"first": {"widget": {"data": "vacancy widget"}}}},
        "old": {"address": "old address", "period": "2 month on 12",
                "presentation": {"first": {"widget": {"data": "old widget"}}}},
        "operation": {"address": "operation address", "period": "3 month on 12",
                      "presentation": {"first": {"widget": {"data": "operation widget"}}}}}})");
    ordered_json expected = ordered_json::parse(R"([
        {"first": {"widget": {"data": "home widget"}}},
        {"first": {"widget": {"data": "work widget"}}},
        {"first": {"widget": {"data": "old widget"}}},
        {"first": {"widget": {"data": "operation widget"}}}])");
    EXPECT_EQ(
        eval("$sift($.buildings, function($v, $k) { $k ~> /o/ and "
             "$v.presentation.*.widget})"
             " ~> $each(?, function($v, $key) {{ $key: $sift($v.presentation, "
             "function($v, $k){ $v.widget }) }})"
             " ~> $map(?, function($v){($v.*)})",
             input),
        expected);
}

// Forgot the leading $
TEST(PartialApplicationTest, missingDollar) {
    try {
        eval("substring(?, 1)");
        FAIL() << "Expected JException";
    } catch (const JException& ex) {
        EXPECT_EQ(ex.getError(), "T1007");
    }
}

// jsonata-java #83: custom functions, with or without a signature
TEST(PartialApplicationTest, customFunction) {
    Jsonata expression("$add(1, 2) ~> $add(?, 3)");
    expression.registerFunction("add", addFunction("<nn:n>"));
    EXPECT_EQ(expression.evaluate(nullptr), ordered_json(6));
}

TEST(PartialApplicationTest, customFunctionWithoutSignature) {
    Jsonata expression("$add(1, 2) ~> $add(?, 3)");
    expression.registerFunction("add", addFunction(nullptr));
    EXPECT_EQ(expression.evaluate(nullptr), ordered_json(6));
}

// Like a direct call, the partial passes every declared parameter, omitted
// ones as undefined
TEST(PartialApplicationTest, customFunctionOptionalArgs) {
    JFunction f([](const Utils::JList& args, const std::any&,
                   std::shared_ptr<Frame>) -> std::any {
        return std::string(args.size() == 2 && !args[1].has_value()
                               ? "omitted"
                               : "size " + std::to_string(args.size()));
    });
    f.signature = std::make_shared<utils::Signature>("<nn?:s>", "f");
    for (const char* expression : {"$f(5)", "$f(?)(5)", "$map([5], $f(?))"}) {
        Jsonata expr(expression);
        expr.registerFunction("f", f);
        EXPECT_EQ(expr.evaluate(nullptr), ordered_json("omitted")) << expression;
    }
}

TEST(PartialApplicationTest, stdFunctionWithoutSignature) {
    Jsonata expression("$add(1, 2) ~> $add(?, 3)");
    expression.registerFunction(
        "add", std::function<std::any(const Utils::JList&)>(
                   [](const Utils::JList& args) -> std::any {
                       return num(args[0]) + num(args[1]);
                   }));
    EXPECT_EQ(expression.evaluate(nullptr), ordered_json(6));
}

// The partial calls the function itself, not a function looked up by name, so
// a function bound only in the frame passed to evaluate() can be partially
// applied
TEST(PartialApplicationTest, functionBoundInFrame) {
    Jsonata expression("$f(1, 2) ~> $f(?, 3)");
    auto frame = expression.createFrame();
    frame->bind("f", addFunction(nullptr));
    EXPECT_EQ(expression.evaluate(nullptr, frame), ordered_json(6));
}

TEST(PartialApplicationTest, functionBoundToVariable) {
    Jsonata expression("($g := $add; $g(?, 3)(4))");
    expression.registerFunction("add", addFunction("<nn:n>"));
    EXPECT_EQ(expression.evaluate(nullptr), ordered_json(7));
}

// jsonata-java #85: the input is passed to a function called directly
TEST(PartialApplicationTest, functionWithInputData) {
    auto factor = [](const std::any& input) {
        const auto& map =
            std::any_cast<const nlohmann::ordered_map<std::string, std::any>&>(
                input);
        return std::stod(std::any_cast<std::string>(map.at("factor")));
    };
    Jsonata expression("$multiply(3) ~> $multiply(?, 2)");
    expression.registerFunction(
        "multiply",
        JFunction([factor](const Utils::JList& args, const std::any& input,
                           std::shared_ptr<Frame>) -> std::any {
            double second = args.size() > 1 ? num(args[1]) : factor(input);
            return num(args[0]) * second;
        }));
    EXPECT_EQ(expression.evaluate(ordered_json::parse(R"({"factor": "3"})")),
              ordered_json(18));
}

// $each and $merge iterate arrays like JS for..in. Functions without a
// signature let arrays reach them.
ordered_json evalRaw(const std::string& expression,
                     const ordered_json& input = nullptr) {
    Jsonata e(expression);
    e.registerFunction(
        "eachRaw", JFunction([](const Utils::JList& args, const std::any&,
                                std::shared_ptr<Frame>) -> std::any {
            return Functions::each(args[0], args[1]);
        }));
    e.registerFunction(
        "mergeRaw", JFunction([](const Utils::JList& args, const std::any&,
                                 std::shared_ptr<Frame>) -> std::any {
            return Functions::merge(args);
        }));
    return e.evaluate(input);
}

TEST(EachMergeTest, eachArrayKeys) {
    EXPECT_EQ(evalRaw("$eachRaw([5,6], function($v,$k) { $k })"),
              ordered_json::parse(R"(["0", "1"])"));
}

TEST(EachMergeTest, eachArrayValues) {
    EXPECT_EQ(evalRaw("$eachRaw([5,6], function($v) { $v })"),
              ordered_json::parse("[5, 6]"));
}

TEST(EachMergeTest, eachArrayAllArgs) {
    EXPECT_EQ(
        evalRaw("[$eachRaw($, function($v,$k,$o) { {'v': $v, 'k': $k, 'o': $o} "
                "})]",
                ordered_json::parse("[5]")),
        ordered_json::parse(R"([{"v": 5, "k": "0", "o": [5]}])"));
}

TEST(EachMergeTest, eachArraySkipsUndefined) {
    EXPECT_EQ(evalRaw("$eachRaw([5,6,7], function($v) { $v > 5 ? $v })"),
              ordered_json::parse("[6, 7]"));
}

TEST(EachMergeTest, eachEmptyArrayAndUndefined) {
    EXPECT_TRUE(evalRaw("$eachRaw([], function($v) { $v })").is_null());
    EXPECT_TRUE(evalRaw("$eachRaw(nothing, function($v) { $v })").is_null());
}

TEST(EachMergeTest, eachMap) {
    EXPECT_EQ(evalRaw("$eachRaw({'a': 1, 'b': 2}, function($v, $k) { $k & '=' "
                      "& $v })"),
              ordered_json::parse(R"(["a=1", "b=2"])"));
    EXPECT_EQ(eval("$each({'a': 1, 'b': 2}, function($v, $k) { $k & '=' & $v "
                   "})"),
              ordered_json::parse(R"(["a=1", "b=2"])"));
}

TEST(EachMergeTest, mergeArray) {
    EXPECT_EQ(evalRaw("$mergeRaw([[5,6]])"),
              ordered_json::parse(R"({"0": 5, "1": 6})"));
}

TEST(EachMergeTest, mergeArrayAndMap) {
    EXPECT_EQ(evalRaw("$mergeRaw($)",
                      ordered_json::parse(R"([[], {"test": "data"}])")),
              ordered_json::parse(R"({"test": "data"})"));
}

TEST(EachMergeTest, mergeMaps) {
    EXPECT_EQ(eval("$merge([{'a':1,'b':2},{'b':3}])"),
              ordered_json::parse(R"({"a": 1, "b": 3})"));
}

}  // namespace
}  // namespace jsonata
