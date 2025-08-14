#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_zipTest : public JsonataTest {
};

// $zip([1,2,3],[4,5,6])
TEST_F(function_zipTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-zip/case000.json"));
}

// $zip([1,2,3],[4,5,6],[7,8,9])
TEST_F(function_zipTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-zip/case001.json"));
}

// $zip([1,2,3],[4,5],[7,8,9])
TEST_F(function_zipTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-zip/case002.json"));
}

// $zip([1,2,3])
TEST_F(function_zipTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-zip/case003.json"));
}

// $zip(1,2,3)
TEST_F(function_zipTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-zip/case004.json"));
}

// $zip([1,2,3], [4,5,6], nothing)
TEST_F(function_zipTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-zip/case005.json"));
}

