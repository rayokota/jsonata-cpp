#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_ceilTest : public JsonataTest {
};

// $ceil(3.7)
TEST_F(function_ceilTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-ceil/case000.json"));
}

// $ceil(-3.7)
TEST_F(function_ceilTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-ceil/case001.json"));
}

// $ceil(0)
TEST_F(function_ceilTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-ceil/case002.json"));
}

// $ceil(nothing)
TEST_F(function_ceilTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-ceil/case003.json"));
}

