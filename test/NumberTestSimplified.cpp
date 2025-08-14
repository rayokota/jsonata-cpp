#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/Utils.h>
#include <jsonata/JException.h>

namespace jsonata {

class NumberTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(NumberTest, DISABLED_testDouble) {
    Jsonata expr1("x");
    auto data = nlohmann::ordered_json::parse(R"({"x": 1.0})");
    auto res = expr1.evaluate(data);
    ASSERT_TRUE(!res.is_null());
    ASSERT_TRUE(res.is_number());
    EXPECT_EQ(static_cast<int>(res.get<double>()), 1);
}

TEST_F(NumberTest, testDouble2) {
    Jsonata expr1("x+0");
    auto data = nlohmann::ordered_json::parse(R"({"x": 1.0})");
    auto res = expr1.evaluate(data);
    ASSERT_TRUE(res.is_number());
    EXPECT_EQ(static_cast<int>(res.get<double>()), 1);
}

TEST_F(NumberTest, testDouble3) {
    Jsonata expr1("x");
    auto data = nlohmann::ordered_json::parse(R"({"x":1.0})");
    auto res = expr1.evaluate(data);
    ASSERT_TRUE(res.is_number());
    EXPECT_EQ(static_cast<int>(res.get<double>()), 1);
}

TEST_F(NumberTest, testInt) {
    Jsonata expr1("$ / 2");
    auto data = nlohmann::ordered_json::parse("1");
    auto res = expr1.evaluate(data);
    ASSERT_TRUE(res.is_number());
    EXPECT_DOUBLE_EQ(res.get<double>(), 0.5);
}

TEST_F(NumberTest, testConst) {
    Jsonata expr1("1.0");
    auto res = expr1.evaluate(nullptr);
    ASSERT_TRUE(res.is_number());
    EXPECT_EQ(static_cast<int>(res.get<double>()), 1);
}

} // namespace jsonata
