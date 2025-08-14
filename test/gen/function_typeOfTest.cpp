#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_typeOfTest : public JsonataTest {
};

// ($var:= undefined; $type($var))
TEST_F(function_typeOfTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case001.json"));
}

// ($var:= null; $type($var))
TEST_F(function_typeOfTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case002.json"));
}

// ($var:= 123; $type($var))
TEST_F(function_typeOfTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case003.json"));
}

// ($var:= '123'; $type($var))
TEST_F(function_typeOfTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case004.json"));
}

// ($var:= true; $type($var))
TEST_F(function_typeOfTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case005.json"));
}

// ($var:= 'true'; $type($var))
TEST_F(function_typeOfTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case006.json"));
}

// ($var:= []; $type($var))
TEST_F(function_typeOfTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case007.json"));
}

// ($var:= ['a']; $type($var))
TEST_F(function_typeOfTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case008.json"));
}

// ($var:= ['a','b']; $type($var))
TEST_F(function_typeOfTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case009.json"));
}

// ($var:= function(){true}; $type($var))
TEST_F(function_typeOfTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case010.json"));
}

// ($var:= (λ($v, $i, $a) {$v.price = $max($a.price)}); $type($var))
TEST_F(function_typeOfTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case011.json"));
}

// ($var:= {}; $type($var))
TEST_F(function_typeOfTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case012.json"));
}

// ($var:= {\"foo\":\"bar\"}; $type($var))
TEST_F(function_typeOfTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-typeOf/case013.json"));
}

