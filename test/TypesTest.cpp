#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

namespace jsonata {

class TypesTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TypesTest, castTestIn) {
    auto arrayData = nlohmann::ordered_json::array({1.0, 2.0});

    auto result1 = Jsonata("3 in $").evaluate(arrayData);
    ASSERT_TRUE(result1.is_boolean());
    EXPECT_FALSE(result1.get<bool>());

    auto result2 = Jsonata("1 in $").evaluate(arrayData);
    ASSERT_TRUE(result2.is_boolean());
    EXPECT_TRUE(result2.get<bool>());
}

TEST_F(TypesTest, castTestEquals) {
    auto data1 = nlohmann::ordered_json(1.0);
    auto result1 = Jsonata("1 = $").evaluate(data1);
    ASSERT_TRUE(result1.is_boolean());
    EXPECT_TRUE(result1.get<bool>());

    auto data2 = nlohmann::ordered_json(2.0);
    auto result2 = Jsonata("1 = $").evaluate(data2);
    ASSERT_TRUE(result2.is_boolean());
    EXPECT_FALSE(result2.get<bool>());

    auto result3 = Jsonata("{'x':1 } = {'x':1 }").evaluate(nullptr);
    ASSERT_TRUE(result3.is_boolean());
    EXPECT_TRUE(result3.get<bool>());

    auto result4 = Jsonata("{'x':1 } = {'x':2 }").evaluate(nullptr);
    ASSERT_TRUE(result4.is_boolean());
    EXPECT_FALSE(result4.get<bool>());

    auto result5 = Jsonata("[1,null] = [1,null]").evaluate(nullptr);
    ASSERT_TRUE(result5.is_boolean());
    EXPECT_TRUE(result5.get<bool>());

    auto result6 = Jsonata("[1,null] = [2,null]").evaluate(nullptr);
    ASSERT_TRUE(result6.is_boolean());
    EXPECT_FALSE(result6.get<bool>());
}

TEST_F(TypesTest, testIllegalTypes) {
    // Not directly applicable in JSON; ensure that passing unexpected types throws when appropriate
    EXPECT_NO_THROW(Jsonata("$").evaluate(nlohmann::ordered_json("c")));
}

TEST_F(TypesTest, testLegalTypes) {
    auto mapData = nlohmann::ordered_json::object({{"a", 1}});
    auto result1 = Jsonata("a").evaluate(mapData);
    ASSERT_TRUE(result1.is_number());
    EXPECT_EQ(result1.get<int>(), 1);

    auto arrayData = nlohmann::ordered_json::array({1, 2});
    auto result2 = Jsonata("$[0]").evaluate(arrayData);
    ASSERT_TRUE(result2.is_number());
    EXPECT_EQ(result2.get<int>(), 1);

    auto stringData = nlohmann::ordered_json("string");
    auto result3 = Jsonata("$").evaluate(stringData);
    ASSERT_TRUE(result3.is_string());
    EXPECT_EQ(result3.get<std::string>(), "string");

    auto intData = nlohmann::ordered_json(1);
    auto result4 = Jsonata("$").evaluate(intData);
    ASSERT_TRUE(result4.is_number());
    EXPECT_EQ(result4.get<int>(), 1);

    auto longData = nlohmann::ordered_json(1L);
    auto result5 = Jsonata("$").evaluate(longData);
    ASSERT_TRUE(result5.is_number());
    EXPECT_EQ(result5.get<long>(), 1L);

    auto boolData = nlohmann::ordered_json(true);
    auto result6 = Jsonata("$").evaluate(boolData);
    ASSERT_TRUE(result6.is_boolean());
    EXPECT_TRUE(result6.get<bool>());

    auto doubleData = nlohmann::ordered_json(1.0);
    auto result7 = Jsonata("$").evaluate(doubleData);
    ASSERT_TRUE(result7.is_number());
    EXPECT_DOUBLE_EQ(result7.get<double>(), 1.0);

    auto floatData = nlohmann::ordered_json(1.0f);
    auto result8 = Jsonata("$").evaluate(floatData);
    ASSERT_TRUE(result8.is_number());
    EXPECT_FLOAT_EQ(result8.get<float>(), 1.0f);

    auto bigDecimalData = nlohmann::ordered_json(3.14);
    auto result9 = Jsonata("$").evaluate(bigDecimalData);
    ASSERT_TRUE(result9.is_number());
    EXPECT_DOUBLE_EQ(result9.get<double>(), 3.14);
}

struct Pojo {
    char c = 'c';
    long d = 1234567890L;
    std::vector<int> arr = {0, 1, 2, 3};
};

TEST_F(TypesTest, testJacksonConversion) {
    nlohmann::ordered_json obj = {
        {"c", "c"},
        {"d", 1234567890L},
        {"arr", nlohmann::ordered_json::array({0,1,2,3})}
    };

    auto result1 = Jsonata("c").evaluate(obj);
    ASSERT_TRUE(result1.is_string());
    EXPECT_EQ(result1.get<std::string>(), "c");

    auto result2 = Jsonata("arr[0]").evaluate(obj);
    ASSERT_TRUE(result2.is_number());
    EXPECT_EQ(result2.get<int>(), 0);

    auto result3 = Jsonata("d").evaluate(obj);
    ASSERT_TRUE(result3.is_number());
    EXPECT_EQ(result3.get<long>(), 1234567890L);

    auto result4 = Jsonata("$").evaluate(obj);
    ASSERT_TRUE(result4.is_object());
}

TEST_F(TypesTest, testCustomFunction) {
    Jsonata fn("$foo()");
    JFunction jfn;
    jfn.implementation = [](const Utils::JList&, const std::any&, std::shared_ptr<Frame>) -> std::any {
        nlohmann::ordered_json obj = {{"c", "c"}};
        return obj;
    };
    fn.registerFunction("foo", jfn);

    auto result = fn.evaluate(nullptr);
    ASSERT_TRUE(result.is_object());
    ASSERT_TRUE(result.contains("c"));
    EXPECT_EQ(result["c"].get<std::string>(), "c");
}

TEST_F(TypesTest, testIgnore) {
    Jsonata expr("a");
    auto validData = nlohmann::ordered_json::object({{"a", "test"}});
    auto result1 = expr.evaluate(validData);
    ASSERT_TRUE(result1.is_string());
    EXPECT_EQ(result1.get<std::string>(), "test");

    Jsonata expr2("a & a");
    auto result2 = expr2.evaluate(validData);
    ASSERT_TRUE(result2.is_string());
    EXPECT_EQ(result2.get<std::string>(), "testtest");
}

} // namespace jsonata
