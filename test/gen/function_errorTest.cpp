#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_errorTest : public JsonataTest {
};

// Account.Order[0].Product[0].Price > 35 ? Account.Order[0].Product[0].Price : $error('Too Expensive')
TEST_F(function_errorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case000.json"));
}

// Account.Order[0].Product[0].Price > 34 ? Account.Order[0].Product[0].Price : $error('Too Expensive')
TEST_F(function_errorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case001.json"));
}

// Account.Order[0].Product[0].Price > 35 ? $error('Too Expensive') : Account.Order[0].Product[0].Price
TEST_F(function_errorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case002.json"));
}

// Account.Order[0].Product[0].Price > 34 ? $error('Too Expensive') : Account.Order[0].Product[0].Price
TEST_F(function_errorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case003.json"));
}

// $count(Account.Order[0].Product) < 2 ? $error('Not enough products in orders')
TEST_F(function_errorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case004.json"));
}

// $count(Account.Order[0].Product) < 3 ? $error('Not enough products in orders')
TEST_F(function_errorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case005.json"));
}

// ($msg:='My Message'; $error($msg); true)
TEST_F(function_errorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case006.json"));
}

// $error(null)
TEST_F(function_errorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case007.json"));
}

// $error(5)
TEST_F(function_errorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case008.json"));
}

// $error()
TEST_F(function_errorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case009.json"));
}

// $error(foo)
TEST_F(function_errorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-error/case010.json"));
}

