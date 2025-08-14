#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_stringTest : public JsonataTest {
};

// $string(5)
TEST_F(function_stringTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case000.json"));
}

// $string(22/7)
TEST_F(function_stringTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case001.json"));
}

// $string(1e100)
TEST_F(function_stringTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case002.json"));
}

// $string(1e-100)
TEST_F(function_stringTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case003.json"));
}

// $string(1e-6)
TEST_F(function_stringTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case004.json"));
}

// $string(1e-7)
TEST_F(function_stringTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case005.json"));
}

// $string(1e20)
TEST_F(function_stringTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case006.json"));
}

// $string(1e21)
TEST_F(function_stringTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case007.json"));
}

// Account.Order.$string($sum(Product.(Price* Quantity)))
TEST_F(function_stringTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case008.json"));
}

// $string(true)
TEST_F(function_stringTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case009.json"));
}

// $string(false)
TEST_F(function_stringTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case010.json"));
}

// $string(null)
TEST_F(function_stringTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case011.json"));
}

// $string(blah)
TEST_F(function_stringTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case012.json"));
}

// $string($string)
TEST_F(function_stringTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case013.json"));
}

// $string(function(){true})
TEST_F(function_stringTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case014.json"));
}

// $string(function(){1})
TEST_F(function_stringTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case015.json"));
}

// $string({\"string\": \"hello\"})
TEST_F(function_stringTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case016.json"));
}

// $string([\"string\", 5])
TEST_F(function_stringTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case017.json"));
}

// $string({  \"string\": \"hello\",  \"number\": 78.8 / 2,  \"null\":null,  \"boolean\": false,  \"function\": $sum,  \"lambda\": function(){true},  \"object\": {    \"str\": \"another\",    \"lambda2\": function($n){$n}  },  \"array\": []})
TEST_F(function_stringTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case018.json"));
}

// $string(1/0)
TEST_F(function_stringTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case019.json"));
}

// $string({\"inf\": 1/0})
TEST_F(function_stringTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case020.json"));
}

// $string(2,3)
TEST_F(function_stringTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case021.json"));
}

// $string()
TEST_F(function_stringTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case022.json"));
}

// $string({\"string\": \"hello\"}, false)
TEST_F(function_stringTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case023.json"));
}

// $string({\"string\": \"hello\"}, true)
TEST_F(function_stringTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case024.json"));
}

// $string([\"string\", 5], false)
TEST_F(function_stringTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case025.json"));
}

// $string([\"string\", 5], true)
TEST_F(function_stringTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case026.json"));
}

// $string({  \"string\": \"hello\",  \"number\": 78.8 / 2,  \"null\":null,  \"boolean\": false,  \"function\": $sum,  \"lambda\": function(){true},  \"object\": {    \"str\": \"another\",    \"lambda2\": function($n){$n}  },  \"array\": []}, false)
TEST_F(function_stringTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case027.json"));
}

// $string({  \"string\": \"hello\",  \"number\": 78.8 / 2,  \"null\":null,  \"boolean\": false,  \"function\": $sum,  \"lambda\": function(){true},  \"object\": {    \"str\": \"another\",    \"lambda2\": function($n){$n}  },  \"array\": []}, true)
TEST_F(function_stringTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case028.json"));
}

// $string()
TEST_F(function_stringTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-string/case029.json"));
}

