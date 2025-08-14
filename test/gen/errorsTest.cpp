#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class errorsTest : public JsonataTest {
};

// \"s\" - 1
TEST_F(errorsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case000.json"));
}

// 1 + null
TEST_F(errorsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case001.json"));
}

// \"no closing quote
TEST_F(errorsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case002.json"));
}

// `no closing backtick
TEST_F(errorsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case003.json"));
}

// - \"s\"
TEST_F(errorsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case004.json"));
}

// unknown(function)
TEST_F(errorsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case005.json"));
}

// sum(Account.Order.OrderID)
TEST_F(errorsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case006.json"));
}

// [1,2)
TEST_F(errorsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case007.json"));
}

// [1:2]
TEST_F(errorsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case008.json"));
}

// $replace(\"foo\", \"o, \"rr\")
TEST_F(errorsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case009.json"));
}

// [1!2]
TEST_F(errorsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case010.json"));
}

// @ bar
TEST_F(errorsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case011.json"));
}

// 2(blah)
TEST_F(errorsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case012.json"));
}

// 2()
TEST_F(errorsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case013.json"));
}

// 3(?)
TEST_F(errorsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case014.json"));
}

// 1=
TEST_F(errorsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case015.json"));
}

// function(x){$x}(3)
TEST_F(errorsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case016.json"));
}

// x:=1
TEST_F(errorsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case017.json"));
}

// 2:=1
TEST_F(errorsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case018.json"));
}

// $foo()
TEST_F(errorsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case019.json"));
}

// 55=>5
TEST_F(errorsTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case020.json"));
}

// Ssum(:)
TEST_F(errorsTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case021.json"));
}

// [1,2,3]{\"num\": $}[true]
TEST_F(errorsTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case022.json"));
}

// [1,2,3]{\"num\": $}{\"num\": $}
TEST_F(errorsTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case023.json"));
}

// Account.Order[0].Product;
TEST_F(errorsTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case024.json"));
}

// ( $A := function(){$min(2, 3)}; $A() )
TEST_F(errorsTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case025.json"));
}

// ( $B := function(){''}; $A := function(){2 + $B()}; $A() )
TEST_F(errorsTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/errors/case026.json"));
}

