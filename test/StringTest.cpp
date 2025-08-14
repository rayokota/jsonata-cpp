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

TEST_F(StringTest, regexTest) {
    auto input = makeObject({{"foo", 1}, {"bar", 2}});
    auto result = Jsonata("($matcher := $eval('/^' & 'foo' & '/i'); $.$spread()[$.$keys() ~> $matcher])").evaluate(input);
    ASSERT_TRUE(result.is_object());
    auto expected = makeObject({{"foo", 1}});
    EXPECT_EQ(result, expected);
}

TEST_F(StringTest, DISABLED_replaceTest) {
    auto input = nlohmann::ordered_json("http://example.org/test{par}");
    auto result = Jsonata("$replace($, /{par}/, '')").evaluate(input);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "http://example.org/test");
}

} // namespace jsonata
