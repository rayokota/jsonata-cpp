#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_tomillisTest : public JsonataTest {
};

// $toMillis(\"1970-01-01T00:00:00.001Z\")
TEST_F(function_tomillisTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case001.json"));
}

// $toMillis(\"2018-02-01T09:42:13.123+0000\")
TEST_F(function_tomillisTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case002.json"));
}

// $toMillis(\"2017-10-30\")
TEST_F(function_tomillisTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case003.json"));
}

// $toMillis(\"2017-10-30T15:28:34Z\")
TEST_F(function_tomillisTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case004.json"));
}

// $toMillis(\"2018\")
TEST_F(function_tomillisTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case005.json"));
}

// $toMillis(foo)
TEST_F(function_tomillisTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case006.json"));
}

// $toMillis(\"foo\")
TEST_F(function_tomillisTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case007.json"));
}

// $toMillis(\"01-02-2018\")
TEST_F(function_tomillisTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case008.json"));
}

// $toMillis(\"2018-02-03 11:15:33\")
TEST_F(function_tomillisTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case009.json"));
}

// $toMillis(\"201802\", \"[Y0001][M01]\")
TEST_F(function_tomillisTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case010.json"));
}

// $toMillis(\"201802\", \"[Y,*-4][M01]\")
TEST_F(function_tomillisTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case011.json"));
}

// $toMillis(\"20180205\", \"[Y0001][M01][D01]\")
TEST_F(function_tomillisTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case012.json"));
}

// $toMillis('2024-01-01T12:38:49Z') = $toMillis('20240101123849', '[Y0000][M00][D00][H00][m00][s00]')
TEST_F(function_tomillisTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-tomillis/case013.json"));
}

// $toMillis(undefined, 'pic')
TEST_F(function_tomillisTest, parseDateTime_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 0));
}

// $toMillis('Hello', 'Hello')
TEST_F(function_tomillisTest, parseDateTime_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 1));
}

// $toMillis('2018', '[Y1]')
TEST_F(function_tomillisTest, parseDateTime_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 2));
}

// $toMillis('2018-03-27', '[Y1]-[M01]-[D01]')
TEST_F(function_tomillisTest, parseDateTime_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 3));
}

// $toMillis('2018-03-27T14:03:00.123Z', '[Y0001]-[M01]-[D01]T[H01]:[m01]:[s01].[f001]Z')
TEST_F(function_tomillisTest, parseDateTime_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 4));
}

// $toMillis('27th 3 1976', '[D1o] [M#1] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 5));
}

// $toMillis('21st 12 1881', '[D1o] [M01] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 6));
}

// $toMillis('2nd 12 2012', '[D1o] [M01] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 7));
}

// $toMillis('MCMLXXXIV', '[YI]')
TEST_F(function_tomillisTest, parseDateTime_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 8));
}

// $toMillis('27 03 MMXVIII', '[D1] [M01] [YI]')
TEST_F(function_tomillisTest, parseDateTime_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 9));
}

// $toMillis('27 iii MMXVIII', '[D1] [Mi] [YI]')
TEST_F(function_tomillisTest, parseDateTime_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 10));
}

// $toMillis('w C mmxviii', '[Da] [MA] [Yi]')
TEST_F(function_tomillisTest, parseDateTime_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 11));
}

// $toMillis('ae C mmxviii', '[Da] [MA] [Yi]')
TEST_F(function_tomillisTest, parseDateTime_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 12));
}

// $toMillis('27th April 2008', '[D1o] [MNn] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 13));
}

// $toMillis('21 August 2017', '[D1] [MNn] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 14));
}

// $toMillis('2 Feb 2012', '[D1] [MNn,3-3] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 15));
}

// $toMillis('one thousand, nine hundred and eighty-four', '[Yw]')
TEST_F(function_tomillisTest, parseDateTime_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 16));
}

// $toMillis('nineteen hundred and eighty-four', '[Yw]')
TEST_F(function_tomillisTest, parseDateTime_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 17));
}

// $toMillis('twenty-seven April 2008', '[Dw] [MNn] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 18));
}

// $toMillis('twenty-seventh April 2008', '[Dw] [MNn] [Y0001]')
TEST_F(function_tomillisTest, parseDateTime_case_19) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 19));
}

// $toMillis('twenty-first August two thousand and seventeen', '[Dw] [MNn] [Yw]')
TEST_F(function_tomillisTest, parseDateTime_case_20) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 20));
}

// $toMillis('TWENTY-SECOND August two thousand and seventeen', '[DW] [MNn] [Yw]')
TEST_F(function_tomillisTest, parseDateTime_case_21) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 21));
}

// $toMillis('Twentieth of August, two thousand and seventeen', '[DW] of [MNn], [Yw]')
TEST_F(function_tomillisTest, parseDateTime_case_22) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 22));
}

