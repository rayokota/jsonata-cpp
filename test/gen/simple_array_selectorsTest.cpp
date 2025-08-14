#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class simple_array_selectorsTest : public JsonataTest {
};

// nest0.nest1[0]
TEST_F(simple_array_selectorsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case000.json"));
}

// foo.blah[0].baz.fud
TEST_F(simple_array_selectorsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case001.json"));
}

// foo.blah[1].baz.fud
TEST_F(simple_array_selectorsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case002.json"));
}

// foo.blah[-1].bazz
TEST_F(simple_array_selectorsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case003.json"));
}

// (foo.blah)[1].baz.fud
TEST_F(simple_array_selectorsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case004.json"));
}

// foo.blah.baz.fud[0]
TEST_F(simple_array_selectorsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case005.json"));
}

// foo.blah.baz.fud[-1]
TEST_F(simple_array_selectorsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case006.json"));
}

// (foo.blah.baz.fud)[0]
TEST_F(simple_array_selectorsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case007.json"));
}

// (foo.blah.baz.fud)[1]
TEST_F(simple_array_selectorsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case008.json"));
}

// (foo.blah.baz.fud)[5 * 0.2]
TEST_F(simple_array_selectorsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case009.json"));
}

// (foo.blah.baz.fud)[-1]
TEST_F(simple_array_selectorsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case010.json"));
}

// (foo.blah.baz.fud)[-2]
TEST_F(simple_array_selectorsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case011.json"));
}

// (foo.blah.baz.fud)[2-4]
TEST_F(simple_array_selectorsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case012.json"));
}

// (foo.blah.baz.fud)[-(4-2)]
TEST_F(simple_array_selectorsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case013.json"));
}

// (foo.blah.baz.fud)[$$.foo.bar / 30]
TEST_F(simple_array_selectorsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case014.json"));
}

// foo.blah[0].baz
TEST_F(simple_array_selectorsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case015.json"));
}

// foo.blah.baz[0]
TEST_F(simple_array_selectorsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case016.json"));
}

// (foo.blah.baz)[0]
TEST_F(simple_array_selectorsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case017.json"));
}

// $[0]
TEST_F(simple_array_selectorsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case018.json"));
}

// $[1]
TEST_F(simple_array_selectorsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case019.json"));
}

// $[-1]
TEST_F(simple_array_selectorsTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case020.json"));
}

// $[1][0]
TEST_F(simple_array_selectorsTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case021.json"));
}

// $[1.1][0.9]
TEST_F(simple_array_selectorsTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/simple-array-selectors/case022.json"));
}

