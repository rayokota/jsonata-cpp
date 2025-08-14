#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_sortTest : public JsonataTest {
};

// $sort(nothing)
TEST_F(function_sortTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case000.json"));
}

// $sort(1)
TEST_F(function_sortTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case001.json"));
}

// $sort([1,3,2])
TEST_F(function_sortTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case002.json"));
}

// $sort([1,3,22,11])
TEST_F(function_sortTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case003.json"));
}

// [[$], [$sort($)], [$]]
TEST_F(function_sortTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case004.json"));
}

// $sort(Account.Order.Product.Price)
TEST_F(function_sortTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case005.json"));
}

// $sort(Account.Order.Product.\"Product Name\")
TEST_F(function_sortTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case006.json"));
}

// $sort(Account.Order.Product)
TEST_F(function_sortTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case007.json"));
}

// $sort(Account.Order.Product, function($a, $b) { $a.(Price * Quantity) > $b.(Price * Quantity) }).(Price & \" x \" & Quantity)
TEST_F(function_sortTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case008.json"));
}

// $sort(Account.Order.Product, function($a, $b) { $a.Price > $b.Price }).SKU
TEST_F(function_sortTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case009.json"));
}

//                  (Account.Order.Product                   ~> $sort(λ($a,$b){$a.Quantity < $b.Quantity})                   ~> $sort(λ($a,$b){$a.Price > $b.Price})                 ).SKU             
TEST_F(function_sortTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-sort/case010.json"));
}

