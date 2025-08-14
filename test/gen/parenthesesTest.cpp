#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class parenthesesTest : public JsonataTest {
};

// foo.(blah).baz.fud
TEST_F(parenthesesTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case000.json"));
}

// foo.(blah.baz).fud
TEST_F(parenthesesTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case001.json"));
}

// (foo.blah.baz).fud
TEST_F(parenthesesTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case002.json"));
}

// foo.blah.(baz.fud)
TEST_F(parenthesesTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case003.json"));
}

// (foo.blah.baz.fud)
TEST_F(parenthesesTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case004.json"));
}

// (foo).(blah).baz.(fud)
TEST_F(parenthesesTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case005.json"));
}

// (foo.(blah).baz.fud)
TEST_F(parenthesesTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case006.json"));
}

// (4 + 2) / 2
TEST_F(parenthesesTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/parentheses/case007.json"));
}

