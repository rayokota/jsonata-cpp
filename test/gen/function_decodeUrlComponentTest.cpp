#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_decodeUrlComponentTest : public JsonataTest {
};

// $decodeUrlComponent(\"%3Fx%3Dtest\")
TEST_F(function_decodeUrlComponentTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-decodeUrlComponent/case000.json"));
}

// $decodeUrlComponent(blah)
TEST_F(function_decodeUrlComponentTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-decodeUrlComponent/case001.json"));
}

// $decodeUrlComponent('%E0%A4%A')
TEST_F(function_decodeUrlComponentTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-decodeUrlComponent/case002.json"));
}

