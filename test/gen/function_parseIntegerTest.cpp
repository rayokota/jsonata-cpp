#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_parseIntegerTest : public JsonataTest {
};

// $parseInteger(undefined, '0')
TEST_F(function_parseIntegerTest, parseInteger_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 0));
}

// $parseInteger('123', '000')
TEST_F(function_parseIntegerTest, parseInteger_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 1));
}

// $parseInteger('0123', '0000')
TEST_F(function_parseIntegerTest, parseInteger_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 2));
}

// $parseInteger('1234', '0')
TEST_F(function_parseIntegerTest, parseInteger_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 3));
}

// $parseInteger('1234', '###0')
TEST_F(function_parseIntegerTest, parseInteger_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 4));
}

// $parseInteger('12', '###0')
TEST_F(function_parseIntegerTest, parseInteger_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 5));
}

// $parseInteger('123rd', '000;o')
TEST_F(function_parseIntegerTest, parseInteger_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 6));
}

// $parseInteger('1st', '0;o')
TEST_F(function_parseIntegerTest, parseInteger_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 7));
}

// $parseInteger('28th', '#0;o')
TEST_F(function_parseIntegerTest, parseInteger_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 8));
}

// $parseInteger('1234th', '###0;o')
TEST_F(function_parseIntegerTest, parseInteger_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 9));
}

// $parseInteger('12th', '###0;o')
TEST_F(function_parseIntegerTest, parseInteger_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 10));
}

// $parseInteger('١٢٣٤٠', '###١')
TEST_F(function_parseIntegerTest, parseInteger_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 11));
}

// $parseInteger('１２３４０', '###０')
TEST_F(function_parseIntegerTest, parseInteger_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 12));
}

// $parseInteger('12', '#,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 13));
}

// $parseInteger('1,200', '#,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 14));
}

// $parseInteger('12,345,678', '#,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 15));
}

// $parseInteger('1,234,567,890', '#,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_16) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 16));
}

// $parseInteger('1,234,567,890', '#,###,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_17) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 17));
}

// $parseInteger('1234:567,890', '#:###,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_18) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 18));
}

// $parseInteger('12345,67,890', '##,##,##0')
TEST_F(function_parseIntegerTest, parseInteger_case_19) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 19));
}

// $parseInteger('', 'I')
TEST_F(function_parseIntegerTest, parseInteger_case_20) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 20));
}

// $parseInteger('MCMLXXXIV', 'I')
TEST_F(function_parseIntegerTest, parseInteger_case_21) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 21));
}

// $parseInteger('xcix', 'i')
TEST_F(function_parseIntegerTest, parseInteger_case_22) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 22));
}

// $parseInteger('twelve', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_23) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 23));
}

// $parseInteger('twenty', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_24) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 24));
}

// $parseInteger('thirty-four', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_25) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 25));
}

// $parseInteger('NINETY-NINE', 'W')
TEST_F(function_parseIntegerTest, parseInteger_case_26) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 26));
}

// $parseInteger('one hundred', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_27) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 27));
}

// $parseInteger('FIVE HUNDRED AND FIFTY-FIVE', 'W')
TEST_F(function_parseIntegerTest, parseInteger_case_28) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 28));
}

// $parseInteger('Five Hundred and Fifty-Five', 'Ww')
TEST_F(function_parseIntegerTest, parseInteger_case_29) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 29));
}

// $parseInteger('nine hundred and nineteen', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_30) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 30));
}

// $parseInteger('seven hundred and thirty', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_31) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 31));
}

// $parseInteger('one thousand', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_32) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 32));
}

// $parseInteger('three thousand, seven hundred and thirty', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_33) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 33));
}

// $parseInteger('three hundred and twenty-seven thousand, seven hundred and thirty', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_34) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 34));
}

// $parseInteger('four million, three hundred and twenty-seven thousand, seven hundred and thirty', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_35) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 35));
}

// $parseInteger('one trillion and one', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_36) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 36));
}

// $parseInteger('one trillion, two hundred and thirty-four billion, five hundred and sixty-seven million, eight hundred and ninety thousand, one hundred and twenty-three', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_37) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 37));
}

// $parseInteger('one thousand trillion', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_38) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 38));
}

// $parseInteger('ten billion trillion trillion trillion', 'w')
TEST_F(function_parseIntegerTest, parseInteger_case_39) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 39));
}

// $parseInteger('twelfth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_40) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 40));
}

// $parseInteger('twentieth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_41) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 41));
}

// $parseInteger('thirty-fourth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_42) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 42));
}

// $parseInteger('NINETY-NINTH', 'W;o')
TEST_F(function_parseIntegerTest, parseInteger_case_43) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 43));
}

// $parseInteger('one hundredth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_44) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 44));
}

// $parseInteger('FIVE HUNDRED AND FIFTY-FIFTH', 'W;o')
TEST_F(function_parseIntegerTest, parseInteger_case_45) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 45));
}

// $parseInteger('nine hundred and nineteenth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_46) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 46));
}

// $parseInteger('seven hundred and thirtieth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_47) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 47));
}

// $parseInteger('Seven Hundred and Thirty-Third', 'Ww;o')
TEST_F(function_parseIntegerTest, parseInteger_case_48) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 48));
}

// $parseInteger('one thousandth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_49) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 49));
}

// $parseInteger('three thousand, seven hundred and thirty-first', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_50) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 50));
}

// $parseInteger('three hundred and twenty-seven thousand, seven hundred and thirteenth', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_51) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 51));
}

// $parseInteger('four million, three hundred and twenty-seven thousand, seven hundred and thirty-second', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_52) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 52));
}

// $parseInteger('one trillion and first', 'w;o')
TEST_F(function_parseIntegerTest, parseInteger_case_53) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 53));
}

// $parseInteger('A', 'A')
TEST_F(function_parseIntegerTest, parseInteger_case_54) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 54));
}

// $parseInteger('l', 'a')
TEST_F(function_parseIntegerTest, parseInteger_case_55) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 55));
}

// $parseInteger('z', 'a')
TEST_F(function_parseIntegerTest, parseInteger_case_56) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 56));
}

// $parseInteger('aa', 'a')
TEST_F(function_parseIntegerTest, parseInteger_case_57) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 57));
}

// $parseInteger('KN', 'A')
TEST_F(function_parseIntegerTest, parseInteger_case_58) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 58));
}

// $parseInteger('FZPH', 'A')
TEST_F(function_parseIntegerTest, parseInteger_case_59) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 59));
}

// $parseInteger('50', '#')
TEST_F(function_parseIntegerTest, parseInteger_case_60) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/function-parseInteger/parseInteger.json", 60));
}