// $toMillis('4/4/2018 12:06 am', '[D1]/[M1]/[Y0001] [h]:[m] [P]')
TEST_F(function_tomillisTest, parseDateTime_case_23) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 23));
}

// $toMillis('4/4/2018 06:30 am', '[D1]/[M1]/[Y0001] [h]:[m] [P]')
TEST_F(function_tomillisTest, parseDateTime_case_24) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 24));
}

// $toMillis('4/4/2018 12:06 pm', '[D1]/[M1]/[Y0001] [h]:[m] [P]')
TEST_F(function_tomillisTest, parseDateTime_case_25) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 25));
}

// $toMillis('4/4/2018 11:30 pm', '[D1]/[M1]/[Y0001] [h]:[m] [P]')
TEST_F(function_tomillisTest, parseDateTime_case_26) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 26));
}

// $toMillis('2018-094', '[Y0001]-[d001]')
TEST_F(function_tomillisTest, parseDateTime_case_27) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 27));
}

// $toMillis('13:45', '[H]:[m]') ~> $fromMillis() ~> $substringBefore('T') = $substringBefore($now(), 'T')
TEST_F(function_tomillisTest, parseDateTime_case_28) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 28));
}

// $toMillis('13:45', '[H]:[m]') ~> $fromMillis() ~> $substringAfter('T')
TEST_F(function_tomillisTest, parseDateTime_case_29) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 29));
}

// $toMillis('Wednesday, 14th November 2018', '[FNn], [D1o] [MNn] [Y]') ~> $fromMillis()
TEST_F(function_tomillisTest, parseDateTime_case_30) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 30));
}

// $toMillis('Mon, Twelfth November 2018', '[FNn,*-3], [DWwo] [MNn] [Y]') ~> $fromMillis()
TEST_F(function_tomillisTest, parseDateTime_case_31) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 31));
}

// $toMillis('2018--180', '[Y]--[d]') ~> $fromMillis()
TEST_F(function_tomillisTest, parseDateTime_case_32) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 32));
}

// $toMillis('three hundred and sixty-fifth day of 2018', '[dwo] day of [Y]') ~> $fromMillis()
TEST_F(function_tomillisTest, parseDateTime_case_33) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 33));
}

// $toMillis('irrelevent string', '[Y]-[M]-[D]')
TEST_F(function_tomillisTest, parseDateTime_case_34) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 34));
}

// $toMillis('2018-05-22', '[Y]-[M]-[q]')
TEST_F(function_tomillisTest, parseDateTime_case_35) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 35));
}

// $toMillis('2018-05-22', '[YN]-[M]-[D]')
TEST_F(function_tomillisTest, parseDateTime_case_36) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 36));
}

// $toMillis('2018-22', '[Y]-[D]')
TEST_F(function_tomillisTest, parseDateTime_case_37) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 37));
}

// $toMillis('5-22 23:59', '[M]-[D] [m]:[s]')
TEST_F(function_tomillisTest, parseDateTime_case_38) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 38));
}

// $toMillis('2018-3-2-5', '[X]-[x]-[w]-[F1]')
TEST_F(function_tomillisTest, parseDateTime_case_39) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 39));
}

// $toMillis('2018-32-5', '[X]-[W]-[F1]')
TEST_F(function_tomillisTest, parseDateTime_case_40) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 40));
}

// $toMillis('2020-09-09 08:00:00 +02:00', '[Y0001]-[M01]-[D01] [H01]:[m01]:[s01] [Z]') ~> $fromMillis() 
TEST_F(function_tomillisTest, parseDateTime_case_41) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 41));
}

// $toMillis('2020-09-09 08:00:00 GMT-05:00', '[Y0001]-[M01]-[D01] [H01]:[m01]:[s01] [z]') ~> $fromMillis() 
TEST_F(function_tomillisTest, parseDateTime_case_42) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 42));
}

// $toMillis('2020-09-09 12:00:00 +05:30', '[Y0001]-[M01]-[D01] [H01]:[m01]:[s01] [Z]') ~> $fromMillis() 
TEST_F(function_tomillisTest, parseDateTime_case_43) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 43));
}

// $toMillis('2020-09-09 12:00:00 GMT-5', '[Y0001]-[M01]-[D01] [H01]:[m01]:[s01] [z01]') ~> $fromMillis() 
TEST_F(function_tomillisTest, parseDateTime_case_44) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 44));
}

// $toMillis('2020-09-09 12:00:00 +0530', '[Y0001]-[M01]-[D01] [H01]:[m01]:[s01] [Z0001]') ~> $fromMillis() 
TEST_F(function_tomillisTest, parseDateTime_case_45) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-tomillis/parseDateTime.json", 45));
}

