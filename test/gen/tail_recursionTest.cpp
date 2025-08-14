#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class tail_recursionTest : public JsonataTest {
};

//             (              $f := function($n){()};              $f(1)            ) 
TEST_F(tail_recursionTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case000.json"));
}

// (  $factorial := function($n){$n = 0 ? 1 : $n * $factorial($n - 1)};  $factorial(99))             
TEST_F(tail_recursionTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case001.json"));
}

// (  $factorial := function($n){$n = 0 ? 1 : $n * $factorial($n - 1)};  $factorial(100))             
TEST_F(tail_recursionTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case002.json"));
}

// (  $factorial := function($n){(    $iter := function($n, $acc) {      $n = 0 ? $acc : $iter($n - 1, $n * $acc)    };    $iter($n, 1)  )};  $factorial(5)) 
TEST_F(tail_recursionTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case003.json"));
}

// (  $factorial := function($n){(    $iter := function($n, $acc) {      $n = 0 ? $acc : $iter($n - 1, $n * $acc)    };    $iter($n, 1)  )};  $factorial(150)) 
TEST_F(tail_recursionTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case004.json"));
}

// (  $inf := function($n){$n+$inf($n-1)};  $inf(5))
TEST_F(tail_recursionTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case005.json"));
}

// (  $inf := function(){$inf()};  $inf())
TEST_F(tail_recursionTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case006.json"));
}

//         (          $even := function($n) { $n = 0 ? true : $odd($n-1) };           $odd := function($n) { $n = 0 ? false : $even($n-1) };           $odd(6555)         )
TEST_F(tail_recursionTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case007.json"));
}

// ($f := function($s, $x)<an:s> { $x > 0 ? $f([$s, $s], $x-1) : $s};  $f('a', 2)  )
TEST_F(tail_recursionTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case008.json"));
}

// ($f := function($s, $x)<sn:s> { $x > 0 ? $f([$s, $s], $x-1) : $s};  $f('a', 2)  )
TEST_F(tail_recursionTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/tail-recursion/case009.json"));
}

