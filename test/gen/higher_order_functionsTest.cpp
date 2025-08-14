#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class higher_order_functionsTest : public JsonataTest {
};

// ($twice:=function($f){function($x){$f($f($x))}}; $add3:=function($y){$y+3}; $add6:=$twice($add3); $add6(7))
TEST_F(higher_order_functionsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/higher-order-functions/case000.json"));
}

// λ($f) { λ($x) { $x($x) }( λ($g) { $f( (λ($a) {$g($g)($a)}))})}(λ($f) { λ($n) { $n < 2 ? 1 : $n * $f($n - 1) } })(6)
TEST_F(higher_order_functionsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/higher-order-functions/case001.json"));
}

// λ($f) { λ($x) { $x($x) }( λ($g) { $f( (λ($a) {$g($g)($a)}))})}(λ($f) { λ($n) { $n <= 1 ? $n : $f($n-1) + $f($n-2) } })(6) 
TEST_F(higher_order_functionsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/higher-order-functions/case002.json"));
}

