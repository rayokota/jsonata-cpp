#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_roundTest : public JsonataTest {
};

// $round(4)
TEST_F(function_roundTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case000.json"));
}

// $round(2.3)
TEST_F(function_roundTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case001.json"));
}

// $round(2.7)
TEST_F(function_roundTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case002.json"));
}

// $round(2.5)
TEST_F(function_roundTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case003.json"));
}

// $round(3.5)
TEST_F(function_roundTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case004.json"));
}

// $round(-0.5)
TEST_F(function_roundTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case005.json"));
}

// $round(-0.3)
TEST_F(function_roundTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case006.json"));
}

// $round(0.5)
TEST_F(function_roundTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case007.json"));
}

// $round(-7.5)
TEST_F(function_roundTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case008.json"));
}

// $round(-8.5)
TEST_F(function_roundTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case009.json"));
}

// $round(4.49, 1)
TEST_F(function_roundTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case010.json"));
}

// $round(4.525, 2)
TEST_F(function_roundTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case011.json"));
}

// $round(4.515, 2)
TEST_F(function_roundTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case012.json"));
}

// $round(12345, -2)
TEST_F(function_roundTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case013.json"));
}

// $round(12450, -2)
TEST_F(function_roundTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case014.json"));
}

// $round(12350, -2)
TEST_F(function_roundTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case015.json"));
}

// $round(6.022e-23, 24)
TEST_F(function_roundTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case016.json"));
}

// $round(unknown)
TEST_F(function_roundTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-round/case017.json"));
}

