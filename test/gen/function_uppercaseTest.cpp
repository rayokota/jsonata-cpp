#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_uppercaseTest : public JsonataTest {
};

// $uppercase(\"Hello World\")
TEST_F(function_uppercaseTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-uppercase/case000.json"));
}

// $uppercase(blah)
TEST_F(function_uppercaseTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-uppercase/case001.json"));
}

