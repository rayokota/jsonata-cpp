#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_evalTest : public JsonataTest {
};

// $eval('[1,2,3]')
TEST_F(function_evalTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case000.json"));
}

// $eval(nothing)
TEST_F(function_evalTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case001.json"));
}

// $eval('[1,$string(2),3]')
TEST_F(function_evalTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case002.json"));
}

// $eval('Account.Order.Product.Quantity ~> $sum()')
TEST_F(function_evalTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case003.json"));
}

// Account.Order.Product.{'Name': `Product Name`, 'Total': $eval('Price * Quantity')}
TEST_F(function_evalTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case004.json"));
}

// Account.Order.Product.{'Name': `Product Name`, 'Volume': $eval('Width*Height*Depth', Description)}
TEST_F(function_evalTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case005.json"));
}

// $eval('[1,string(2),3]')
TEST_F(function_evalTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case006.json"));
}

// $eval('[1,#string(2),3]')
TEST_F(function_evalTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-eval/case007.json"));
}

// $eval(\"{ 'test': 1 }\", [])
TEST_F(function_evalTest, case008_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-eval/case008.json", 0));
}

// $eval(\"{ 'test': 1 }\")
TEST_F(function_evalTest, case008_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-eval/case008.json", 1));
}

// $eval(\"{ 'test': 1 }\", [1,2,3])
TEST_F(function_evalTest, case008_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-eval/case008.json", 2));
}

// $eval(\"{ 'test': 1 }\")
TEST_F(function_evalTest, case008_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-eval/case008.json", 3));
}

