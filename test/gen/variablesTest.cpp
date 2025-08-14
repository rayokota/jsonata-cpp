#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class variablesTest : public JsonataTest {
};

// $price.foo.bar
TEST_F(variablesTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case000.json"));
}

// $price.foo.bar
TEST_F(variablesTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case001.json"));
}

// $var[1]
TEST_F(variablesTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case002.json"));
}

// $.foo.bar
TEST_F(variablesTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case003.json"));
}

// $a := 5
TEST_F(variablesTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case004.json"));
}

// $a := $b := 5
TEST_F(variablesTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case005.json"));
}

// ($a := $b := 5; $a)
TEST_F(variablesTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case006.json"));
}

// ($a := $b := 5; $b)
TEST_F(variablesTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case007.json"));
}

// ( $a := 5; $a := $a + 2; $a )
TEST_F(variablesTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case008.json"));
}

// [1,2,3].$v
TEST_F(variablesTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case009.json"));
}

// ( $foo := \"defined\"; ( $foo := nothing ); $foo )
TEST_F(variablesTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case010.json"));
}

// ( $foo := \"defined\"; ( $foo := nothing; $foo ) )
TEST_F(variablesTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case011.json"));
}

// ($a := [1,2]; $a[1]:=3; $a)
TEST_F(variablesTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/variables/case012.json"));
}

