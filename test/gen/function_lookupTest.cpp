#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_lookupTest : public JsonataTest {
};

// $lookup(Account, \"Account Name\")
TEST_F(function_lookupTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-lookup/case000.json"));
}

// $lookup(Account.Order.Product, \"Product Name\")
TEST_F(function_lookupTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-lookup/case001.json"));
}

// $lookup(Account.Order.Product.ProductID, \"Product Name\")
TEST_F(function_lookupTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-lookup/case002.json"));
}

// $lookup($$, \"gust\")
TEST_F(function_lookupTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-lookup/case003.json"));
}

