#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_existsTest : public JsonataTest {
};

// $exists(\"Hello World\")
TEST_F(function_existsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case000.json"));
}

// $exists(\"\")
TEST_F(function_existsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case001.json"));
}

// $exists(true)
TEST_F(function_existsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case002.json"));
}

// $exists(false)
TEST_F(function_existsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case003.json"));
}

// $exists(0)
TEST_F(function_existsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case004.json"));
}

// $exists(-0.5)
TEST_F(function_existsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case005.json"));
}

// $exists(null)
TEST_F(function_existsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case006.json"));
}

// $exists([])
TEST_F(function_existsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case007.json"));
}

// $exists([0])
TEST_F(function_existsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case008.json"));
}

// $exists([1,2,3])
TEST_F(function_existsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case009.json"));
}

// $exists([[]])
TEST_F(function_existsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case010.json"));
}

// $exists([[null]])
TEST_F(function_existsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case011.json"));
}

// $exists([[[true]]])
TEST_F(function_existsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case012.json"));
}

// $exists({})
TEST_F(function_existsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case013.json"));
}

// $exists({\"hello\":\"world\"})
TEST_F(function_existsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case014.json"));
}

// $exists(Account)
TEST_F(function_existsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case015.json"));
}

// $exists(Account.Order.Product.Price)
TEST_F(function_existsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case016.json"));
}

// $exists($exists)
TEST_F(function_existsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case017.json"));
}

// $exists(function(){true})
TEST_F(function_existsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case018.json"));
}

// $exists(blah)
TEST_F(function_existsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case019.json"));
}

// $exists(Account.blah)
TEST_F(function_existsTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case020.json"));
}

// $exists(Account.Order[2])
TEST_F(function_existsTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case021.json"));
}

// $exists(Account.Order[0].blah)
TEST_F(function_existsTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case022.json"));
}

// $exists(2,3)
TEST_F(function_existsTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case023.json"));
}

// $exists()
TEST_F(function_existsTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-exists/case024.json"));
}

