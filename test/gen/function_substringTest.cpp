#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_substringTest : public JsonataTest {
};

// $substring(\"hello world\", 0, 5)
TEST_F(function_substringTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case000.json"));
}

// $substring(\"hello world\", -5, 5)
TEST_F(function_substringTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case001.json"));
}

// $substring(\"hello world\", 6)
TEST_F(function_substringTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case002.json"));
}

// $substring(blah, 6)
TEST_F(function_substringTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case003.json"));
}

// $substring('𝄞💩', 0, 2)
TEST_F(function_substringTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case004.json"));
}

// $substring('𝄞💩', 0, 1)
TEST_F(function_substringTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case005.json"));
}

// $substring('𝄞💩', 1, 1)
TEST_F(function_substringTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case006.json"));
}

// $substring(\"hello world\", -5)
TEST_F(function_substringTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case007.json"));
}

// $substring(\"hello world\", -5, 5)
TEST_F(function_substringTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case008.json"));
}

// $substring(\"hello world\", -5, 4)
TEST_F(function_substringTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case009.json"));
}

// $substring(\"hello world\", -5, 1)
TEST_F(function_substringTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case010.json"));
}

// $substring(\"hello world\", -5, 0)
TEST_F(function_substringTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case011.json"));
}

// $substring(\"hello world\", -5, -1)
TEST_F(function_substringTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case012.json"));
}

// $substring(\"hello world\", 0, -6)
TEST_F(function_substringTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case013.json"));
}

// $substring(\"hello world\", -100, 3)
TEST_F(function_substringTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case014.json"));
}

// $substring(\"😂😁😀\", -2, 1)
TEST_F(function_substringTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case015.json"));
}

// $substring(\"😂😁😀\", -3, 1)
TEST_F(function_substringTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case016.json"));
}

// $substring(\"😂😁😀\", -4, 1)
TEST_F(function_substringTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case017.json"));
}

// $substring(\"😂😁😀\", -5, 1)
TEST_F(function_substringTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substring/case018.json"));
}

