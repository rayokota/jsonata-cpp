#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_appendTest : public JsonataTest {
};

// $append([1,2], [3,4])
TEST_F(function_appendTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-append/case000.json"));
}

// $append(1, [3,4])
TEST_F(function_appendTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-append/case001.json"));
}

// $append(1,2)
TEST_F(function_appendTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-append/case002.json"));
}

// $append(1,notexist)
TEST_F(function_appendTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-append/case003.json"));
}

// $append(notexist, [2,3,4])
TEST_F(function_appendTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-append/case004.json"));
}

// ($append($data.a, [2]); $data.a)
TEST_F(function_appendTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-append/case005.json"));
}

