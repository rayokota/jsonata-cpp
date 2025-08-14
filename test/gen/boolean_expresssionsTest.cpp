#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class boolean_expresssionsTest : public JsonataTest {
};

// true
TEST_F(boolean_expresssionsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case000.json"));
}

// false
TEST_F(boolean_expresssionsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case001.json"));
}

// false or false
TEST_F(boolean_expresssionsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case002.json"));
}

// false or true
TEST_F(boolean_expresssionsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case003.json"));
}

// true or false
TEST_F(boolean_expresssionsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case004.json"));
}

// true or true
TEST_F(boolean_expresssionsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case005.json"));
}

// false and false
TEST_F(boolean_expresssionsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case006.json"));
}

// false and true
TEST_F(boolean_expresssionsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case007.json"));
}

// true and false
TEST_F(boolean_expresssionsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case008.json"));
}

// true and true
TEST_F(boolean_expresssionsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case009.json"));
}

// $not(false)
TEST_F(boolean_expresssionsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case010.json"));
}

// $not(true)
TEST_F(boolean_expresssionsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case011.json"));
}

// and=1 and or=2
TEST_F(boolean_expresssionsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case012.json"));
}

// and>1 or or<=2
TEST_F(boolean_expresssionsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case013.json"));
}

// and>1 or or!=2
TEST_F(boolean_expresssionsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case014.json"));
}

// and and and
TEST_F(boolean_expresssionsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case015.json"));
}

// $[].content.origin.$lowercase(name)
TEST_F(boolean_expresssionsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case016.json"));
}

// true or foo
TEST_F(boolean_expresssionsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case017.json"));
}

// foo or true
TEST_F(boolean_expresssionsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case018.json"));
}

// false or foo
TEST_F(boolean_expresssionsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case019.json"));
}

// foo or false
TEST_F(boolean_expresssionsTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case020.json"));
}

// foo or bar
TEST_F(boolean_expresssionsTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case021.json"));
}

// true and foo
TEST_F(boolean_expresssionsTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case022.json"));
}

// foo and true
TEST_F(boolean_expresssionsTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case023.json"));
}

// false and foo
TEST_F(boolean_expresssionsTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case024.json"));
}

// foo and false
TEST_F(boolean_expresssionsTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case025.json"));
}

// foo and bar
TEST_F(boolean_expresssionsTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case026.json"));
}

// $not(nothing)
TEST_F(boolean_expresssionsTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case027.json"));
}

// foo = '' or $number(foo) = 0
TEST_F(boolean_expresssionsTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case028.json"));
}

// $type(data) = 'number' and data > 10
TEST_F(boolean_expresssionsTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case029.json"));
}

// $type(age) = 'number' or $number(age) > 0
TEST_F(boolean_expresssionsTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/boolean-expresssions/case030.json"));
}

