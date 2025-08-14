#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <jsonata/Functions.h>
#include <vector>
#include <memory>
#include <functional>

namespace jsonata {

class CustomFunctionTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CustomFunctionTest, testSupplier) {
    Jsonata expression("$greet()");
    // Register zero-arg function via typed overload
    expression.registerFunction<std::string>("greet", []() { return std::string("Hello world"); });
    auto result = expression.evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "Hello world");
}

TEST_F(CustomFunctionTest, testEval) {
    Jsonata expression("$eval('$greet()')");
    expression.registerFunction<std::string>("greet", []() { return std::string("Hello world"); });
    auto result = expression.evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "Hello world");
}

TEST_F(CustomFunctionTest, testEvalWithParams) {
    Jsonata expression("($eval('$greet()'))");
    expression.registerFunction<std::string>("greet", []() { return std::string("Hello world"); });
    auto result = expression.evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "Hello world");
}

TEST_F(CustomFunctionTest, testUnary) {
    Jsonata expression("$echo(123)");
    expression.registerFunction<int64_t,int64_t>("echo", [](int64_t x) { return x; });
    auto result = expression.evaluate(nullptr);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int>(), 123);
}

TEST_F(CustomFunctionTest, testBinary) {
    Jsonata expression("$add(21, 21)");
    expression.registerFunction<int64_t,int64_t,int64_t>("add", [](int64_t a, int64_t b) { return a + b; });
    auto result = expression.evaluate(nullptr);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int>(), 42);
}

TEST_F(CustomFunctionTest, testTernary) {
    Jsonata expression("$abc(a,b,c)");
    JFunction abc;
    abc.signature = std::make_shared<utils::Signature>("<sss:s>", "abc");
    abc.implementation = [](const Utils::JList& args, const std::any&, std::shared_ptr<Frame>) -> std::any {
        std::string a = std::any_cast<std::string>(args[0]);
        std::string b = std::any_cast<std::string>(args[1]);
        std::string c = std::any_cast<std::string>(args[2]);
        return a + b + c;
    };
    expression.registerFunction("abc", abc);

    nlohmann::ordered_json data = {{"a","a"},{"b","b"},{"c","c"}};
    auto result = expression.evaluate(data);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "abc");
}

TEST_F(CustomFunctionTest, testLambdaSignatureError) {
    Jsonata expression("$append(1, 2)");
    // Register JFunction without signature; inside will cast wrongly to force error
    JFunction append;
    append.implementation = [](const Utils::JList& args, const std::any&, std::shared_ptr<Frame>) -> std::any {
        int a = 0; bool b = false;
        if (!args.empty()) {
            if (args[0].type() == typeid(double)) a = static_cast<int>(std::any_cast<double>(args[0]));
            else if (args[0].type() == typeid(long long)) a = static_cast<int>(std::any_cast<long long>(args[0]));
            else if (args[0].type() == typeid(int)) a = std::any_cast<int>(args[0]);
        }
        // Force bad cast for the second arg
        try { b = std::any_cast<bool>(args[1]); }
        catch (const std::bad_any_cast&) { throw std::runtime_error("ClassCastException equivalent"); }
        return std::to_string(a) + (b ? "true" : "false");
    };
    expression.registerFunction("append", append);
    EXPECT_THROW(expression.evaluate(nullptr), std::runtime_error);
}

TEST_F(CustomFunctionTest, testJFunctionSignatureError) {
    Jsonata expression("$append(1, 2)");
    JFunction append;
    append.signature = std::make_shared<utils::Signature>("<nb:s>", "append");
    append.implementation = [](const Utils::JList& args, const std::any&, std::shared_ptr<Frame>) -> std::any {
        return std::string();
    };
    expression.registerFunction("append", append);

    try {
        expression.evaluate(nullptr);
        FAIL() << "Expected JException";
    } catch (const JException& ex) {
        EXPECT_EQ(ex.getError(), "T0410");
        // expected holds function name as std::any
        auto expectedAny = ex.getExpected();
        std::string expectedStr;
        try { expectedStr = std::any_cast<std::string>(expectedAny); } catch (...) { expectedStr = ""; }
        EXPECT_EQ(expectedStr, std::string("append"));
    }
}

} // namespace jsonata
