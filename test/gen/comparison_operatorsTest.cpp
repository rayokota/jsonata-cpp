#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class comparison_operatorsTest : public JsonataTest {
};

// 3>-3
TEST_F(comparison_operatorsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case000.json"));
}

// 3>3
TEST_F(comparison_operatorsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case001.json"));
}

// 3=3
TEST_F(comparison_operatorsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case002.json"));
}

// \"3\"=\"3\"
TEST_F(comparison_operatorsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case003.json"));
}

// \"3\"=3
TEST_F(comparison_operatorsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case004.json"));
}

// \"hello\" = \"hello\"
TEST_F(comparison_operatorsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case005.json"));
}

// \"hello\" != \"world\"
TEST_F(comparison_operatorsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case006.json"));
}

// \"hello\" < \"world\"
TEST_F(comparison_operatorsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case007.json"));
}

// \"32\" < 42
TEST_F(comparison_operatorsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case008.json"));
}

// null <= \"world\"
TEST_F(comparison_operatorsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case009.json"));
}

// 3 >= true
TEST_F(comparison_operatorsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case010.json"));
}

// foo.bar > bar
TEST_F(comparison_operatorsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case011.json"));
}

// foo.bar >= bar
TEST_F(comparison_operatorsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case012.json"));
}

// foo.bar<bar
TEST_F(comparison_operatorsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case013.json"));
}

// foo.bar<=bar
TEST_F(comparison_operatorsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case014.json"));
}

// bar>foo.bar
TEST_F(comparison_operatorsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case015.json"));
}

// bar < foo.bar
TEST_F(comparison_operatorsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case016.json"));
}

// foo.bar = bar
TEST_F(comparison_operatorsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case017.json"));
}

// foo.bar!= bar
TEST_F(comparison_operatorsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case018.json"));
}

// bar = foo.bar + 56
TEST_F(comparison_operatorsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case019.json"));
}

// bar !=foo.bar + 56
TEST_F(comparison_operatorsTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case020.json"));
}

// foo.blah.baz[fud = \"hello\"]
TEST_F(comparison_operatorsTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case021.json"));
}

// foo.blah.baz[fud != \"world\"]
TEST_F(comparison_operatorsTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case022.json"));
}

// Account.Order.Product[Price > 30].Price
TEST_F(comparison_operatorsTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case023.json"));
}

// Account.Order.Product.Price[$<=35]
TEST_F(comparison_operatorsTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case024.json"));
}

// false > 1
TEST_F(comparison_operatorsTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case025.json"));
}

// false > $x
TEST_F(comparison_operatorsTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case026.json"));
}

// 3 > $x
TEST_F(comparison_operatorsTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case027.json"));
}

// $x <= 'hello'
TEST_F(comparison_operatorsTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comparison-operators/case028.json"));
}

// [1,2,3,4] = [1, 2, 3, 4]
TEST_F(comparison_operatorsTest, deep_equals_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 0));
}

// [1,2] != [1, 2]
TEST_F(comparison_operatorsTest, deep_equals_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 1));
}

// [1,2,[3],4] = [1, 2, [3], 4]
TEST_F(comparison_operatorsTest, deep_equals_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 2));
}

// [1,2,3,4] = [1, 2, [3], 4]
TEST_F(comparison_operatorsTest, deep_equals_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 3));
}

// [1,2,3,4] = [2, 3, 4]
TEST_F(comparison_operatorsTest, deep_equals_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 4));
}

// data = [1, 2, 3, 4]
TEST_F(comparison_operatorsTest, deep_equals_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 5));
}

// data[2] = 3
TEST_F(comparison_operatorsTest, deep_equals_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 6));
}

// { 'one': 1, 'two': 2 } = { 'two': 2, 'one': 1 }
TEST_F(comparison_operatorsTest, deep_equals_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 7));
}

// { 'one': 1, 'two': 2 } = { 'too': 2, 'one': 1 }
TEST_F(comparison_operatorsTest, deep_equals_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 8));
}

// { 'one': 1, 'two': 3 } = { 'two': 2, 'one': 1 }
TEST_F(comparison_operatorsTest, deep_equals_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 9));
}

// { 'one': 1, 'two': 2, 'three': 3 } = { 'two': 2, 'one': 1 }
TEST_F(comparison_operatorsTest, deep_equals_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 10));
}

// two = 2
TEST_F(comparison_operatorsTest, deep_equals_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 11));
}

// one + one = two
TEST_F(comparison_operatorsTest, deep_equals_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 12));
}

// null = null
TEST_F(comparison_operatorsTest, deep_equals_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 13));
}

// null != null
TEST_F(comparison_operatorsTest, deep_equals_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 14));
}

// null = nothing
TEST_F(comparison_operatorsTest, deep_equals_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 15));
}

// nothing = nothing
TEST_F(comparison_operatorsTest, deep_equals_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 16));
}

// Account.Order[0].Product[0].Description = Account.Order[1].Product[0].Description
TEST_F(comparison_operatorsTest, deep_equals_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 17));
}

// Account.Order[0].Product[0] = Account.Order[1].Product[0]
TEST_F(comparison_operatorsTest, deep_equals_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/comparison-operators/deep-equals.json", 18));
}

