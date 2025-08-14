#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_applicationsTest : public JsonataTest {
};

// Account.Order[0].OrderID ~> $uppercase()
TEST_F(function_applicationsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case000.json"));
}

// Account.Order[0].OrderID ~> $uppercase() ~> $lowercase()
TEST_F(function_applicationsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case001.json"));
}

// Account.Order.OrderID ~> $join()
TEST_F(function_applicationsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case002.json"));
}

// Account.Order.OrderID ~> $join(\", \")
TEST_F(function_applicationsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case003.json"));
}

// Account.Order.Product.(Price * Quantity) ~> $sum()
TEST_F(function_applicationsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case004.json"));
}

// ( $uppertrim := $trim ~> $uppercase;  $uppertrim(\"   Hello    World   \") )
TEST_F(function_applicationsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case005.json"));
}

// \"john@example.com\" ~> $substringAfter(\"@\") ~> $substringBefore(\".\") 
TEST_F(function_applicationsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case006.json"));
}

// \"\" ~> $substringAfter(\"@\") ~> $substringBefore(\".\") 
TEST_F(function_applicationsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case007.json"));
}

// foo ~> $substringAfter(\"@\") ~> $substringBefore(\".\") 
TEST_F(function_applicationsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case008.json"));
}

// ( $domain := $substringAfter(?,\"@\") ~> $substringBefore(?,\".\"); $domain(\"john@example.com\") )
TEST_F(function_applicationsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case009.json"));
}

// ( $square := function($x){$x*$x}; [1..5] ~> $map($square) ) 
TEST_F(function_applicationsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case010.json"));
}

// ( $square := function($x){$x*$x}; [1..5] ~> $map($square) ~> $sum() ) 
TEST_F(function_applicationsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case011.json"));
}

// ($betweenBackets := $substringAfter(?, \"(\") ~> $substringBefore(?, \")\");$betweenBackets(\"test(foo)bar\")) 
TEST_F(function_applicationsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case012.json"));
}

// ($square := function($x){$x*$x};$chain := λ($f, $g){λ($x){$g($f($x))}};$instructions := [$sum, $square];$sumsq := $instructions ~> $reduce($chain);[1..5] ~> $sumsq()) 
TEST_F(function_applicationsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case013.json"));
}

// ($square := function($x){$x*$x};$chain := λ($f, $g){λ($x){ $x ~> $f ~> $g }};$instructions := [$sum, $square, $string];$sumsq := $instructions ~> $reduce($chain);[1..5] ~> $sumsq()) 
TEST_F(function_applicationsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case014.json"));
}

// ($square := function($x){$x*$x};$instructions := $sum ~> $square;[1..5] ~> $instructions())  
TEST_F(function_applicationsTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case015.json"));
}

// ($square := function($x){$x*$x};$sum_of_squares := $map(?, $square) ~> $sum;[1..5] ~> $sum_of_squares())  
TEST_F(function_applicationsTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case016.json"));
}

// ($times := λ($x, $y) { $x * $y };$product := $reduce(?, $times);$square := function($x){$x*$x};$product_of_squares := $map(?, $square) ~> $product;[1..5] ~> $product_of_squares())
TEST_F(function_applicationsTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case017.json"));
}

// ($square := function($x){$x*$x};[1..5] ~> $map($square) ~> $reduce(λ($x, $y) { $x * $y });)
TEST_F(function_applicationsTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case018.json"));
}

// ($prices := Account.Order.Product.Price;$quantities := Account.Order.Product.Quantity;$product := λ($arr) { $arr[0] * $arr[1] };$zip($prices, $quantities) ~> $map($product) ~> $sum())
TEST_F(function_applicationsTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case019.json"));
}

// 42 ~> \"hello\"
TEST_F(function_applicationsTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case020.json"));
}

// Account.Order.Product[$.\"Product Name\" ~> /hat/i].ProductID
TEST_F(function_applicationsTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-applications/case021.json"));
}

