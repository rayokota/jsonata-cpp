#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_booleanTest : public JsonataTest {
};

// $boolean(\"Hello World\")
TEST_F(function_booleanTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case000.json"));
}

// $boolean(\"\")
TEST_F(function_booleanTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case001.json"));
}

// $boolean(true)
TEST_F(function_booleanTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case002.json"));
}

// $boolean(false)
TEST_F(function_booleanTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case003.json"));
}

// $boolean(0)
TEST_F(function_booleanTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case004.json"));
}

// $boolean(10)
TEST_F(function_booleanTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case005.json"));
}

// $boolean(-0.5)
TEST_F(function_booleanTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case006.json"));
}

// $boolean(null)
TEST_F(function_booleanTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case007.json"));
}

// $boolean([])
TEST_F(function_booleanTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case008.json"));
}

// $boolean([0])
TEST_F(function_booleanTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case009.json"));
}

// $boolean([1])
TEST_F(function_booleanTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case010.json"));
}

// $boolean([1,2,3])
TEST_F(function_booleanTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case011.json"));
}

// $boolean([0,0])
TEST_F(function_booleanTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case012.json"));
}

// $boolean([[]])
TEST_F(function_booleanTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case013.json"));
}

// $boolean([[null]])
TEST_F(function_booleanTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case014.json"));
}

// $boolean([[[true]]])
TEST_F(function_booleanTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case015.json"));
}

// $boolean({})
TEST_F(function_booleanTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case016.json"));
}

// $boolean({\"hello\":\"world\"})
TEST_F(function_booleanTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case017.json"));
}

// $boolean(Account)
TEST_F(function_booleanTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case018.json"));
}

// $boolean(Account.Order.Product.Price)
TEST_F(function_booleanTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case019.json"));
}

// $boolean(Account.blah)
TEST_F(function_booleanTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case020.json"));
}

// $boolean($boolean)
TEST_F(function_booleanTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case021.json"));
}

// $boolean(function(){true})
TEST_F(function_booleanTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case022.json"));
}

// $boolean(2,3)
TEST_F(function_booleanTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-boolean/case023.json"));
}

