#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_formatBaseTest : public JsonataTest {
};

// $formatBase(100)
TEST_F(function_formatBaseTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case000.json"));
}

// $formatBase(nothing)
TEST_F(function_formatBaseTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case001.json"));
}

// $formatBase(100, 2)
TEST_F(function_formatBaseTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case002.json"));
}

// $formatBase(-100, 2)
TEST_F(function_formatBaseTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case003.json"));
}

// $formatBase(100, 36)
TEST_F(function_formatBaseTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case004.json"));
}

// $formatBase(99.5, 2.5)
TEST_F(function_formatBaseTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case005.json"));
}

// $formatBase(100, 1)
TEST_F(function_formatBaseTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case006.json"));
}

// $formatBase(100, 37)
TEST_F(function_formatBaseTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatBase/case007.json"));
}

