#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_distinctTest : public JsonataTest {
};

// $distinct([1,2,3,4])
TEST_F(function_distinctTest, distinct_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 0));
}

// $distinct([1])
TEST_F(function_distinctTest, distinct_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 1));
}

// $distinct(nothing)
TEST_F(function_distinctTest, distinct_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 2));
}

// $distinct(null)
TEST_F(function_distinctTest, distinct_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 3));
}

// $distinct('hello')
TEST_F(function_distinctTest, distinct_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 4));
}

// $distinct(Account.Order.Product.SKU)
TEST_F(function_distinctTest, distinct_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 5));
}

// $distinct(Account.Order.Product.`Product Name`)
TEST_F(function_distinctTest, distinct_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 6));
}

// $distinct(Account.Order.Product.Description)
TEST_F(function_distinctTest, distinct_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-distinct/distinct.json", 7));
}

