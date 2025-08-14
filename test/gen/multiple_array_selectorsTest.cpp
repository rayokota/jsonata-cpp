#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class multiple_array_selectorsTest : public JsonataTest {
};

// [1..10][[1..3,8,-1]]
TEST_F(multiple_array_selectorsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/multiple-array-selectors/case000.json"));
}

// [1..10][[1..3,8,5]]
TEST_F(multiple_array_selectorsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/multiple-array-selectors/case001.json"));
}

// [1..10][[1..3,8,false]]
TEST_F(multiple_array_selectorsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/multiple-array-selectors/case002.json"));
}

