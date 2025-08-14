#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <vector>
#include <memory>

namespace jsonata {

class NullSafetyTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    static nlohmann::ordered_json makeArray(const std::vector<nlohmann::ordered_json>& values) {
        nlohmann::ordered_json arr = nlohmann::ordered_json::array();
        for (const auto& v : values) arr.push_back(v);
        return arr;
    }
};

TEST_F(NullSafetyTest, testNullSafety) {
    auto r = Jsonata("$sift(undefined, $uppercase)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$each(undefined, $uppercase)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$keys(null)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$map(null, $uppercase)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$filter(null, $uppercase)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$single(null, $uppercase)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$reduce(null, $uppercase)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$lookup(null, 'anykey')").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());

    r = Jsonata("$spread(null)").evaluate(nullptr);
    EXPECT_TRUE(r.is_null());
}

TEST_F(NullSafetyTest, testFilterNull) {
    auto arrayData = makeArray({1, nullptr});
    Jsonata expr("$filter($, function($v, $i, $a){$v})");
    auto result = expr.evaluate(arrayData);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int>(), 1);
}

TEST_F(NullSafetyTest, testNotNull) {
    auto result = Jsonata("$not($)").evaluate(nullptr);
    EXPECT_TRUE(result.is_null());
}

TEST_F(NullSafetyTest, testSingleNull) {
    auto arrayData = makeArray({nullptr, 1});
    Jsonata expr("$single($, function($v, $i, $a){ $v })");
    auto result = expr.evaluate(arrayData);
    ASSERT_TRUE(result.is_number());
    EXPECT_EQ(result.get<int>(), 1);
}

TEST_F(NullSafetyTest, testFilterNullLookup) {
    nlohmann::ordered_json arrayData = nlohmann::ordered_json::array({
        nlohmann::ordered_json::object({{"content", "some"}}),
        nlohmann::ordered_json::object()
    });
    Jsonata expr("$filter($, function($v, $i, $a){$lookup($v, 'content')})");
    auto result = expr.evaluate(arrayData);
    ASSERT_TRUE(result.is_object());
    ASSERT_TRUE(result.contains("content"));
    EXPECT_EQ(result["content"].get<std::string>(), "some");
}

} // namespace jsonata
