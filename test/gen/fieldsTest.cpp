#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class fieldsTest : public JsonataTest {
};

// foo.bar
TEST_F(fieldsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case000.json"));
}

// foo.blah
TEST_F(fieldsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case001.json"));
}

// foo.blah.bazz
TEST_F(fieldsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case002.json"));
}

// foo.blah.baz
TEST_F(fieldsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case003.json"));
}

// foo.blah.baz.fud
TEST_F(fieldsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case004.json"));
}

// Other.Misc
TEST_F(fieldsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case005.json"));
}

// bazz
TEST_F(fieldsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case006.json"));
}

// fud
TEST_F(fieldsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/fields/case007.json"));
}

