#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class sortingTest : public JsonataTest {
};

// Account.Order.Product.Price^($)
TEST_F(sortingTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case000.json"));
}

// Account.Order.Product.Price^(<$)
TEST_F(sortingTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case001.json"));
}

// Account.Order.Product.Price^(>$)
TEST_F(sortingTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case002.json"));
}

// Account.Order.Product^(Price).Description.Colour
TEST_F(sortingTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case003.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case004.json"));
}

// Account.Order.Product^(Price * Quantity).Description.Colour
TEST_F(sortingTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case005.json"));
}

// Account.Order.Product^(Quantity, Description.Colour).Description.Colour
TEST_F(sortingTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case006.json"));
}

// Account.Order.Product^(Quantity, >Description.Colour).Description.Colour
TEST_F(sortingTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case007.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case008.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case009.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case010.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case011.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case012.json"));
}

// Account.Order.Product^(Price).SKU
TEST_F(sortingTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case013.json"));
}

// $^(age).name
TEST_F(sortingTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case014.json"));
}

// ($^(age)).name
TEST_F(sortingTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case015.json"));
}

// $^(age)[0].name
TEST_F(sortingTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case016.json"));
}

// ($^(age))[0].name
TEST_F(sortingTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case017.json"));
}

// ($^(age)[0]).name
TEST_F(sortingTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case018.json"));
}

// $[0]^(age)
TEST_F(sortingTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case019.json"));
}

// Account.Order#$o.Product^(ProductID).{ 'Product': `Product Name`, 'Order Index': $o }
TEST_F(sortingTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/sorting/case020.json"));
}

