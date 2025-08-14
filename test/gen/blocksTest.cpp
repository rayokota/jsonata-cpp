#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class blocksTest : public JsonataTest {
};

// ()
TEST_F(blocksTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case000.json"));
}

// (1; 2; 3)
TEST_F(blocksTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case001.json"));
}

// (1; 2; 3;)
TEST_F(blocksTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case002.json"));
}

// ($a:=1; $b:=2; $c:=($a:=4; $a+$b); $a+$c)
TEST_F(blocksTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case003.json"));
}

// Account.Order.Product.($var1 := Price ; $var2:=Quantity; $var1 * $var2)
TEST_F(blocksTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case004.json"));
}

// (  $func := function($arg) {$arg.Account.Order[0].OrderID};  $func($))
TEST_F(blocksTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case005.json"));
}

// (  $func := function($arg) {$arg.Account.Order[0]};  $func($).OrderID)
TEST_F(blocksTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/blocks/case006.json"));
}

