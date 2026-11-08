#include <gtest/gtest.h>
#include <jsonata/JException.h>
#include <jsonata/Jsonata.h>
#include <jsonata/utils/Signature.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

// Signature errors (T0410) name the function the way jsonata-js does: by the
// name used at the call site (its registered name or the variable it is bound
// to). Ported from jsonata-java (FunctionNameTest).
namespace jsonata {
namespace {

using nlohmann::ordered_json;

JException error(const std::string& expression,
                 const ordered_json& input = nullptr) {
    Jsonata e(expression);
    JFunction test([](const Utils::JList& args, const std::any&,
                      std::shared_ptr<Frame>) -> std::any { return args[0]; });
    test.signature = std::make_shared<utils::Signature>("<s:s>", "test");
    e.registerFunction("test", test);
    try {
        e.evaluate(input);
    } catch (const JException& ex) {
        EXPECT_EQ(ex.getError(), "T0410") << ex.what();
        return ex;
    }
    ADD_FAILURE() << "Expected JException for " << expression;
    return JException("none");
}

std::string name(const JException& ex) {
    return ex.getExpected().has_value()
               ? std::any_cast<std::string>(ex.getExpected())
               : "<undefined>";
}

TEST(FunctionNameTest, registeredFunction) {
    JException ex = error("$test(1)");
    EXPECT_EQ(name(ex), "test");
    EXPECT_STREQ(ex.what(),
                 "Argument 1 of Object test does not match Object signature");
}

TEST(FunctionNameTest, registeredFunctionViaVariable) {
    JException ex = error("($f := $test; $f(1))");
    EXPECT_EQ(name(ex), "f");
    EXPECT_STREQ(ex.what(),
                 "Argument 1 of Object f does not match Object signature");
}

TEST(FunctionNameTest, builtinFunctionViaVariable) {
    EXPECT_EQ(name(error("($s := $substringBefore; $s(1, 2))")), "s");
    // also for the null checks done by the function itself
    EXPECT_EQ(name(error("($s := $substring; $s('abc', null))")), "s");
}

TEST(FunctionNameTest, lambdaViaVariable) {
    EXPECT_EQ(name(error("($f := function($x)<n:n>{ $x }; $f('a'))")), "f");
}

// T0412 errors of a function without a name are named at the call site too
TEST(FunctionNameTest, lambdaArrayTypeError) {
    Jsonata e("($f := function($x)<a<n>:n>{ $x }; $f(['a']))");
    try {
        e.evaluate(nullptr);
        FAIL() << "Expected JException";
    } catch (const JException& ex) {
        EXPECT_EQ(ex.getError(), "T0412");
        EXPECT_STREQ(ex.what(), "Argument 1 of Object f must be an array of n");
        EXPECT_EQ(std::any_cast<std::string>(ex.getExtra()), "n");
    }
    // a builtin keeps its own name
    try {
        Jsonata("($s := $sum; $s(['a']))").evaluate(nullptr);
        FAIL() << "Expected JException";
    } catch (const JException& ex) {
        EXPECT_STREQ(ex.what(),
                     "Argument 1 of Object sum must be an array of n");
    }
}

// An error raised while a higher-order function calls its function argument
// is named after the higher-order function, the innermost call site
TEST(FunctionNameTest, functionArgument) {
    EXPECT_EQ(name(error("$map(['a'], function($x)<n:n>{ $x })")), "map");
}

// An error from a nested call keeps the name of that call. (A call in tail
// position is run by the trampoline in apply(), so like in jsonata-js it is
// named after the outer call.)
TEST(FunctionNameTest, nestedCall) {
    EXPECT_EQ(name(error("($f := function($x){ [$test($x)] }; $f(1))")),
              "test");
    EXPECT_EQ(name(error("($f := function($x){ $test($x) }; $f(1))")), "f");
}

// A function that wasn't called by name has no name, like in jsonata-js
TEST(FunctionNameTest, notCalledByName) {
    JException ex = error("'a' ~> function($x)<n:n>{ $x }");
    EXPECT_FALSE(ex.getExpected().has_value());
    EXPECT_STREQ(
        ex.what(),
        "Argument 1 of Object undefined does not match Object signature");
}

}  // namespace
}  // namespace jsonata
