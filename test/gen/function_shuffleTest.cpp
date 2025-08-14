#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_shuffleTest : public JsonataTest {
};

// $count($shuffle([1..10]))
TEST_F(function_shuffleTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-shuffle/case000.json"));
}

// $sort($shuffle([1..10]))
TEST_F(function_shuffleTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-shuffle/case001.json"));
}

// $shuffle(nothing)
TEST_F(function_shuffleTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-shuffle/case002.json"));
}

// $shuffle([1])
TEST_F(function_shuffleTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-shuffle/case003.json"));
}

