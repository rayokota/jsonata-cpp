#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_averageTest : public JsonataTest {
};

// $average(Account.Order.Product.(Price * Quantity))
TEST_F(function_averageTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case000.json"));
}

// Account.Order.$average(Product.(Price * Quantity))
TEST_F(function_averageTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case001.json"));
}

// Account.Order.(OrderID & \": \" & $average(Product.(Price*Quantity)))
TEST_F(function_averageTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case002.json"));
}

// $average([])
TEST_F(function_averageTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case003.json"));
}

// $average([1,2,3])
TEST_F(function_averageTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case004.json"));
}

// $average([\"1\",\"2\",\"3\"])
TEST_F(function_averageTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case005.json"));
}

// $average([\"1\",\"2\",3])
TEST_F(function_averageTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case006.json"));
}

// $average(1)
TEST_F(function_averageTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case007.json"));
}

// $average([],[])
TEST_F(function_averageTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case008.json"));
}

// $average([1,2,3],[])
TEST_F(function_averageTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case009.json"));
}

// $average([],[],[])
TEST_F(function_averageTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case010.json"));
}

// $average([1,2],[],[])
TEST_F(function_averageTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case011.json"));
}

// $average(undefined)
TEST_F(function_averageTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-average/case012.json"));
}

