#include <gtest/gtest.h>
#include <jsonata/JException.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <string>

// Error messages with three placeholders (T0412 and T2009) must have all of
// them filled in.
namespace jsonata {
namespace {

using nlohmann::ordered_json;

JException evalError(const std::string& expression,
                     const ordered_json& input = nullptr) {
    try {
        Jsonata(expression).evaluate(input);
    } catch (const JException& ex) {
        return ex;
    }
    ADD_FAILURE() << "Expected JException for " << expression;
    return JException("none");
}

TEST(ErrorMessageTest, arrayArgumentOfWrongType) {
    JException ex = evalError("$sum(x)", ordered_json::parse(R"({"x": ["y"]})"));
    EXPECT_EQ(ex.getError(), "T0412");
    EXPECT_STREQ(ex.what(),
                 "Argument 1 of Object sum must be an array of n");
}

TEST(ErrorMessageTest, arrayArgumentOfWrongItemType) {
    JException ex = evalError("$join(x, \",\")",
                              ordered_json::parse(R"({"x": [1, 2]})"));
    EXPECT_EQ(ex.getError(), "T0412");
    EXPECT_STREQ(ex.what(),
                 "Argument 1 of Object join must be an array of s");
}

TEST(ErrorMessageTest, arrayArgumentErrorDetails) {
    JException ex = evalError("$sum(x)", ordered_json::parse(R"({"x": ["y"]})"));
    EXPECT_EQ(std::any_cast<std::string>(ex.getExpected()), "sum");
    EXPECT_EQ(std::any_cast<std::string>(ex.getExtra()), "n");
    EXPECT_EQ(ex.getDetailedErrorMessage(),
              "Argument 1 of Object sum must be an array of n {code=T0412}");
}

TEST(ErrorMessageTest, valuesOfDifferentTypes) {
    JException ex = evalError(R"("foo" > 3)");
    EXPECT_EQ(ex.getError(), "T2009");
    EXPECT_STREQ(ex.what(),
                 "The values foo and 3 either side of operator > must be of "
                 "the same data type");
    EXPECT_EQ(std::any_cast<std::string>(ex.getExtra()), ">");
}

TEST(ErrorMessageTest, valuesOfDifferentTypesDetails) {
    JException ex = evalError("1 < \"a\"");
    EXPECT_EQ(ex.getDetailedErrorMessage(),
              "The values 1 and a either side of operator < must be of the "
              "same data type {code=T2009 position=0}");
}

TEST(ErrorMessageTest, twoPlaceholderMessagesAreUnchanged) {
    JException ex = evalError("$substring()");
    EXPECT_EQ(ex.getError(), "T0410");
    EXPECT_EQ(std::string(ex.what()),
              "Argument 1 of Object substring does not match Object signature");
}

}  // namespace
}  // namespace jsonata
