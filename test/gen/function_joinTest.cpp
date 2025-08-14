#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_joinTest : public JsonataTest {
};

// $join(\"hello\")
TEST_F(function_joinTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case000.json"));
}

// $join([\"hello\"])
TEST_F(function_joinTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case001.json"));
}

// $join([\"hello\", \"world\"])
TEST_F(function_joinTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case002.json"));
}

// $join([\"hello\", \"world\"], \", \")
TEST_F(function_joinTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case003.json"));
}

// $join([], \", \")
TEST_F(function_joinTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case004.json"));
}

// $join(Account.Order.Product.Description.Colour, \", \")
TEST_F(function_joinTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case005.json"));
}

// $join(Account.Order.Product.Description.Colour, no.sep)
TEST_F(function_joinTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case006.json"));
}

// $join(Account.blah.Product.Description.Colour, \", \")
TEST_F(function_joinTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case007.json"));
}

// $join(true, \", \")
TEST_F(function_joinTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case008.json"));
}

// $join([1,2,3], \", \")
TEST_F(function_joinTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case009.json"));
}

// $join([\"hello\"], 3)
TEST_F(function_joinTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case010.json"));
}

// $join()
TEST_F(function_joinTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-join/case011.json"));
}

