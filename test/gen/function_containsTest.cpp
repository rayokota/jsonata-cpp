#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_containsTest : public JsonataTest {
};

// $contains(\"Hello World\", \"lo\")
TEST_F(function_containsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case000.json"));
}

// $contains(\"Hello World\", \"World\")
TEST_F(function_containsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case001.json"));
}

// $contains(\"Hello World\", \"world\")
TEST_F(function_containsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case002.json"));
}

// $contains(\"Hello World\", \"Word\")
TEST_F(function_containsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case003.json"));
}

// $contains(nothing, \"World\")
TEST_F(function_containsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case004.json"));
}

// $contains(23, 3)
TEST_F(function_containsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case005.json"));
}

// $contains(\"23\", 3)
TEST_F(function_containsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-contains/case006.json"));
}

