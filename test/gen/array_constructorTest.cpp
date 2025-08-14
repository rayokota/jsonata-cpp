#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class array_constructorTest : public JsonataTest {
};

// $.[value,epochSeconds][]
TEST_F(array_constructorTest, array_sequences_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/array-constructor/array-sequences.json", 0));
}

// $.[value,epochSeconds][]
TEST_F(array_constructorTest, array_sequences_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/array-constructor/array-sequences.json", 1));
}

// $.[value,epochSeconds]
TEST_F(array_constructorTest, array_sequences_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/array-constructor/array-sequences.json", 2));
}

// singleArray
TEST_F(array_constructorTest, array_sequences_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/array-constructor/array-sequences.json", 3));
}

// singleArray[]
TEST_F(array_constructorTest, array_sequences_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/array-constructor/array-sequences.json", 4));
}

// []
TEST_F(array_constructorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case000.json"));
}

// [1]
TEST_F(array_constructorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case001.json"));
}

// [1, 2]
TEST_F(array_constructorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case002.json"));
}

// [1, 2,3]
TEST_F(array_constructorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case003.json"));
}

// [1, 2, [3, 4]]
TEST_F(array_constructorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case004.json"));
}

// [1, \"two\", [\"three\", 4]]
TEST_F(array_constructorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case005.json"));
}

// [1, $two, [\"three\", $four]]
TEST_F(array_constructorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case006.json"));
}

// [\"foo.bar\", foo.bar, [\"foo.baz\", foo.blah.baz]]
TEST_F(array_constructorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case007.json"));
}

// [1, 2, 3][0]
TEST_F(array_constructorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case008.json"));
}

// [1, 2, [3, 4]][-1]
TEST_F(array_constructorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case009.json"));
}

// [1, 2, [3, 4]][-1][-1]
TEST_F(array_constructorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case010.json"));
}

// foo.blah.baz.[fud, fud]
TEST_F(array_constructorTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case011.json"));
}

// foo.blah.baz.[[fud, fud]]
TEST_F(array_constructorTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case012.json"));
}

// foo.blah.[baz].fud
TEST_F(array_constructorTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case013.json"));
}

// foo.blah.[baz, buz].fud
TEST_F(array_constructorTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case014.json"));
}

// [Address, Other.\"Alternative.Address\"].City
TEST_F(array_constructorTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case015.json"));
}

// [Address, Other.`Alternative.Address`].City
TEST_F(array_constructorTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case016.json"));
}

// [0,1,2,3,4,5,6,7,8,9][$ % 2 = 0]
TEST_F(array_constructorTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case017.json"));
}

// [1, 2, 3].$
TEST_F(array_constructorTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case018.json"));
}

// [1, 2, 3].$
TEST_F(array_constructorTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case019.json"));
}

// [1, 2, 3].$
TEST_F(array_constructorTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/array-constructor/case020.json"));
}

