#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_spreadTest : public JsonataTest {
};

// $spread(\"Hello World\")
TEST_F(function_spreadTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-spread/case000.json"));
}

// $spread((Account.Order.Product.Description))
TEST_F(function_spreadTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-spread/case001.json"));
}

// $spread(blah)
TEST_F(function_spreadTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-spread/case002.json"));
}

// $string($spread(function($x){$x*$x}))
TEST_F(function_spreadTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-spread/case003.json"));
}

