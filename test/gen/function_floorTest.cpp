#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_floorTest : public JsonataTest {
};

// $floor(3.7)
TEST_F(function_floorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-floor/case000.json"));
}

// $floor(-3.7)
TEST_F(function_floorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-floor/case001.json"));
}

// $floor(0)
TEST_F(function_floorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-floor/case002.json"));
}

// $floor(nothing)
TEST_F(function_floorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-floor/case003.json"));
}

