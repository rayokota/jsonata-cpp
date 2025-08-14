#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_eachTest : public JsonataTest {
};

// $each(Address, λ($v, $k) {$k & \": \" & $v})
TEST_F(function_eachTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-each/case000.json"));
}

// $each({'a': 'hello', 'b': 'world'}, $uppercase)
TEST_F(function_eachTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-each/case001.json"));
}

// $each(function($v, $k) {$k[$v>2]})
TEST_F(function_eachTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-each/case002.json"));
}

