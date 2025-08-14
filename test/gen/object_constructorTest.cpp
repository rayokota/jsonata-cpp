#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class object_constructorTest : public JsonataTest {
};

// {}
TEST_F(object_constructorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case000.json"));
}

// {\"key\": \"value\"}
TEST_F(object_constructorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case001.json"));
}

// {\"one\": 1, \"two\": 2}
TEST_F(object_constructorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case002.json"));
}

// {\"one\": 1, \"two\": 2}.two
TEST_F(object_constructorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case003.json"));
}

// {\"one\": 1, \"two\": {\"three\": 3, \"four\": \"4\"}}
TEST_F(object_constructorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case004.json"));
}

// {\"one\": 1, \"two\": [3, \"four\"]}
TEST_F(object_constructorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case005.json"));
}

// {\"test\": ()}
TEST_F(object_constructorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case006.json"));
}

// blah.{}
TEST_F(object_constructorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case007.json"));
}

// Account.Order{OrderID: Product.\"Product Name\"}
TEST_F(object_constructorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case008.json"));
}

// Account.Order.{OrderID: Product.\"Product Name\"}
TEST_F(object_constructorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case009.json"));
}

// Account.Order.Product{$string(ProductID): Price}
TEST_F(object_constructorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case010.json"));
}

// Account.Order.Product{$string(ProductID): (Price)[0]}
TEST_F(object_constructorTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case011.json"));
}

// Account.Order.Product.{$string(ProductID): Price}
TEST_F(object_constructorTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case012.json"));
}

// Account.Order.Product{ProductID: \"Product Name\"}
TEST_F(object_constructorTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case013.json"));
}

// Account.Order.Product.{ProductID: \"Product Name\"}
TEST_F(object_constructorTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case014.json"));
}

// Account.Order{OrderID: $sum(Product.(Price*Quantity))}
TEST_F(object_constructorTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case015.json"));
}

// Account.Order.{OrderID: $sum(Product.(Price*Quantity))}
TEST_F(object_constructorTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case016.json"));
}

// Account.Order.Product{$.\"Product Name\": Price, $.\"Product Name\": Price}
TEST_F(object_constructorTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case017.json"));
}

// Account.Order.Product{`Product Name`: Price, SKU: Price}
TEST_F(object_constructorTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case018.json"));
}

// Account.Order{  OrderID: {    \"TotalPrice\":$sum(Product.(Price * Quantity)),    \"Items\": Product.\"Product Name\"  }}
TEST_F(object_constructorTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case019.json"));
}

// {  \"Order\": Account.Order.{      \"ID\": OrderID,      \"Product\": Product.{          \"Name\": $.\"Product Name\",          \"SKU\": ProductID,          \"Details\": {            \"Weight\": Description.Weight,            \"Dimensions\": Description.(Width & \" x \" & Height & \" x \" & Depth)          }        },      \"Total Price\": $sum(Product.(Price * Quantity))    }}
TEST_F(object_constructorTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case020.json"));
}

// {  \"Order\": Account.Order.{      \"ID\": OrderID,      \"Product\": Product.{          \"Name\": `Product Name`,          \"SKU\": ProductID,          \"Details\": {            \"Weight\": Description.Weight,            \"Dimensions\": Description.(Width & \" x \" & Height & \" x \" & Depth)          }        },      \"Total Price\": $sum(Product.(Price * Quantity))    }}
TEST_F(object_constructorTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case021.json"));
}

// Phone{type: $join(number, \", \"), \"phone\":number}
TEST_F(object_constructorTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case022.json"));
}

// $.{ type: $average(value), kind: $sum(value) } 
TEST_F(object_constructorTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case023.json"));
}

// { type: $average(value), kind: $sum(value) } 
TEST_F(object_constructorTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case024.json"));
}

// 
TEST_F(object_constructorTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case025.json"));
}

// $${id:{'label':label,'value':value}}
TEST_F(object_constructorTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/object-constructor/case026.json"));
}

