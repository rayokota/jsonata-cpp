#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_lengthTest : public JsonataTest {
};

// $length(\"\")
TEST_F(function_lengthTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case000.json"));
}

// $length(\"hello\")
TEST_F(function_lengthTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case001.json"));
}

// $length(missing)
TEST_F(function_lengthTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case002.json"));
}

// $length(\"\\u03BB-calculus\")
TEST_F(function_lengthTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case003.json"));
}

// $length(\"\\uD834\\uDD1E\")
TEST_F(function_lengthTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case004.json"));
}

// $length(\"𝄞\")
TEST_F(function_lengthTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case005.json"));
}

// $length(\"超明體繁\")
TEST_F(function_lengthTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case006.json"));
}

// $length(\"\\t\")
TEST_F(function_lengthTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case007.json"));
}

// $length(\"\\n\")
TEST_F(function_lengthTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case008.json"));
}

// $length(1234)
TEST_F(function_lengthTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case009.json"));
}

// $length(null)
TEST_F(function_lengthTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case010.json"));
}

// $length(true)
TEST_F(function_lengthTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case011.json"));
}

// $length([\"str\"])
TEST_F(function_lengthTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case012.json"));
}

// $length()
TEST_F(function_lengthTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case013.json"));
}

// $length()
TEST_F(function_lengthTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case014.json"));
}

// $length(\"Hello\", \"World\")
TEST_F(function_lengthTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case015.json"));
}

// $length(\"𝄞💩\")
TEST_F(function_lengthTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-length/case016.json"));
}

