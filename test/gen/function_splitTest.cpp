#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_splitTest : public JsonataTest {
};

// $split(\"Hello World\", \" \")
TEST_F(function_splitTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case000.json"));
}

// $split(\"Hello\", \" \")
TEST_F(function_splitTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case001.json"));
}

// $split(\"Hello  World\", \" \")
TEST_F(function_splitTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case002.json"));
}

// $split(\"Hello\", \"\")
TEST_F(function_splitTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case003.json"));
}

// $sum($split(\"12345\", \"\").$number($))
TEST_F(function_splitTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case004.json"));
}

// $split(\"a, b, c, d\", \", \")
TEST_F(function_splitTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case005.json"));
}

// $split(\"a, b, c, d\", \", \", 2)
TEST_F(function_splitTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case006.json"));
}

// $split(\"a, b, c, d\", \", \", 2.5)
TEST_F(function_splitTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case007.json"));
}

// $split(\"a, b, c, d\", \", \", 10)
TEST_F(function_splitTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case008.json"));
}

// $split(\"a, b, c, d\", \", \", 0)
TEST_F(function_splitTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case009.json"));
}

// $split(nothing, \" \")
TEST_F(function_splitTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case010.json"));
}

// $split(\"a, b, c, d\", \", \", -3)
TEST_F(function_splitTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case011.json"));
}

// $split(\"a, b, c, d\", \", \", null)
TEST_F(function_splitTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case012.json"));
}

// $split(\"a, b, c, d\", \", \", -5)
TEST_F(function_splitTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case013.json"));
}

// $split(\"a, b, c, d\", \", \", \"2\")
TEST_F(function_splitTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case014.json"));
}

// $split(\"a, b, c, d\", true)
TEST_F(function_splitTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case015.json"));
}

// $split(12345, 3)
TEST_F(function_splitTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case016.json"));
}

// $split(12345)
TEST_F(function_splitTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case017.json"));
}

// $split('12💩345💩6', '💩')
TEST_F(function_splitTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-split/case018.json"));
}

