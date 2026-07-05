#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <vector>
#include <memory>
#include <string>

namespace jsonata {

class StringTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    static nlohmann::ordered_json makeObject(const std::map<std::string, std::string>& data) {
        nlohmann::ordered_json obj = nlohmann::ordered_json::object();
        for (const auto& [k, v] : data) obj[k] = v;
        return obj;
    }
    static nlohmann::ordered_json makeObject(const std::map<std::string, int>& data) {
        nlohmann::ordered_json obj = nlohmann::ordered_json::object();
        for (const auto& [k, v] : data) obj[k] = v;
        return obj;
    }
};

TEST_F(StringTest, stringTest) {
    auto result1 = Jsonata("$string($)").evaluate(nlohmann::ordered_json("abc"));
    ASSERT_TRUE(result1.is_string());
    EXPECT_EQ(result1.get<std::string>(), "abc");

    auto result2 = Jsonata("$string(100.0)").evaluate(nullptr);
    ASSERT_TRUE(result2.is_string());
    EXPECT_EQ(result2.get<std::string>(), "100");
}

TEST_F(StringTest, DISABLED_stringExponentTest) {
    auto data1 = makeObject({{"x", 100}});
    auto result1 = Jsonata("$string(x)").evaluate(data1);
    ASSERT_TRUE(result1.is_string());
    EXPECT_EQ(result1.get<std::string>(), "100");

    auto data2 = nlohmann::ordered_json::parse("{\"x\": 100000000000000000000}");
    auto result2 = Jsonata("$string(x)").evaluate(data2);
    ASSERT_TRUE(result2.is_string());
    EXPECT_EQ(result2.get<std::string>(), "100000000000000000000");

    auto data3 = nlohmann::ordered_json::parse("{\"x\": 1000000000000000000000}");
    auto result3 = Jsonata("$string(x)").evaluate(data3);
    ASSERT_TRUE(result3.is_string());
    EXPECT_EQ(result3.get<std::string>(), "1e+21");
}

TEST_F(StringTest, booleanTest) {
    auto result = Jsonata("$string($)").evaluate(nlohmann::ordered_json(true));
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "true");
}

TEST_F(StringTest, numberTest) {
    auto result = Jsonata("$string(5)").evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "5");
}

TEST_F(StringTest, arrayTest) {
    auto result = Jsonata("[1..5].$string()").evaluate(nullptr);
    ASSERT_TRUE(result.is_array());
    std::vector<std::string> expected = {"1", "2", "3", "4", "5"};
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_TRUE(result[i].is_string());
        EXPECT_EQ(result[i].get<std::string>(), expected[i]);
    }
}

TEST_F(StringTest, mapTest) {
    nlohmann::ordered_json emptyMap = nlohmann::ordered_json::object();
    auto result = Jsonata("$string($)").evaluate(emptyMap);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "{}");
}

TEST_F(StringTest, map2Test) {
    auto data = makeObject({{"x", 1}});
    auto result = Jsonata("$string($)").evaluate(data);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "{\"x\":1}");
}

TEST_F(StringTest, escapeTest) {
    auto result1 = Jsonata("$string($)").evaluate(makeObject({{"a", std::string("\"")}}));
    ASSERT_TRUE(result1.is_string());
    // The result contains an escaped quote in JSON string
    EXPECT_EQ(result1.get<std::string>(), "{\"a\":\"\\\"\"}");

    auto result2 = Jsonata("$string($)").evaluate(makeObject({{"a", std::string("\\")}}));
    ASSERT_TRUE(result2.is_string());
    EXPECT_EQ(result2.get<std::string>(), "{\"a\":\"\\\\\"}");

    auto result3 = Jsonata("$string($)").evaluate(makeObject({{"a", std::string("\t")}}));
    ASSERT_TRUE(result3.is_string());
    EXPECT_EQ(result3.get<std::string>(), "{\"a\":\"\\t\"}");

    auto result4 = Jsonata("$string($)").evaluate(makeObject({{"a", std::string("\n")}}));
    ASSERT_TRUE(result4.is_string());
    EXPECT_EQ(result4.get<std::string>(), "{\"a\":\"\\n\"}");

    auto result5 = Jsonata("$string($)").evaluate(makeObject({{"a", "</"}}));
    ASSERT_TRUE(result5.is_string());
    EXPECT_EQ(result5.get<std::string>(), "{\"a\":\"</\"}");
}

