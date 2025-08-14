#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_padTest : public JsonataTest {
};

// $pad(\"foo\", 5)
TEST_F(function_padTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case000.json"));
}

// $pad(\"foo\", -5)
TEST_F(function_padTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case001.json"));
}

// $pad(\"foo\", -5, \"#\")
TEST_F(function_padTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case002.json"));
}

// $pad(\"foo\", 5, \"\")
TEST_F(function_padTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case003.json"));
}

// $pad(\"foo\", 1)
TEST_F(function_padTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case004.json"));
}

// $pad(\"foo\", 8, \"-+\")
TEST_F(function_padTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case005.json"));
}

// $pad(nothing, 1)
TEST_F(function_padTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case006.json"));
}

// $pad('a𝄞💩', 7)
TEST_F(function_padTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case007.json"));
}

// $pad('𝄞💩', -7)
TEST_F(function_padTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case008.json"));
}

// $pad('a𝄞💩', 7, '💩')
TEST_F(function_padTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case009.json"));
}

// $pad('a𝄞💩', 6, '💩')
TEST_F(function_padTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case010.json"));
}

// $pad('foo', 5.7, ' ')
TEST_F(function_padTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case011.json"));
}

// $pad('foo', -5.7, ' ')
TEST_F(function_padTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-pad/case012.json"));
}

