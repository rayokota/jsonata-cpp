#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_powerTest : public JsonataTest {
};

// $power(4, 2)
TEST_F(function_powerTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case000.json"));
}

// $power(4, 0.5)
TEST_F(function_powerTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case001.json"));
}

// $power(10, -2)
TEST_F(function_powerTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case002.json"));
}

// $power(-2, 3)
TEST_F(function_powerTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case003.json"));
}

// $power(nothing, 3)
TEST_F(function_powerTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case004.json"));
}

// $power(-2, 1/3)
TEST_F(function_powerTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case005.json"));
}

// $power(100, 1000)
TEST_F(function_powerTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-power/case006.json"));
}

