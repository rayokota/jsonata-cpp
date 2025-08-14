#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_encodeUrlComponentTest : public JsonataTest {
};

// $encodeUrlComponent(\"?x=test\")
TEST_F(function_encodeUrlComponentTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-encodeUrlComponent/case000.json"));
}

// $encodeUrlComponent(blah)
TEST_F(function_encodeUrlComponentTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-encodeUrlComponent/case001.json"));
}

// $encodeUrlComponent('\\uD800')
TEST_F(function_encodeUrlComponentTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-encodeUrlComponent/case002.json"));
}

