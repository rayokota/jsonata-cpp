#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <string>

// The left-hand side of ?: and ?? is both the condition and the result, so it
// must not be processed twice. Expected results match the JavaScript reference
// implementation.
namespace jsonata {
namespace {

using nlohmann::ordered_json;

ordered_json eval(const std::string& expression,
                  const ordered_json& input = nullptr) {
    return Jsonata(expression).evaluate(input);
}

TEST(ConditionTest, defaultNegativeNumber) {
    EXPECT_EQ(eval("-5 ?: 99"), ordered_json(-5));
}

TEST(ConditionTest, defaultNegativeIndex) {
    EXPECT_EQ(eval("[1,2,3][-1] ?: 42"), ordered_json(3));
    EXPECT_EQ(eval("[true][-1] ?: \"no\""), ordered_json(true));
}

TEST(ConditionTest, defaultFalsyLeftUsesDefault) {
    EXPECT_EQ(eval("0 ?: 99"), ordered_json(99));
    EXPECT_EQ(eval("[1,2,3][-5] ?: 42"), ordered_json(42));
}

TEST(ConditionTest, coalescingNegativeNumber) {
    EXPECT_EQ(eval("-5 ?? 99"), ordered_json(-5));
}

TEST(ConditionTest, coalescingNegativeIndex) {
    EXPECT_EQ(eval("[1,2,3][-1] ?? 42"), ordered_json(3));
    EXPECT_EQ(eval("[1,2,3][-5] ?? 42"), ordered_json(42));
}

TEST(ConditionTest, leftHandSideWithRegexAndLambda) {
    EXPECT_EQ(eval("$match(\"abc\", /b/).match ?: \"none\""),
              ordered_json("b"));
    EXPECT_EQ(eval("(function($x){-$x})(5) ?? 0"), ordered_json(-5));
}

TEST(ConditionTest, pathLeftHandSide) {
    ordered_json input = ordered_json::parse(R"({"a": {"b": [1, 2, 3]}})");
    EXPECT_EQ(eval("a.b[-1] ?: 0", input), ordered_json(3));
    EXPECT_EQ(eval("a.c ?? \"dflt\"", input), ordered_json("dflt"));
}

}  // namespace
}  // namespace jsonata
