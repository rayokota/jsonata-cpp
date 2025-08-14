#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class matchersTest : public JsonataTest {
};

// 
TEST_F(matchersTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/matchers/case000.json"));
}

// $split('some text', $uppercase)
TEST_F(matchersTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/matchers/case001.json"));
}

