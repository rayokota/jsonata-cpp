#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_numberTest : public JsonataTest {
};

// $number(0)
TEST_F(function_numberTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case000.json"));
}

// $number(10)
TEST_F(function_numberTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case001.json"));
}

// $number(-0.05)
TEST_F(function_numberTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case002.json"));
}

// $number(\"0\")
TEST_F(function_numberTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case003.json"));
}

// $number(\"-0.05\")
TEST_F(function_numberTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case004.json"));
}

// $number(\"1e2\")
TEST_F(function_numberTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case005.json"));
}

// $number(\"1.0e-2\")
TEST_F(function_numberTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case006.json"));
}

// $number(\"1e0\")
TEST_F(function_numberTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case007.json"));
}

// $number(\"10e500\")
TEST_F(function_numberTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case008.json"));
}

// $number(\"Hello world\")
TEST_F(function_numberTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case009.json"));
}

// $number(\"1/2\")
TEST_F(function_numberTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case010.json"));
}

// $number(\"1234 hello\")
TEST_F(function_numberTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case011.json"));
}

// $number(\"\")
TEST_F(function_numberTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case012.json"));
}

// $number(true)
TEST_F(function_numberTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case013.json"));
}

// $number(false)
TEST_F(function_numberTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case014.json"));
}

// $number(Account.blah)
TEST_F(function_numberTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case015.json"));
}

// $number(null)
TEST_F(function_numberTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case016.json"));
}

// $number([])
TEST_F(function_numberTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case017.json"));
}

// $number(\"[1]\")
TEST_F(function_numberTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case018.json"));
}

// $number([1,2])
TEST_F(function_numberTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case019.json"));
}

// $number([\"hello\"])
TEST_F(function_numberTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case020.json"));
}

// $number([\"2\"])
TEST_F(function_numberTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case021.json"));
}

// $number({})
TEST_F(function_numberTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case022.json"));
}

// $number({\"hello\":\"world\"})
TEST_F(function_numberTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case023.json"));
}

// $number($number)
TEST_F(function_numberTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case024.json"));
}

// $number(function(){5})
TEST_F(function_numberTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case025.json"));
}

// $number(1,2)
TEST_F(function_numberTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case026.json"));
}

// $number('00')
TEST_F(function_numberTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case027.json"));
}

// $number('0123')
TEST_F(function_numberTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case028.json"));
}

// $number('-007')
TEST_F(function_numberTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case029.json"));
}

// $number('000.00123000')
TEST_F(function_numberTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case030.json"));
}

// $number('0x12')
TEST_F(function_numberTest, case031) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case031.json"));
}

// $number('0B101')
TEST_F(function_numberTest, case032) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case032.json"));
}

// $number('0O12')
TEST_F(function_numberTest, case033) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-number/case033.json"));
}

