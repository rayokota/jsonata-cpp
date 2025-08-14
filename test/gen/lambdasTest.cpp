#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class lambdasTest : public JsonataTest {
};

// function($x){$x*$x}(5)
TEST_F(lambdasTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case000.json"));
}

// function($x){$x>5 ? \"foo\"}(6)
TEST_F(lambdasTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case001.json"));
}

// function($x){$x>5 ? \"foo\"}(3)
TEST_F(lambdasTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case002.json"));
}

// ($factorial:= function($x){$x <= 1 ? 1 : $x * $factorial($x-1)}; $factorial(4))
TEST_F(lambdasTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case003.json"));
}

// ($fibonacci := function($x){$x <= 1 ? $x : $fibonacci($x-1) + $fibonacci($x-2)}; [1,2,3,4,5,6,7,8,9].$fibonacci($))
TEST_F(lambdasTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case004.json"));
}

// ($nth_price := function($n) { (Account.Order.Product.Price)[$n] }; $nth_price(1) )
TEST_F(lambdasTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case005.json"));
}

//             (              $even := function($n) { $n = 0 ? true : $odd($n-1) };              $odd := function($n) { $n = 0 ? false : $even($n-1) };              $even(82)            )
TEST_F(lambdasTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case006.json"));
}

//             (              $even := function($n) { $n = 0 ? true : $odd($n-1) };               $odd := function($n) { $n = 0 ? false : $even($n-1) };               $even(65) )
TEST_F(lambdasTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case007.json"));
}

//         (          $even := function($n) { $n = 0 ? true : $odd($n-1) };           $odd := function($n) { $n = 0 ? false : $even($n-1) };           $odd(65)         )
TEST_F(lambdasTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case008.json"));
}

// (  $gcd := λ($a, $b){$b = 0 ? $a : $gcd($b, $a%$b) };  [$gcd(8,12), $gcd(9,12)])
TEST_F(lambdasTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case009.json"));
}

// (  $range := function($start, $end, $step) { (    $step:=($step?$step:1);    $start+$step > $end ? $start : $append($start, $range($start+$step, $end, $step))   )};  $range(0,15))        
TEST_F(lambdasTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case010.json"));
}

// (  $range := function($start, $end, $step) { (      $step:=($step?$step:1);        $start+$step > $end ? $start : $append($start, $range($start+$step, $end, $step))   )};  $range(0,15,2))        
TEST_F(lambdasTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case011.json"));
}

// ( $median := λ($x){$sort($x)[$count($x)/2]};  $median([5,4,3,2,1,6,7,8,1]) )
TEST_F(lambdasTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case012.json"));
}

// ( $f := function(){1}; $f.environment )
TEST_F(lambdasTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/lambdas/case013.json"));
}

