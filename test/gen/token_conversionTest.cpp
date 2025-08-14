#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class token_conversionTest : public JsonataTest {
};

// $.'7a'
TEST_F(token_conversionTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/token-conversion/case000.json"));
}

// $.'7'
TEST_F(token_conversionTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/token-conversion/case001.json"));
}

// $.7a
TEST_F(token_conversionTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/token-conversion/case002.json"));
}

// $.7
TEST_F(token_conversionTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/token-conversion/case003.json"));
}

