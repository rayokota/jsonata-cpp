#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class predicatesTest : public JsonataTest {
};

// nothing[x=6][y=3].number
TEST_F(predicatesTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/predicates/case000.json"));
}

// clues[x=6][y=3].number
TEST_F(predicatesTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/predicates/case001.json"));
}

// $[x=6][y=3].number
TEST_F(predicatesTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/predicates/case002.json"));
}

// Account.Order.Product[$lowercase(Description.Colour) = \"purple\"][0].Price
TEST_F(predicatesTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/predicates/case003.json"));
}

