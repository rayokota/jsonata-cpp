#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_absTest : public JsonataTest {
};

// $abs(3.7)
TEST_F(function_absTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-abs/case000.json"));
}

// $abs(-3.7)
TEST_F(function_absTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-abs/case001.json"));
}

// $abs(0)
TEST_F(function_absTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-abs/case002.json"));
}

// $abs(nothing)
TEST_F(function_absTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-abs/case003.json"));
}

