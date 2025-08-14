#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_fromMillisTest : public JsonataTest {
};

// $fromMillis(1)
TEST_F(function_fromMillisTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-fromMillis/case000.json"));
}

// $fromMillis(1509380732935)
TEST_F(function_fromMillisTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-fromMillis/case001.json"));
}

// $fromMillis(foo)
TEST_F(function_fromMillisTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-fromMillis/case002.json"));
}

// $fromMillis(undefined, 'undefined')
TEST_F(function_fromMillisTest, formatDateTime_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 0));
}

// $fromMillis(1521801216617, 'Hello')
TEST_F(function_fromMillisTest, formatDateTime_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 1));
}

// $fromMillis(1521801216617, 'Year: [Y0001]')
TEST_F(function_fromMillisTest, formatDateTime_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 2));
}

// $fromMillis(1521801216617, 'Year: <[Y0001]>')
TEST_F(function_fromMillisTest, formatDateTime_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 3));
}

// $fromMillis(1521801216617, 'Year: <[Y9,999,*]>')
TEST_F(function_fromMillisTest, formatDateTime_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 4));
}

// $fromMillis(1521801216617, '[[Year: <[Y0001]>')
TEST_F(function_fromMillisTest, formatDateTime_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 5));
}

// $fromMillis(1521801216617, 'Year]]: <[Y0001]>')
TEST_F(function_fromMillisTest, formatDateTime_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 6));
}

// $fromMillis(1521801216617, '[[Year]]: [[[Y0001]]]')
TEST_F(function_fromMillisTest, formatDateTime_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 7));
}

// $fromMillis(1521801216617, '[D#1]/[M#1]/[Y0001]')
TEST_F(function_fromMillisTest, formatDateTime_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 8));
}

// $fromMillis(1522616700000, '[F0] [FNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 9));
}

// $fromMillis(1522703100000, '[F0] [FNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 10));
}

// $fromMillis(1521801216617, '[Y0001]-[M01]-[D01]')
TEST_F(function_fromMillisTest, formatDateTime_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 11));
}

// $fromMillis(1521801216617, '[M01]/[D01]/[Y0001] at [H01]:[m01]:[s01]')
TEST_F(function_fromMillisTest, formatDateTime_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 12));
}

// $fromMillis(1521801216617, '[Y]-[M01]-[D01]T[H01]:[m]:[s].[f001][Z01:01t]')
TEST_F(function_fromMillisTest, formatDateTime_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 13));
}

// $fromMillis(1521801216617, '[Y]-[ M01]-[D 01]T[H01 ]:[ m   ]:[s].[f0  01][Z01:  01t]')
TEST_F(function_fromMillisTest, formatDateTime_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 14));
}

// $fromMillis(1521801216617, '[Y]-[M01]-[D01]T[H01]:[m]:[s].[f001][Z0101t]', '+0100')
TEST_F(function_fromMillisTest, formatDateTime_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 15));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z01:01]')
TEST_F(function_fromMillisTest, formatDateTime_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 16));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z01:01t]')
TEST_F(function_fromMillisTest, formatDateTime_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 17));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z]', '-0500')
TEST_F(function_fromMillisTest, formatDateTime_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 18));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z0]', '-0500')
TEST_F(function_fromMillisTest, formatDateTime_case_19) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 19));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z]', '+1000')
TEST_F(function_fromMillisTest, formatDateTime_case_20) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 20));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z0]', '+1000')
TEST_F(function_fromMillisTest, formatDateTime_case_21) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 21));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][Z0]', '+0530')
TEST_F(function_fromMillisTest, formatDateTime_case_22) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 22));
}

// $fromMillis(1531310400000, '[Y]-[M01]-[D01]T[H01]:[m]:[s][z]', '-0500')
TEST_F(function_fromMillisTest, formatDateTime_case_23) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 23));
}