TEST_F(StringTest, splitTest) {
    nlohmann::ordered_json emptyMap = nlohmann::ordered_json::object();
    auto result1 = Jsonata("$split(a, '-')").evaluate(emptyMap);
    EXPECT_TRUE(result1.is_null());

    auto result2 = Jsonata("a ~> $split('-')").evaluate(emptyMap);
    EXPECT_TRUE(result2.is_null());

    auto result3 = Jsonata("$split('', '')").evaluate(nullptr);
    ASSERT_TRUE(result3.is_array());
    EXPECT_EQ(result3.size(), 0);

    auto result4 = Jsonata("$split('a1b2c3d4', '', 4)").evaluate(nullptr);
    ASSERT_TRUE(result4.is_array());
    std::vector<std::string> expected = {"a", "1", "b", "2"};
    ASSERT_EQ(result4.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_TRUE(result4[i].is_string());
        EXPECT_EQ(result4[i].get<std::string>(), expected[i]);
    }

    auto result5 = Jsonata("$split('this..is.a.test', '.')").evaluate(nullptr);
    ASSERT_TRUE(result5.is_array());
    std::vector<std::string> expected5 = {"this", "", "is", "a", "test"};
    ASSERT_EQ(result5.size(), expected5.size());
    for (size_t i = 0; i < expected5.size(); ++i) {
        ASSERT_TRUE(result5[i].is_string());
        EXPECT_EQ(result5[i].get<std::string>(), expected5[i]);
    }

    auto result6 = Jsonata("$split('this..is.a.test...', '.')").evaluate(nullptr);
    ASSERT_TRUE(result6.is_array());
    std::vector<std::string> expected6 = {"this", "", "is", "a", "test", "", "", ""};
    ASSERT_EQ(result6.size(), expected6.size());
    for (size_t i = 0; i < expected6.size(); ++i) {
        ASSERT_TRUE(result6[i].is_string());
        EXPECT_EQ(result6[i].get<std::string>(), expected6[i]);
    }
}

TEST_F(StringTest, trimTest) {
    auto result1 = Jsonata("$trim(\"\n\")").evaluate(nullptr);
    ASSERT_TRUE(result1.is_string());
    EXPECT_EQ(result1.get<std::string>(), "");

    auto result2 = Jsonata("$trim(\" \")").evaluate(nullptr);
    ASSERT_TRUE(result2.is_string());
    EXPECT_EQ(result2.get<std::string>(), "");

    auto result3 = Jsonata("$trim(\"\")").evaluate(nullptr);
    ASSERT_TRUE(result3.is_string());
    EXPECT_EQ(result3.get<std::string>(), "");

    auto result4 = Jsonata("$trim(notthere)").evaluate(nullptr);
    EXPECT_TRUE(result4.is_null());
}

TEST_F(StringTest, evalTest) {
    auto result = Jsonata("(\n  $data := {'Wert1': 'AAA', 'Wert2': 'BBB'};\n  $eval('$data.Wert1')\n)").evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "AAA");
}

TEST_F(StringTest, evalSeesEnclosingVariableBindingTest) {
    // $eval's dynamically-parsed expression must see variables bound in
    // the enclosing scope (here, via an in-expression := assignment), not
    // just a static top-level environment (regression test).
    Jsonata expr("($x := 5; $eval(\"$x + 1\"))");
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int64_t>(), 6);
}

TEST_F(StringTest, evalSeesExplicitTopLevelBindingsTest) {
    // Same as above, but for bindings passed via evaluate()'s bindings
    // argument rather than an in-expression assignment.
    Jsonata expr("$eval(\"$x\")");
    auto bindingFrame = expr.createFrame();
    bindingFrame->bind("x", int64_t(42));
    auto result = expr.evaluate(nullptr, bindingFrame);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int64_t>(), 42);
}

TEST_F(StringTest, evalUnaffectedBySiblingArgumentScopeTest) {
    // $eval's second (focus) argument is evaluated before its own body
    // runs, and here contains a nested block with its own environment.
    // Confirms evaluating that sibling argument doesn't leave the tracked
    // "current" environment pointing at the inner block's scope, which
    // would otherwise cause $eval to resolve $x (from the outer scope) as
    // undefined instead of 5. (jsonata-python had this bug; jsonata-cpp
    // does not, verified as a regression test.)
    Jsonata expr("($x := 5; $eval(\"$x\", (($y := 1; $y))))");
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int64_t>(), 5);
}

TEST_F(StringTest, stackGuardrailStopsNonTailRecursionTest) {
    // Ackermann is not tail-recursive, so its eval-apply depth grows with
    // each call. A small `stack` bound should trip before it completes.
    std::string ackExpr =
        "($ack := function($m, $n) { $m = 0 ? $n + 1 : ($n = 0 ? "
        "$ack($m - 1, 1) : $ack($m - 1, $ack($m, $n - 1))) }; $ack(4, 3))";
    Jsonata expr(ackExpr, defaultRegexEngine(), std::nullopt, 50);
    try {
        expr.evaluate(nullptr);
        FAIL() << "Expected JException D1011";
    } catch (const JException& e) {
        EXPECT_EQ(e.getError(), "D1011");
    }
}

