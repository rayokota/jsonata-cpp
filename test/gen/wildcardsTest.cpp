#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class wildcardsTest : public JsonataTest {
};

// foo.*
TEST_F(wildcardsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case000.json"));
}

// foo.*.baz
TEST_F(wildcardsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case001.json"));
}

// foo.*.bazz
TEST_F(wildcardsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case002.json"));
}

// foo.*.baz.*
TEST_F(wildcardsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case003.json"));
}

// foo.*.baz.*
TEST_F(wildcardsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case004.json"));
}

// foo.*.baz.*
TEST_F(wildcardsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case005.json"));
}

// *[type=\"home\"]
TEST_F(wildcardsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case006.json"));
}

// Account[$$.Account.\"Account Name\" = \"Firefly\"].*[OrderID=\"order104\"].Product.Price
TEST_F(wildcardsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case007.json"));
}

// Account[$$.Account.`Account Name` = \"Firefly\"].*[OrderID=\"order104\"].Product.Price
TEST_F(wildcardsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case008.json"));
}

// *
TEST_F(wildcardsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/wildcards/case009.json"));
}

