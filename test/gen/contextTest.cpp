#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class contextTest : public JsonataTest {
};

// $number()
TEST_F(contextTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/context/case000.json"));
}

// [1..5].$string()
TEST_F(contextTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/context/case001.json"));
}

// [1..5].(\"Item \" & $string())
TEST_F(contextTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/context/case002.json"));
}

// Account.Order.Product.\"Product Name\".$uppercase().$substringBefore(\" \")
TEST_F(contextTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/context/case003.json"));
}