TEST_F(StringTest, stackGuardrailAllowsBoundedRecursionTest) {
    // A stack bound that's high enough for the actual recursion depth
    // should not interfere with a normal, terminating evaluation.
    std::string ackExpr =
        "($ack := function($m, $n) { $m = 0 ? $n + 1 : ($n = 0 ? "
        "$ack($m - 1, 1) : $ack($m - 1, $ack($m, $n - 1))) }; $ack(3, 4))";
    Jsonata expr(ackExpr, defaultRegexEngine(), std::nullopt, 1000);
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int64_t>(), 125);
}

TEST_F(StringTest, timeoutGuardrailStopsInfiniteTailRecursionTest) {
    // Tail recursion doesn't grow the eval-apply depth, so only the
    // `timeout` guardrail (not `stack`) can catch this infinite loop.
    Jsonata expr("($f := function($n) { $f($n + 1) }; $f(0))",
                defaultRegexEngine(), 100, std::nullopt);
    try {
        expr.evaluate(nullptr);
        FAIL() << "Expected JException D1012";
    } catch (const JException& e) {
        EXPECT_EQ(e.getError(), "D1012");
    }
}

TEST_F(StringTest, stackGuardrailPropagatesThroughEvalTest) {
    // $eval() reuses the enclosing instance's environment directly, so a
    // `stack` bound configured on the outer expression must also apply to
    // dynamically-evaluated code (regression test for guardrail bypass).
    std::string ackExpr =
        "($ack := function($m, $n) { $m = 0 ? $n + 1 : ($n = 0 ? "
        "$ack($m - 1, 1) : $ack($m - 1, $ack($m, $n - 1))) }; "
        "$ack(4, 3))";
    Jsonata expr("$eval(\"" + ackExpr + "\")", defaultRegexEngine(),
                std::nullopt, 50);
    // Unlike jsonata-java (which wraps every $eval-time error, guardrail or
    // not, into a generic D3121), jsonata-cpp's functionEval re-throws
    // JException as-is, so the original D1011 code survives here.
    try {
        expr.evaluate(nullptr);
        FAIL() << "Expected JException D1011";
    } catch (const JException& e) {
        EXPECT_EQ(e.getError(), "D1011");
    }
}

TEST_F(StringTest, regexTest) {
    auto input = makeObject({{"foo", 1}, {"bar", 2}});
    auto result = Jsonata("($matcher := $eval('/^' & 'foo' & '/i'); $.$spread()[$.$keys() ~> $matcher])").evaluate(input);
    ASSERT_TRUE(result.is_object());
    auto expected = makeObject({{"foo", 1}});
    EXPECT_EQ(result, expected);
}

TEST_F(StringTest, fieldnameWithSpecialCharTest) {
    Jsonata expr("$ ~> |$|{}|");
    nlohmann::ordered_json input;
    input["a\nb"] = "c\nd";
    auto result = expr.evaluate(input);
    EXPECT_EQ(result, input);
}

TEST_F(StringTest, regexLiteralTest) {
    // Verify regex at end of expression doesn't crash (issue #88)
    EXPECT_NO_THROW({
        Jsonata expr("/^test.*$/");
        expr.evaluate(nullptr);
    });
}

TEST_F(StringTest, evalRegexTest) {
    // Verify $eval of regex at end of expression doesn't crash (issue #88)
    EXPECT_NO_THROW({
        Jsonata expr("$eval('/^test.*$/')");
        expr.evaluate(nullptr);
    });
}

TEST_F(StringTest, evalRegexCheckAnswerDataTest) {
    Jsonata expr("(\n    $matcher := $eval('/l/');\n    ('Hello World' ~> $matcher);\n)");
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["match"].get<std::string>(), "l");
    EXPECT_EQ(result["start"].get<long long>(), 2);
    EXPECT_EQ(result["end"].get<long long>(), 3);
    ASSERT_TRUE(result["groups"].is_array());
    EXPECT_EQ(result["groups"][0].get<std::string>(), "l");
}

TEST_F(StringTest, evalRegexCallNextTest) {
    Jsonata expr("(\n    $matcher := $eval('/l/');\n    ('Hello World' ~> $matcher).next();\n)");
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["match"].get<std::string>(), "l");
    EXPECT_EQ(result["start"].get<long long>(), 3);
    EXPECT_EQ(result["end"].get<long long>(), 4);
    ASSERT_TRUE(result["groups"].is_array());
    EXPECT_EQ(result["groups"][0].get<std::string>(), "l");
}

TEST_F(StringTest, DISABLED_replaceTest) {
    auto input = nlohmann::ordered_json("http://example.org/test{par}");
    auto result = Jsonata("$replace($, /{par}/, '')").evaluate(input);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "http://example.org/test");
}

} // namespace jsonata
