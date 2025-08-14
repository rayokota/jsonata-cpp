#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class descendent_operatorTest : public JsonataTest {
};

// foo.**.blah
TEST_F(descendent_operatorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case000.json"));
}

// foo.**.baz
TEST_F(descendent_operatorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case001.json"));
}

// foo.**.fud
TEST_F(descendent_operatorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case002.json"));
}

// \"foo\".**.fud
TEST_F(descendent_operatorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case003.json"));
}

// foo.**.\"fud\"
TEST_F(descendent_operatorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case004.json"));
}

// \"foo\".**.\"fud\"
TEST_F(descendent_operatorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case005.json"));
}

// foo.*.**.fud
TEST_F(descendent_operatorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case006.json"));
}

// foo.**.*.fud
TEST_F(descendent_operatorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case007.json"));
}

// Account.Order.**.Colour
TEST_F(descendent_operatorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case008.json"));
}

// foo.**.fud[0]
TEST_F(descendent_operatorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case009.json"));
}

// (foo.**.fud)[0]
TEST_F(descendent_operatorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case010.json"));
}

// (**.fud)[0]
TEST_F(descendent_operatorTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case011.json"));
}

// **.Price
TEST_F(descendent_operatorTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case012.json"));
}

// **.Price[0]
TEST_F(descendent_operatorTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case013.json"));
}

// (**.Price)[0]
TEST_F(descendent_operatorTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case014.json"));
}

// Account.Order.blah.**
TEST_F(descendent_operatorTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case015.json"));
}

// **
TEST_F(descendent_operatorTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/descendent-operator/case016.json"));
}

