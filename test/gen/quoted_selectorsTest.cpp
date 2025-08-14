#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class quoted_selectorsTest : public JsonataTest {
};

// foo.\"blah\"
TEST_F(quoted_selectorsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case000.json"));
}

// foo.\"blah\".baz.'fud'
TEST_F(quoted_selectorsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case001.json"));
}

// \"foo\".\"blah\".\"baz\".\"fud\"
TEST_F(quoted_selectorsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case002.json"));
}

// foo.\"blah.baz\"
TEST_F(quoted_selectorsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case003.json"));
}

// foo.`blah`
TEST_F(quoted_selectorsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case004.json"));
}

// foo.`blah`.baz.'fud'
TEST_F(quoted_selectorsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case005.json"));
}

// `foo`.`blah`.`baz`.`fud`
TEST_F(quoted_selectorsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case006.json"));
}

// foo.`blah.baz`
TEST_F(quoted_selectorsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/quoted-selectors/case007.json"));
}