// $fromMillis(1204405500000, '[Y]-[M01]-[D01]T[H01]:[m]:[s].[f001][Z0101t]', '+0530')
TEST_F(function_fromMillisTest, formatDateTime_case_24) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 24));
}

// $fromMillis(1230757500000, '[Y]-[M01]-[D01]T[H01]:[m]:[s].[f001][Z0101t]', '+0530')
TEST_F(function_fromMillisTest, formatDateTime_case_25) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 25));
}

// $fromMillis(1230757500000, '[Y]-[M01]-[D01]T[H01]:[m]:[s].[f001][Z010101t]', '+0530')
TEST_F(function_fromMillisTest, formatDateTime_case_26) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 26));
}

// $fromMillis(1521801216617, '[D#1,2]/[M1,2]/[Y,2]')
TEST_F(function_fromMillisTest, formatDateTime_case_27) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 27));
}

// $fromMillis(1521801216617, '[D#1,2]/[M1,2]/[Y0001,2]')
TEST_F(function_fromMillisTest, formatDateTime_case_28) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 28));
}

// $fromMillis(1521801216617, '[D#1,2]/[M1,2]/[Y##01,2-2]')
TEST_F(function_fromMillisTest, formatDateTime_case_29) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 29));
}

// $fromMillis(1521801216617, '[D#1,2]/[M1,2]/[Y0001,2-2]')
TEST_F(function_fromMillisTest, formatDateTime_case_30) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 30));
}

// $fromMillis(1521801216617, '[D1] [M01] [YI]')
TEST_F(function_fromMillisTest, formatDateTime_case_31) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 31));
}

// $fromMillis(1521801216617, '[Da] [MA] [Yi]')
TEST_F(function_fromMillisTest, formatDateTime_case_32) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 32));
}

// $fromMillis(1521801216617, '[D1o] [M01] [Y]')
TEST_F(function_fromMillisTest, formatDateTime_case_33) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 33));
}

// $fromMillis(1521801216617, '[Yw]')
TEST_F(function_fromMillisTest, formatDateTime_case_34) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 34));
}

// $fromMillis(1521801216617, '[Dwo] [M01] [Y]')
TEST_F(function_fromMillisTest, formatDateTime_case_35) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 35));
}

// $fromMillis(1521801216617, '[D1o] [MNn] [Y]')
TEST_F(function_fromMillisTest, formatDateTime_case_36) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 36));
}

// $fromMillis(1521801216617, '[D1o] [MN] [Y]')
TEST_F(function_fromMillisTest, formatDateTime_case_37) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 37));
}

// $fromMillis(1521801216617, '[FNn], [D1o] [MNn] [Y]')
TEST_F(function_fromMillisTest, formatDateTime_case_38) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 38));
}

// $fromMillis(1521801216617, '[FNn], the [Dwo] of [MNn] [Y] [E]')
TEST_F(function_fromMillisTest, formatDateTime_case_39) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 39));
}

// $fromMillis(1521801216617, '[FNn,3-3], [D1o] [MNn,3-3] [Y] [C]')
TEST_F(function_fromMillisTest, formatDateTime_case_40) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 40));
}

// $fromMillis(1521801216617, '[F], [D]/[M]/[Y] [h]:[m]:[s] [P]')
TEST_F(function_fromMillisTest, formatDateTime_case_41) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 41));
}

// $fromMillis(1204405500000, '[F], [D]/[M]/[Y] [h]:[m]:[s] [P]')
TEST_F(function_fromMillisTest, formatDateTime_case_42) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 42));
}

// $fromMillis(1199664000000, '[F], [D]/[M]/[Y] [h]:[m]:[s] [P]')
TEST_F(function_fromMillisTest, formatDateTime_case_43) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 43));
}

// $fromMillis(1514808000000, '[dwo] day of the year')
TEST_F(function_fromMillisTest, formatDateTime_case_44) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 44));
}

// $fromMillis(1546257600000, '[d] days in [Y0001]')
TEST_F(function_fromMillisTest, formatDateTime_case_45) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 45));
}

