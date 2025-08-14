#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class closuresTest : public JsonataTest {
};

// Account.(  $AccName := function() { $.\"Account Name\" };  Order[OrderID = \"order104\"].Product{    \"Account\": $AccName(),    \"SKU-\" & $string(ProductID): $.\"Product Name\"  } )
TEST_F(closuresTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/closures/case000.json"));
}

// Account.(  $AccName := function() { `Account Name` };  Order[OrderID = \"order104\"].Product{    \"Account\": $AccName(),    \"SKU-\" & $string(ProductID): `Product Name`  } )
TEST_F(closuresTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/closures/case001.json"));
}

