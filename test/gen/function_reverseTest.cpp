#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_reverseTest : public JsonataTest {
};

// $reverse([1..5])
TEST_F(function_reverseTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-reverse/case000.json"));
}

// [[$], [$reverse($)], [$]]
TEST_F(function_reverseTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-reverse/case001.json"));
}

// $reverse(nothing)
TEST_F(function_reverseTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-reverse/case002.json"));
}

// $reverse([1])
TEST_F(function_reverseTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-reverse/case003.json"));
}

