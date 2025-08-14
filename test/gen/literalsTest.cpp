#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class literalsTest : public JsonataTest {
};

// {'fgFoo:0:hausnummer': {'type': 'de.example.api.PossibleValuesList', 'value': [{'label': '', 'value': ''}] }}
TEST_F(literalsTest, array_inputs_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/literals/array-inputs.json", 0));
}

// {'fgFoo:0:hausnummer': {'type': 'de.example.api.PossibleValuesList', 'value': [{'label': '', 'value': ''}] }}
TEST_F(literalsTest, array_inputs_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/literals/array-inputs.json", 1));
}

// {'fgFoo:0:hausnummer': {'type': 'de.example.api.PossibleValuesList', 'value': [{'label': '', 'value': ''}] }}
TEST_F(literalsTest, array_inputs_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/literals/array-inputs.json", 2));
}

// {'fgFoo:0:hausnummer': {'type': 'de.example.api.PossibleValuesList', 'value': [{'label': '', 'value': ''}] }}
TEST_F(literalsTest, array_inputs_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/literals/array-inputs.json", 3));
}

// \"hello\"
TEST_F(literalsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case000.json"));
}

// 'hello'
TEST_F(literalsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case001.json"));
}

// \"Wayne's World\"
TEST_F(literalsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case002.json"));
}

// 42
TEST_F(literalsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case003.json"));
}

// -42
TEST_F(literalsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case004.json"));
}

// 3.14159
TEST_F(literalsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case005.json"));
}

// 6.022e23
TEST_F(literalsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case006.json"));
}

// 1.602E-19
TEST_F(literalsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case007.json"));
}

// 10e1000
TEST_F(literalsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case008.json"));
}

// \"hello\\tworld\"
TEST_F(literalsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case009.json"));
}

// \"hello\\nworld\"
TEST_F(literalsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case010.json"));
}

// \"hello \\\"world\\\"\"
TEST_F(literalsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case011.json"));
}

// \"C:\\\\Test\\\\test.txt\"
TEST_F(literalsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case012.json"));
}

// \"\\u03BB-calculus rocks\"
TEST_F(literalsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case013.json"));
}

// \"𝄞\"
TEST_F(literalsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case014.json"));
}

// \"\\y\"
TEST_F(literalsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case015.json"));
}

// \"\\u\"
TEST_F(literalsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case016.json"));
}

// \"\\u123t\"
TEST_F(literalsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case017.json"));
}

// { 'foo': { 'sequence': 123, 'bar': 'baz' } } 
TEST_F(literalsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case018.json"));
}

// { 'foo': { 'sequence': true, 'bar': 'baz' } } 
TEST_F(literalsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/literals/case019.json"));
}

