#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_lowercaseTest : public JsonataTest {
};

// $lowercase(\"Hello World\")
TEST_F(function_lowercaseTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-lowercase/case000.json"));
}

// $lowercase(blah)
TEST_F(function_lowercaseTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-lowercase/case001.json"));
}

