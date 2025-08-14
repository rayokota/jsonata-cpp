#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_formatNumberTest : public JsonataTest {
};

// $formatNumber(12345.6, \"#,###.00\")
TEST_F(function_formatNumberTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case000.json"));
}

// $formatNumber(12345678.9, \"9,999.99\")
TEST_F(function_formatNumberTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case001.json"));
}

// $formatNumber(123412345678.9, \"9,9,99.99\")
TEST_F(function_formatNumberTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case002.json"));
}

// $formatNumber(1234.56789, \"9,999.999,999\")
TEST_F(function_formatNumberTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case003.json"));
}

// $formatNumber(123.9, \"9999\")
TEST_F(function_formatNumberTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case004.json"));
}

// $formatNumber(0.14, \"01%\")
TEST_F(function_formatNumberTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case005.json"));
}

// $formatNumber(0.4857,\"###.###‰\")
TEST_F(function_formatNumberTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case006.json"));
}

// $formatNumber(0.14, \"###pm\", {\"per-mille\": \"pm\"})
TEST_F(function_formatNumberTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case007.json"));
}

// $formatNumber(-6, \"000\")
TEST_F(function_formatNumberTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case008.json"));
}

// $formatNumber(1234.5678, \"00.000e0\")
TEST_F(function_formatNumberTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case009.json"));
}

// $formatNumber(1234.5678, \"00.000e000\")
TEST_F(function_formatNumberTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case010.json"));
}

// $formatNumber(1234.5678, \"①①.①①①e①\", {\"zero-digit\": \"⑟\"})
TEST_F(function_formatNumberTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case011.json"));
}

// $formatNumber(0.234, \"0.0e0\")
TEST_F(function_formatNumberTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case012.json"));
}

// $formatNumber(0.234, \"#.00e0\")
TEST_F(function_formatNumberTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case013.json"));
}

// $formatNumber(0.123, \"#.e9\")
TEST_F(function_formatNumberTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case014.json"));
}

// $formatNumber(0.234, \".00e0\")
TEST_F(function_formatNumberTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case015.json"));
}

// $formatNumber(2392.14*(-36.58), \"000,000.000###;###,###.000###\")
TEST_F(function_formatNumberTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case016.json"));
}

// $formatNumber(2.14*86.58,\"PREFIX##00.000###SUFFIX\")
TEST_F(function_formatNumberTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case017.json"));
}

// $formatNumber(1E20,\"#,######\")
TEST_F(function_formatNumberTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case018.json"));
}

// $formatNumber(20,\"#;#;#\")
TEST_F(function_formatNumberTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case019.json"));
}

// $formatNumber(20,\"#.0.0\")
TEST_F(function_formatNumberTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case020.json"));
}

// $formatNumber(20,\"#0%%\")
TEST_F(function_formatNumberTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case021.json"));
}

// $formatNumber(20,\"#0‰‰\")
TEST_F(function_formatNumberTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case022.json"));
}

// $formatNumber(20,\"#0%‰\")
TEST_F(function_formatNumberTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case023.json"));
}

// $formatNumber(20,\".e0\")
TEST_F(function_formatNumberTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case024.json"));
}

// $formatNumber(20,\"0+.e0\")
TEST_F(function_formatNumberTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case025.json"));
}

// $formatNumber(20,\"0,.e0\")
TEST_F(function_formatNumberTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case026.json"));
}

// $formatNumber(20,\"0,\")
TEST_F(function_formatNumberTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case027.json"));
}

// $formatNumber(20,\"0,,0\")
TEST_F(function_formatNumberTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case028.json"));
}

// $formatNumber(20,\"0#.e0\")
TEST_F(function_formatNumberTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case029.json"));
}

// $formatNumber(20,\"#0.#0e0\")
TEST_F(function_formatNumberTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case030.json"));
}

// $formatNumber(20,\"#0.0e0%\")
TEST_F(function_formatNumberTest, case031) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case031.json"));
}

// $formatNumber(20,\"#0.0e0,0\")
TEST_F(function_formatNumberTest, case032) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case032.json"));
}

// $formatNumber(2, '000.000')
TEST_F(function_formatNumberTest, case033) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case033.json"));
}

// $formatNumber(2, 'AAA.AAA', {'zero-digit': 'A'})
TEST_F(function_formatNumberTest, case034) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case034.json"));
}

// $formatNumber(0.1, 'AeAAA', {'zero-digit': 'A'})
TEST_F(function_formatNumberTest, case035) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case035.json"));
}

// $formatNumber(foo, '#0.00')
TEST_F(function_formatNumberTest, case036) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-formatNumber/case036.json"));
}

