#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_countTest : public JsonataTest {
};

// $count(Account.Order.Product.(Price * Quantity))
TEST_F(function_countTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case000.json"));
}

// Account.Order.$count(Product.(Price * Quantity))
TEST_F(function_countTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case001.json"));
}

// Account.Order.(OrderID & \": \" & $count(Product.(Price*Quantity)))
TEST_F(function_countTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case002.json"));
}

// $count([])
TEST_F(function_countTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case003.json"));
}

// $count([1,2,3])
TEST_F(function_countTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case004.json"));
}

// $count([\"1\",\"2\",\"3\"])
TEST_F(function_countTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case005.json"));
}

// $count([\"1\",\"2\",3])
TEST_F(function_countTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case006.json"));
}

// $count(1)
TEST_F(function_countTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case007.json"));
}

// $count([],[])
TEST_F(function_countTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case008.json"));
}

// $count([1,2,3],[])
TEST_F(function_countTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case009.json"));
}

// $count([],[],[])
TEST_F(function_countTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case010.json"));
}

// $count([1,2],[],[])
TEST_F(function_countTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case011.json"));
}

// $count(undefined)
TEST_F(function_countTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case012.json"));
}

// $count([1,2,3,4]) / 2
TEST_F(function_countTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-count/case013.json"));
}

