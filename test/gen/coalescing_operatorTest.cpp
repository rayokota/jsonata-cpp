#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class coalescing_operatorTest : public JsonataTest {
};

// bar ?? 42
TEST_F(coalescing_operatorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case000.json"));
}

// foo.bar ?? 98
TEST_F(coalescing_operatorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case001.json"));
}

// foo.blah[0].baz.fud ?? 98
TEST_F(coalescing_operatorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case002.json"));
}

// baz ?? 42
TEST_F(coalescing_operatorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case003.json"));
}

// foo.baz ?? 42
TEST_F(coalescing_operatorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case004.json"));
}

// foo.blah[9].baz.fud ?? 42
TEST_F(coalescing_operatorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case005.json"));
}

// null ?? 42
TEST_F(coalescing_operatorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case006.json"));
}

// false ?? 42
TEST_F(coalescing_operatorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case007.json"));
}

// true ?? 42
TEST_F(coalescing_operatorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case008.json"));
}

// 0 ?? 42
TEST_F(coalescing_operatorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case009.json"));
}

// [] ?? 42
TEST_F(coalescing_operatorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case010.json"));
}

// {} ?? 42
TEST_F(coalescing_operatorTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case011.json"));
}

// \"\" ?? 42
TEST_F(coalescing_operatorTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/coalescing-operator/case012.json"));
}

