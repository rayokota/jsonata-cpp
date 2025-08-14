#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class range_operatorTest : public JsonataTest {
};

// [0..9]
TEST_F(range_operatorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case000.json"));
}

// [0..9][$ % 2 = 0]
TEST_F(range_operatorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case001.json"));
}

// [0, 4..9, 20, 22]
TEST_F(range_operatorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case002.json"));
}

// [5..2]
TEST_F(range_operatorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case003.json"));
}

// [5..2, 2..5]
TEST_F(range_operatorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case004.json"));
}

// [-2..2]
TEST_F(range_operatorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case005.json"));
}

// [-2..2].($*$)
TEST_F(range_operatorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case006.json"));
}

// [-2..blah]
TEST_F(range_operatorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case007.json"));
}

// [blah..5, 3, -2..blah]
TEST_F(range_operatorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case008.json"));
}

// [1.1 .. 5]
TEST_F(range_operatorTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case009.json"));
}

// [1 .. 5.5]
TEST_F(range_operatorTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case010.json"));
}

// [10..1.5]
TEST_F(range_operatorTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case011.json"));
}

// [true..false]
TEST_F(range_operatorTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case012.json"));
}

// ['dogs'..'cats']
TEST_F(range_operatorTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case013.json"));
}

// [1..'']
TEST_F(range_operatorTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case014.json"));
}

// [1..[]]
TEST_F(range_operatorTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case015.json"));
}

// [1..{}]
TEST_F(range_operatorTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case016.json"));
}

// [1..false]
TEST_F(range_operatorTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case017.json"));
}

// [2..true]
TEST_F(range_operatorTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case018.json"));
}

// [$x..true]
TEST_F(range_operatorTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case019.json"));
}

// [false..$x]
TEST_F(range_operatorTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case020.json"));
}

// [1..10000000] ~> $count()
TEST_F(range_operatorTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case021.json"));
}

// [0..10000000] ~> $count()
TEST_F(range_operatorTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case022.json"));
}

// [1..10000001] ~> $count()
TEST_F(range_operatorTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case023.json"));
}

// [100..10000099] ~> $count()
TEST_F(range_operatorTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/range-operator/case024.json"));
}

