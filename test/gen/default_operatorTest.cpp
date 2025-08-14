#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class default_operatorTest : public JsonataTest {
};

// true ?: 42
TEST_F(default_operatorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case000.json"));
}

// false ?: 42
TEST_F(default_operatorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case001.json"));
}

// 1 ?: 42
TEST_F(default_operatorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case002.json"));
}

// 0 ?: 42
TEST_F(default_operatorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case003.json"));
}

// \"hello\" ?: 42
TEST_F(default_operatorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case004.json"));
}

// \"\" ?: 42
TEST_F(default_operatorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case005.json"));
}

// [1] ?: 42
TEST_F(default_operatorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case006.json"));
}

// [0] ?: 42
TEST_F(default_operatorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case007.json"));
}

// [] ?: 42
TEST_F(default_operatorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case008.json"));
}

// { \"a\": 1 } ?: 42
TEST_F(default_operatorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case009.json"));
}

// {} ?: 42
TEST_F(default_operatorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case010.json"));
}

// function(){true} ?: 42
TEST_F(default_operatorTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case011.json"));
}

// Account.blah ?: 42
TEST_F(default_operatorTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case012.json"));
}

// Account.blah ?: 42
TEST_F(default_operatorTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/default-operator/case013.json"));
}

