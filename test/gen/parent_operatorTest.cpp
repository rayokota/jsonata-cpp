#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class parent_operatorTest : public JsonataTest {
};

// {'hello': 'world'}.% 
TEST_F(parent_operatorTest, errors_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 0));
}

// %
TEST_F(parent_operatorTest, errors_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 1));
}

// %()
TEST_F(parent_operatorTest, errors_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 2));
}

// %(1)
TEST_F(parent_operatorTest, errors_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 3));
}

// %%
TEST_F(parent_operatorTest, errors_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 4));
}

// (%)
TEST_F(parent_operatorTest, errors_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 5));
}

// (%%)
TEST_F(parent_operatorTest, errors_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 6));
}

// library.loans.%%
TEST_F(parent_operatorTest, errors_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 7));
}

// $.%
TEST_F(parent_operatorTest, errors_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 8));
}

// $$.%
TEST_F(parent_operatorTest, errors_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 9));
}

// library.loans.%.%.%
TEST_F(parent_operatorTest, errors_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 10));
}

// library.%%%
TEST_F(parent_operatorTest, errors_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 11));
}

// library.(%%%)
TEST_F(parent_operatorTest, errors_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 12));
}

// library.(%% %)
TEST_F(parent_operatorTest, errors_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 13));
}

// library.(% %%)
TEST_F(parent_operatorTest, errors_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 14));
}

// library.(% % %)
TEST_F(parent_operatorTest, errors_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/errors.json", 15));
}

// 
TEST_F(parent_operatorTest, parent_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 0));
}

// 
TEST_F(parent_operatorTest, parent_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 1));
}

// Account.Order.Product.[`Product Name`, %.OrderID]
TEST_F(parent_operatorTest, parent_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 2));
}

// Account.Order.Product.{ `Product Name`: [Quantity, %.OrderID] }
TEST_F(parent_operatorTest, parent_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 3));
}

// Account.Order.Product.{ `Product Name`: [Quantity, (%.OrderID)] }
TEST_F(parent_operatorTest, parent_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 4));
}

// Account.Order.Product[%.OrderID='order104'].SKU
TEST_F(parent_operatorTest, parent_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 5));
}

// Account.Order.Product[%.%.`Account Name`='Firefly'].SKU
TEST_F(parent_operatorTest, parent_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 6));
}

// Account.Order.Product.Price[%.%.OrderID='order103']
TEST_F(parent_operatorTest, parent_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 7));
}

// Account.Order.Product.Price.%[%.OrderID='order103'].SKU
TEST_F(parent_operatorTest, parent_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 8));
}

// Account.Order.Product[%.OrderID='order104'][%.%.`Account Name`='Firefly'].SKU
TEST_F(parent_operatorTest, parent_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 9));
}

// (Account.Order.Product)[%.OrderID='order104'].SKU
TEST_F(parent_operatorTest, parent_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 10));
}

// Account.Order.Product.{ %.OrderID: Price * Quantity }
TEST_F(parent_operatorTest, parent_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 11));
}

// Account.Order.().%
TEST_F(parent_operatorTest, parent_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 12));
}

// 
TEST_F(parent_operatorTest, parent_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 13));
}

// 
TEST_F(parent_operatorTest, parent_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 14));
}

// 
TEST_F(parent_operatorTest, parent_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 15));
}

// 
TEST_F(parent_operatorTest, parent_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 16));
}

// 
TEST_F(parent_operatorTest, parent_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 17));
}

// Account.Order.Product.SKU^(%.Price)
TEST_F(parent_operatorTest, parent_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 18));
}

// Account.Order.Product.SKU^(%.Price, >%.%.OrderID)
TEST_F(parent_operatorTest, parent_case_19) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 19));
}

// Account.Order.Product.Description.{ 'Colour': Colour, 'Total': %.Price * %.Quantity }
TEST_F(parent_operatorTest, parent_case_20) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 20));
}

// Account.Order.Product.Description.{ 'Item': Colour & ' ' & %.`Product Name`, 'Discounted': (%.%.OrderID = 'order103') ? %.Price / 2 : %.Price }
TEST_F(parent_operatorTest, parent_case_21) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 21));
}

// Account.Order.Product.( $parent := %; %.OrderID )
TEST_F(parent_operatorTest, parent_case_22) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 22));
}

// Account.Order.Product.( $parent := %; $parent.OrderID )
TEST_F(parent_operatorTest, parent_case_23) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 23));
}

// library.loans@$L.books@$B[$L.isbn=$B.isbn].{ 'book': $B.title, 'parent': $keys(%) }
TEST_F(parent_operatorTest, parent_case_24) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 24));
}

// library.loans@$L.books@$B[$L.isbn=$B.isbn].customers[id=$L.customer].{ 'book': $B.title, 'customer': name, 'parent': $keys(%) }
TEST_F(parent_operatorTest, parent_case_25) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 25));
}

// library.loans@$L.books@$B[$L.isbn=$B.isbn].customers[id=$L.customer].{ 'book': $B.title, 'customer': name, 'parent': $keys(%.%) }
TEST_F(parent_operatorTest, parent_case_26) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 26));
}

// library.loans@$L.books@$B[$L.isbn=$B.isbn].customers@$C[$C.id=$L.customer].{ 'book': $B.title, 'customer': $C.name, 'grandparent': $keys(%.%) }
TEST_F(parent_operatorTest, parent_case_27) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/parent-operator/parent.json", 27));
}

