#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_formatIntegerTest : public JsonataTest {
};

// $formatInteger(undefined, '0')
TEST_F(function_formatIntegerTest, formatInteger_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 0));
}

// $formatInteger(123, '000')
TEST_F(function_formatIntegerTest, formatInteger_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 1));
}

// $formatInteger(123, '0000')
TEST_F(function_formatIntegerTest, formatInteger_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 2));
}

// $formatInteger(-3, '0000')
TEST_F(function_formatIntegerTest, formatInteger_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 3));
}

// $formatInteger(1234, '0;c')
TEST_F(function_formatIntegerTest, formatInteger_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 4));
}

// $formatInteger(1234, '###0')
TEST_F(function_formatIntegerTest, formatInteger_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 5));
}

// $formatInteger(12, '###0')
TEST_F(function_formatIntegerTest, formatInteger_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 6));
}

// $formatInteger(12.6, '###0')
TEST_F(function_formatIntegerTest, formatInteger_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 7));
}

// $formatInteger(123, '000;o')
TEST_F(function_formatIntegerTest, formatInteger_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 8));
}

// $formatInteger(1, '0;o')
TEST_F(function_formatIntegerTest, formatInteger_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 9));
}

// $formatInteger(28, '#0;o')
TEST_F(function_formatIntegerTest, formatInteger_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 10));
}

// $formatInteger(1234, '###0;o')
TEST_F(function_formatIntegerTest, formatInteger_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 11));
}

// $formatInteger(12, '###0;o')
TEST_F(function_formatIntegerTest, formatInteger_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 12));
}

// $formatInteger(12340, '###١')
TEST_F(function_formatIntegerTest, formatInteger_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 13));
}

// $formatInteger(12340, '###０')
TEST_F(function_formatIntegerTest, formatInteger_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 14));
}

// $formatInteger(12340, '##0０')
TEST_F(function_formatIntegerTest, formatInteger_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 15));
}

// $formatInteger(12, '#,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 16));
}

// $formatInteger(1200, '#,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 17));
}

// $formatInteger(120000, '#,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 18));
}

// $formatInteger(12345678, '#,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_19) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 19));
}

// $formatInteger(1234567890, '#,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_20) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 20));
}

// $formatInteger(1234567890, '#,###,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_21) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 21));
}

// $formatInteger(1234567890, '#:###,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_22) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 22));
}

// $formatInteger(1234567890, '##,##,##0')
TEST_F(function_formatIntegerTest, formatInteger_case_23) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 23));
}

// $formatInteger(0, 'I')
TEST_F(function_formatIntegerTest, formatInteger_case_24) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 24));
}

// $formatInteger(1984, 'I')
TEST_F(function_formatIntegerTest, formatInteger_case_25) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 25));
}

// $formatInteger(99, 'i')
TEST_F(function_formatIntegerTest, formatInteger_case_26) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 26));
}

// $formatInteger(12, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_27) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 27));
}

// $formatInteger(20, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_28) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 28));
}

// $formatInteger(34, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_29) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 29));
}

// $formatInteger(99, 'W')
TEST_F(function_formatIntegerTest, formatInteger_case_30) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 30));
}

// $formatInteger(100, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_31) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 31));
}

// $formatInteger(555, 'W')
TEST_F(function_formatIntegerTest, formatInteger_case_32) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 32));
}

// $formatInteger(555, 'Ww')
TEST_F(function_formatIntegerTest, formatInteger_case_33) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 33));
}

// $formatInteger(919, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_34) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 34));
}

// $formatInteger(730, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_35) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 35));
}

// $formatInteger(1000, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_36) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 36));
}

// $formatInteger(3730, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_37) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 37));
}

// $formatInteger(327730, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_38) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 38));
}

// $formatInteger(4327730, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_39) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 39));
}

// $formatInteger(1000000000001, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_40) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 40));
}

// $formatInteger(1234567890123, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_41) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 41));
}

// $formatInteger(1000000000000000, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_42) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 42));
}

// $formatInteger(1e+46, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_43) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 43));
}

// $formatInteger(1234567890123456, 'w')
TEST_F(function_formatIntegerTest, formatInteger_case_44) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 44));
}

// $formatInteger(12, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_45) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 45));
}

// $formatInteger(20, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_46) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 46));
}

// $formatInteger(34, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_47) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 47));
}

// $formatInteger(99, 'W;o')
TEST_F(function_formatIntegerTest, formatInteger_case_48) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 48));
}

// $formatInteger(100, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_49) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 49));
}

// $formatInteger(555, 'W;o')
TEST_F(function_formatIntegerTest, formatInteger_case_50) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 50));
}

// $formatInteger(919, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_51) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 51));
}

// $formatInteger(730, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_52) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 52));
}

// $formatInteger(1000, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_53) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 53));
}

// $formatInteger(3731, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_54) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 54));
}

// $formatInteger(327713, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_55) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 55));
}

// $formatInteger(4327732, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_56) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 56));
}

// $formatInteger(1000000000001, 'w;o')
TEST_F(function_formatIntegerTest, formatInteger_case_57) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 57));
}

// $formatInteger(1, 'A')
TEST_F(function_formatIntegerTest, formatInteger_case_58) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 58));
}

// $formatInteger(12, 'a')
TEST_F(function_formatIntegerTest, formatInteger_case_59) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 59));
}

// $formatInteger(26, 'a')
TEST_F(function_formatIntegerTest, formatInteger_case_60) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 60));
}

// $formatInteger(27, 'a')
TEST_F(function_formatIntegerTest, formatInteger_case_61) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 61));
}

// $formatInteger(300, 'A')
TEST_F(function_formatIntegerTest, formatInteger_case_62) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 62));
}

// $formatInteger(123456, 'A')
TEST_F(function_formatIntegerTest, formatInteger_case_63) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 63));
}

// $formatInteger(123456, 'α')
TEST_F(function_formatIntegerTest, formatInteger_case_64) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-formatInteger/formatInteger.json", 64));
}

