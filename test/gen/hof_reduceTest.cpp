#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class hof_reduceTest : public JsonataTest {
};

// (  $seq := [1,2,3,4,5];  $reduce($seq, function($x, $y){$x+$y})) 
TEST_F(hof_reduceTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case000.json"));
}

// (  $concat := function($s){function($a, $b){$string($a) & $s & $string($b)}};  $comma_join := $concat(' ... ');  $reduce([1,2,3,4,5], $comma_join))      
TEST_F(hof_reduceTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case001.json"));
}

// (  $seq := [1,2,3,4,5];  $reduce($seq, function($x, $y){$x+$y}, 2))
TEST_F(hof_reduceTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case002.json"));
}

// (  $seq := 1;  $reduce($seq, function($x, $y){$x+$y}))
TEST_F(hof_reduceTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case003.json"));
}

// (  $seq := 1;  $reduce($seq, function($x, $y){$x+$y}))
TEST_F(hof_reduceTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case004.json"));
}

// $reduce(Account.Order.Product.Quantity, $append)
TEST_F(hof_reduceTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case005.json"));
}

// $reduce(Account.Order.Product.Quantity, $append)
TEST_F(hof_reduceTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case006.json"));
}

// ($product := function($a, $b) { $a * $b };$power := function($x, $n) { $n = 0 ? 1 : $reduce([1..$n].($x), $product) };[0..5].$power(2, $))
TEST_F(hof_reduceTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case007.json"));
}

// (  $seq := 1;  $reduce($seq, function($x){$x}))
TEST_F(hof_reduceTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case008.json"));
}

// 
TEST_F(hof_reduceTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case009.json"));
}

// 
TEST_F(hof_reduceTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-reduce/case010.json"));
}

