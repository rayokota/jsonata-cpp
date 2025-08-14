#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_encodeUrlTest : public JsonataTest {
};

// $encodeUrl(\"https://mozilla.org/?x=шеллы\")
TEST_F(function_encodeUrlTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-encodeUrl/case000.json"));
}

// $encodeUrl(blah)
TEST_F(function_encodeUrlTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-encodeUrl/case001.json"));
}

// $encodeUrl('\\uD800')
TEST_F(function_encodeUrlTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-encodeUrl/case002.json"));
}

