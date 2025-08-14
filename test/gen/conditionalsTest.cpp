#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class conditionalsTest : public JsonataTest {
};

// [\"Red\"[$$=\"Bus\"], \"White\"[$$=\"Police Car\"]][0]
TEST_F(conditionalsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case000.json"));
}

// [\"Red\"[$$=\"Bus\"], \"White\"[$$=\"Police Car\"]][0]
TEST_F(conditionalsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case001.json"));
}

// [\"Red\"[$$=\"Bus\"], \"White\"[$$=\"Police Car\"]][0]
TEST_F(conditionalsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case002.json"));
}

// $lookup({\"Bus\": \"Red\", \"Police Car\": \"White\"}, $$)
TEST_F(conditionalsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case003.json"));
}

// $lookup({\"Bus\": \"Red\", \"Police Car\": \"White\"}, $$)
TEST_F(conditionalsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case004.json"));
}

// $lookup({\"Bus\": \"Red\", \"Police Car\": \"White\"}, $$)
TEST_F(conditionalsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case005.json"));
}

// Account.Order.Product.(Price < 30 ? \"Cheap\")
TEST_F(conditionalsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case006.json"));
}

// Account.Order.Product.(Price < 30 ? \"Cheap\" : \"Expensive\")
TEST_F(conditionalsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case007.json"));
}

// Account.Order.Product.(Price < 30 ? \"Cheap\" : Price < 100 ? \"Expensive\" : \"Rip off\")
TEST_F(conditionalsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/conditionals/case008.json"));
}

