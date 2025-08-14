#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class numeric_operatorsTest : public JsonataTest {
};

// foo.bar + bar
TEST_F(numeric_operatorsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case000.json"));
}

// bar + foo.bar
TEST_F(numeric_operatorsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case001.json"));
}

// foo.bar - bar
TEST_F(numeric_operatorsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case002.json"));
}

// bar - foo.bar
TEST_F(numeric_operatorsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case003.json"));
}

// foo.bar * bar
TEST_F(numeric_operatorsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case004.json"));
}

// bar * foo.bar
TEST_F(numeric_operatorsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case005.json"));
}

// foo.bar / bar
TEST_F(numeric_operatorsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case006.json"));
}

// bar / foo.bar
TEST_F(numeric_operatorsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case007.json"));
}

// foo.bar % bar
TEST_F(numeric_operatorsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case008.json"));
}

// bar % foo.bar
TEST_F(numeric_operatorsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case009.json"));
}

// bar + foo.bar * bar
TEST_F(numeric_operatorsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case010.json"));
}

// foo.bar * bar + bar
TEST_F(numeric_operatorsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case011.json"));
}

// 24 * notexist
TEST_F(numeric_operatorsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case012.json"));
}

// notexist + 1
TEST_F(numeric_operatorsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case013.json"));
}

// 1/(10e300 * 10e100) 
TEST_F(numeric_operatorsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case014.json"));
}

// \"5\" + \"5\"
TEST_F(numeric_operatorsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case015.json"));
}

// - notexist
TEST_F(numeric_operatorsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case016.json"));
}

// false + 1
TEST_F(numeric_operatorsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case017.json"));
}

// false + $x
TEST_F(numeric_operatorsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/numeric-operators/case018.json"));
}

