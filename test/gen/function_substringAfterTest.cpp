#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_substringAfterTest : public JsonataTest {
};

// $substringAfter(\"Hello World\", \" \")
TEST_F(function_substringAfterTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringAfter/case000.json"));
}

// $substringAfter(\"Hello World\", \"l\")
TEST_F(function_substringAfterTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringAfter/case001.json"));
}

// $substringAfter(\"Hello World\", \"f\")
TEST_F(function_substringAfterTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringAfter/case002.json"));
}

// $substringAfter(\"Hello World\", \"ld\")
TEST_F(function_substringAfterTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringAfter/case003.json"));
}

// $substringAfter(blah, \"ld\")
TEST_F(function_substringAfterTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-substringAfter/case004.json"));
}

