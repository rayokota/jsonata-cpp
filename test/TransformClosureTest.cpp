#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>

#include <nlohmann/json.hpp>
#include <string>

// A transform (`|pattern|update,delete|`) closes over the environment it is
// defined in, so its clauses can refer to enclosing variables. Expected
// results match the JavaScript reference implementation.

namespace jsonata {
namespace {

using nlohmann::ordered_json;

ordered_json eval(const std::string& expression,
                  const ordered_json& input = nullptr) {
    return Jsonata(expression).evaluate(input);
}

TEST(TransformClosureTest, updateSeesBlockVariable) {
    EXPECT_EQ(eval(R"(($v := 2; {"x": 1} ~> |$|{"y": $v}|))"),
              ordered_json::parse(R"({"x": 1, "y": 2})"));
}

TEST(TransformClosureTest, boundTransformSeesBlockVariable) {
    EXPECT_EQ(eval(R"(($v := 2; $t := |$|{"y": $v}|; $t({"x": 1})))"),
              ordered_json::parse(R"({"x": 1, "y": 2})"));
}

TEST(TransformClosureTest, patternSeesBlockVariable) {
    EXPECT_EQ(
        eval(
            R"(($p := "b"; {"a": {"n": 1}, "b": {"n": 2}} ~> |$lookup($, $p)|{"hit": true}|))"),
        ordered_json::parse(R"({"a": {"n": 1}, "b": {"n": 2, "hit": true}})"));
}

TEST(TransformClosureTest, deleteSeesBlockVariable) {
    EXPECT_EQ(eval(R"(($k := "x"; {"x": 1, "y": 2} ~> |$|{}, $k|))"),
              ordered_json::parse(R"({"y": 2})"));
}

// Variables are resolved where the transform was defined, not where it is
// called.
TEST(TransformClosureTest, lexicalScope) {
    EXPECT_EQ(
        eval(R"(($v := 1; $t := |$|{"y": $v}|; ($v := 2; $t({"x": 1}))))"),
        ordered_json::parse(R"({"x": 1, "y": 1})"));
}

TEST(TransformClosureTest, seesEnclosingLambdaParameter) {
    EXPECT_EQ(
        eval(
            R"(($f := function($v){ |$|{"y": $v}| }; $g := $f(3); $g({"x": 1})))"),
        ordered_json::parse(R"({"x": 1, "y": 3})"));
}

TEST(TransformClosureTest, seesRootInput) {
    EXPECT_EQ(eval(R"(o ~> |$|{"y": $$.r}|)",
                   ordered_json::parse(R"({"r": 9, "o": {"x": 1}})")),
              ordered_json::parse(R"({"x": 1, "y": 9})"));
}

}  // namespace
}  // namespace jsonata
