#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>

namespace jsonata {

class ArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(ArrayTest, testArray) {
    // Create test data equivalent to Java: Map.of("key", Arrays.asList(Map.of("x", "y"), Map.of("a", "b")))
    auto data = nlohmann::ordered_json::parse(R"({"key": [{"x": "y"}, {"a": "b"}]})");

    // Test first expression: {'key': $append($.[{'x': 'y'}],$.[{'a': 'b'}])}
    Jsonata expr1("{'key': $append($.[{'x': 'y'}],$.[{'a': 'b'}])}");
    auto res1 = expr1.evaluate(data);

    // Test second expression: {'key': $append($.[{'x': 'y'}],[{'a': 'b'}])}
    Jsonata expr2("{'key': $append($.[{'x': 'y'}],[{'a': 'b'}])}");
    auto res2 = expr2.evaluate(data);

    // Compare results - they should be equal
    EXPECT_EQ(res1.dump(), res2.dump());
}

TEST_F(ArrayTest, DISABLED_filterTest) {
    // Frame value not evaluated if used in array filter #45
    // This test is disabled as in the Java version
    Jsonata expr("($arr := [{'x':1}, {'x':2}];$arr[x=$number(variable.field)])");

    auto inputData = nlohmann::ordered_json::parse(R"({"variable": {"field": "1"}})");

    auto result = expr.evaluate(inputData);
    EXPECT_TRUE(result != nullptr);
}

TEST_F(ArrayTest, testWildcard) {
    Jsonata expr("*");
    auto input = nlohmann::ordered_json::parse(R"([{"x": 1}])");
    auto result = expr.evaluate(input);
    auto expected = nlohmann::ordered_json::parse(R"({"x": 1})");
    EXPECT_EQ(result.dump(), expected.dump());
}

TEST_F(ArrayTest, testWildcardFilter) {
    auto data = nlohmann::ordered_json::parse(
        R"([{"value": {"Name": "Cell1", "Product": "Product1"}}, {"value": {"Name": "Cell2", "Product": "Product2"}}])");

    Jsonata expression("*[value.Product = 'Product1']");
    auto result1 = expression.evaluate(data);
    auto expected = nlohmann::ordered_json::parse(
        R"({"value": {"Name": "Cell1", "Product": "Product1"}})");
    EXPECT_EQ(result1.dump(), expected.dump());

    Jsonata expression2("**[value.Product = 'Product1']");
    auto result2 = expression2.evaluate(data);
    EXPECT_EQ(result2.dump(), expected.dump());
}

} // namespace jsonata
