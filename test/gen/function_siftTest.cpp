#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_siftTest : public JsonataTest {
};

// $sift(λ($v){$v.**.Postcode})
TEST_F(function_siftTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sift/case000.json"));
}

// **[*].$sift(λ($v){$v.Postcode})
TEST_F(function_siftTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sift/case001.json"));
}

// $sift(λ($v, $k){$k ~> /^A/})
TEST_F(function_siftTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sift/case002.json"));
}

// $sift({'a': 1, 'b': 0}, $boolean)
TEST_F(function_siftTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sift/case003.json"));
}

// $sift({'a': 'hello', 'b': 'world', 'hello': 'again'}, λ($v,$k,$o){$lookup($o, $v)})
TEST_F(function_siftTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sift/case004.json"));
}

