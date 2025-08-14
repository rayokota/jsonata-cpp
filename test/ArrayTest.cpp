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

} // namespace jsonata
