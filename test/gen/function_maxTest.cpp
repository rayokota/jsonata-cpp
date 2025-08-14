#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_maxTest : public JsonataTest {
};

// $max(Account.Order.Product.(Price * Quantity))
TEST_F(function_maxTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case000.json"));
}

// Account.Order.$max(Product.(Price * Quantity))
TEST_F(function_maxTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case001.json"));
}

// Account.Order.(OrderID & \": \" & $count(Product.(Price*Quantity)))
TEST_F(function_maxTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case002.json"));
}

// $max([])
TEST_F(function_maxTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case003.json"));
}

// $max([1,2,3])
TEST_F(function_maxTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case004.json"));
}

// $max([\"1\",\"2\",\"3\"])
TEST_F(function_maxTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case005.json"));
}

// $max([\"1\",\"2\",3])
TEST_F(function_maxTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case006.json"));
}

// $max(1)
TEST_F(function_maxTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case007.json"));
}

// $max([-1,-5])
TEST_F(function_maxTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case008.json"));
}

// $max([],[])
TEST_F(function_maxTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case009.json"));
}

// $max([1,2,3],[])
TEST_F(function_maxTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case010.json"));
}

// $max([],[],[])
TEST_F(function_maxTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case011.json"));
}

// $max([1,2],[],[])
TEST_F(function_maxTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case012.json"));
}

// $max(undefined)
TEST_F(function_maxTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case013.json"));
}

// $min(Account.Order.Product.(Price * Quantity))
TEST_F(function_maxTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case014.json"));
}

// Account.Order.$min(Product.(Price * Quantity))
TEST_F(function_maxTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case015.json"));
}

// Account.Order.(OrderID & \": \" & $min(Product.(Price*Quantity)))
TEST_F(function_maxTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case016.json"));
}

// $min([])
TEST_F(function_maxTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case017.json"));
}

// $min([1,2,3])
TEST_F(function_maxTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case018.json"));
}

// $min([\"1\",\"2\",\"3\"])
TEST_F(function_maxTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case019.json"));
}

// $min([\"1\",\"2\",3])
TEST_F(function_maxTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case020.json"));
}

// $min(1)
TEST_F(function_maxTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case021.json"));
}

// $min([],[])
TEST_F(function_maxTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case022.json"));
}

// $min([1,2,3],[])
TEST_F(function_maxTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case023.json"));
}

// $min([],[],[])
TEST_F(function_maxTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case024.json"));
}

// $min([1,2],[],[])
TEST_F(function_maxTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case025.json"));
}

// $min(undefined)
TEST_F(function_maxTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-max/case026.json"));
}

