#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class string_concatTest : public JsonataTest {
};

// \"foo\" & \"bar\"
TEST_F(string_concatTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case000.json"));
}

// \"foo\"&\"bar\"
TEST_F(string_concatTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case001.json"));
}

// foo.blah[0].baz.fud &foo.blah[1].baz.fud
TEST_F(string_concatTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case002.json"));
}

// foo.(blah[0].baz.fud & blah[1].baz.fud)
TEST_F(string_concatTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case003.json"));
}

// foo.(blah[0].baz.fud & none)
TEST_F(string_concatTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case004.json"));
}

// foo.(none.here & blah[1].baz.fud)
TEST_F(string_concatTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case005.json"));
}

// [1,2]&[3,4]
TEST_F(string_concatTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case006.json"));
}

// [1,2]&3
TEST_F(string_concatTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case007.json"));
}

// 1&2
TEST_F(string_concatTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case008.json"));
}

// 1&[2]
TEST_F(string_concatTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case009.json"));
}

// \"hello\"&5
TEST_F(string_concatTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case010.json"));
}

// \"Prices: \" & Account.Order.Product.Price
TEST_F(string_concatTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/string-concat/case011.json"));
}