// $fromMillis(1483185600000, '[d] days in [Y0001]')
TEST_F(function_fromMillisTest, formatDateTime_case_46) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 46));
}

// $fromMillis(1514808000000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_47) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 47));
}

// $fromMillis(1515326400000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_48) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 48));
}

// $fromMillis(1545739200000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_49) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 49));
}

// $fromMillis(1388577600000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_50) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 50));
}

// $fromMillis(1419854400000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_51) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 51));
}

// $fromMillis(1419768000000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_52) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 52));
}

// $fromMillis(1419336000000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_53) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 53));
}

// $fromMillis(1420113600000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_54) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 54));
}

// $fromMillis(1420459200000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_55) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 55));
}

// $fromMillis(1451304000000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_56) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 56));
}

// $fromMillis(1451563200000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_57) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 57));
}

// $fromMillis(1451736000000, 'Week: [W]')
TEST_F(function_fromMillisTest, formatDateTime_case_58) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 58));
}

// $fromMillis(1359460800000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_59) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 59));
}

// $fromMillis(1359633600000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_60) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 60));
}

// $fromMillis(1359720000000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_61) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 61));
}

// $fromMillis(1514808000000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_62) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 62));
}

// $fromMillis(1483272000000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_63) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 63));
}

// $fromMillis(1533038400000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_64) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 64));
}

// $fromMillis(1419940800000, 'Week: [w] of [xNn]')
TEST_F(function_fromMillisTest, formatDateTime_case_65) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 65));
}

// $toMillis('2018-10-21T13:05:00Z').[$fromMillis(), $fromMillis($, (),'-0500'), $fromMillis($, (),'0000')] 
TEST_F(function_fromMillisTest, formatDateTime_case_66) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 66));
}

// $fromMillis(1521801216617, '[F], [D]/[M]/[Y] [h]:[m]:[s] [PN]')
TEST_F(function_fromMillisTest, formatDateTime_case_67) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 67));
}

// $fromMillis(1521801216617, '[F], [D]/[M]/[Y] [h]:[m]:[s] [Pn]')
TEST_F(function_fromMillisTest, formatDateTime_case_68) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 68));
}

// $fromMillis(1419940800000, '[YN]-[M]-[D]')
TEST_F(function_fromMillisTest, formatDateTime_case_69) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 69));
}

// $fromMillis(1419940800000, '[YN]-[M')
TEST_F(function_fromMillisTest, formatDateTime_case_70) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/formatDateTime.json", 70));
}

// ($ts := $toMillis('2005-01-01', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 0));
}

// ($ts := $toMillis('2005-01-02', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 1));
}

// ($ts := $toMillis('2005-12-31', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 2));
}

// ($ts := $toMillis('2006-01-01', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 3));
}

// ($ts := $toMillis('2006-01-02', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 4));
}

// ($ts := $toMillis('2006-12-31', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 5));
}

// ($ts := $toMillis('2007-01-01', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 6));
}

// ($ts := $toMillis('2007-12-30', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 7));
}

// ($ts := $toMillis('2007-12-31', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 8));
}

// ($ts := $toMillis('2008-01-01', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 9));
}

// ($ts := $toMillis('2008-12-28', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 10));
}

// ($ts := $toMillis('2008-12-29', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 11));
}

// ($ts := $toMillis('2008-12-30', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 12));
}

// ($ts := $toMillis('2008-12-31', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 13));
}

// ($ts := $toMillis('2009-01-01', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 14));
}

// ($ts := $toMillis('2009-12-31', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 15));
}

// ($ts := $toMillis('2010-01-01', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 16));
}

// ($ts := $toMillis('2010-01-02', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 17));
}

// ($ts := $toMillis('2010-01-03', '[Y]-[M]-[D]'); $fromMillis($ts, '[X0001]-W[W01]-[F1]') )
TEST_F(function_fromMillisTest, isoWeekDate_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-fromMillis/isoWeekDate.json", 18));
}

