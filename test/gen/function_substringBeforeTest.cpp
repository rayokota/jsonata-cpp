#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_substringBeforeTest : public JsonataTest {
};

// $substringBefore(\"Hello World\", \" \")
TEST_F(function_substringBeforeTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringBefore/case000.json"));
}

// $substringBefore(\"Hello World\", \"l\")
TEST_F(function_substringBeforeTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringBefore/case001.json"));
}

// $substringBefore(\"Hello World\", \"f\")
TEST_F(function_substringBeforeTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringBefore/case002.json"));
}

// $substringBefore(\"Hello World\", \"He\")
TEST_F(function_substringBeforeTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringBefore/case003.json"));
}

// $substringBefore(blah, \"He\")
TEST_F(function_substringBeforeTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringBefore/case004.json"));
}

