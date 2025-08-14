#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_decodeUrlTest : public JsonataTest {
};

// $decodeUrl(\"https://mozilla.org/?x=%D1%88%D0%B5%D0%BB%D0%BB%D1%8B\")
TEST_F(function_decodeUrlTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-decodeUrl/case000.json"));
}

// $decodeUrl(blah)
TEST_F(function_decodeUrlTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-decodeUrl/case001.json"));
}

// $decodeUrl('%E0%A4%A')
TEST_F(function_decodeUrlTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-decodeUrl/case002.json"));
}

