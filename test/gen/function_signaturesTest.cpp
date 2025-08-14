#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_signaturesTest : public JsonataTest {
};

// λ($arg)<b:b>{$not($arg)}(true)
TEST_F(function_signaturesTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case000.json"));
}

// λ($arg)<b:b>{$not($arg)}(foo)
TEST_F(function_signaturesTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case001.json"));
}

// λ($arg)<x:b>{$not($arg)}(null)
TEST_F(function_signaturesTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case002.json"));
}

// function($x,$y)<n-n:n>{$x+$y}(2, 6)
TEST_F(function_signaturesTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case003.json"));
}

// [1..5].function($x,$y)<n-n:n>{$x+$y}(6)
TEST_F(function_signaturesTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case004.json"));
}

// [1..5].function($x,$y)<n-n:n>{$x+$y}(2, 6)
TEST_F(function_signaturesTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case005.json"));
}

// Age.function($x,$y)<n-n:n>{$x+$y}(6)
TEST_F(function_signaturesTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case006.json"));
}

// λ($str)<s->{$uppercase($str)}(\"hello\")
TEST_F(function_signaturesTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case007.json"));
}

// Account.Order.Product.Description.Colour.λ($str)<s->{$uppercase($str)}()
TEST_F(function_signaturesTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case008.json"));
}

// λ($str, $prefix)<s-s>{$prefix & $str}(\"World\", \"Hello \")
TEST_F(function_signaturesTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case009.json"));
}

// FirstName.λ($str, $prefix)<s-s>{$prefix & $str}(\"Hello \")
TEST_F(function_signaturesTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case010.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}(\"a\")
TEST_F(function_signaturesTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case011.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}([\"a\"])
TEST_F(function_signaturesTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case012.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}(\"a\", \"-\")
TEST_F(function_signaturesTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case013.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}([\"a\"], \"-\")
TEST_F(function_signaturesTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case014.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}([\"a\", \"b\"], \"-\")
TEST_F(function_signaturesTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case015.json"));
}

// λ($arr, $sep)<as?:s>{$join($arr, $sep)}([\"a\", \"b\"], \"-\")
TEST_F(function_signaturesTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case016.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}([], \"-\")
TEST_F(function_signaturesTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case017.json"));
}

// λ($arr, $sep)<a<s>s?:s>{$join($arr, $sep)}(foo, \"-\")
TEST_F(function_signaturesTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case018.json"));
}

// λ($obj)<o>{$obj}({\"hello\": \"world\"})
TEST_F(function_signaturesTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case019.json"));
}

// λ($arr)<a<a<n>>>{$arr}([[1]])
TEST_F(function_signaturesTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case020.json"));
}

// λ($num)<(ns)-:n>{$number($num)}(5)
TEST_F(function_signaturesTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case021.json"));
}

// λ($num)<(ns)-:n>{$number($num)}(\"5\")
TEST_F(function_signaturesTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case022.json"));
}

// [1..5].λ($num)<(ns)-:n>{$number($num)}()
TEST_F(function_signaturesTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case023.json"));
}

// ($twice := function($f)<f:f>{function($x)<n:n>{$f($f($x))}};$add2 := function($x)<n:n>{$x+2};$add4 := $twice($add2);$add4(5))
TEST_F(function_signaturesTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case024.json"));
}

// ($twice := function($f)<f<n:n>:f<n:n>>{function($x)<n:n>{$f($f($x))}};$add2 := function($x)<n:n>{$x+2};$add4 := $twice($add2);$add4(5))
TEST_F(function_signaturesTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case025.json"));
}

// λ($arg)<n<n>>{$arg}(5)
TEST_F(function_signaturesTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case026.json"));
}

// λ($arg1, $arg2)<nn:a>{[$arg1, $arg2]}(1,\"2\")
TEST_F(function_signaturesTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case027.json"));
}

// λ($arg1, $arg2)<nn:a>{[$arg1, $arg2]}(1,3,\"2\")
TEST_F(function_signaturesTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case028.json"));
}

// λ($arg1, $arg2)<nn+:a>{[$arg1, $arg2]}(1,3, 2,\"g\")
TEST_F(function_signaturesTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case029.json"));
}

// λ($arr)<a<n>>{$arr}([\"3\"]) 
TEST_F(function_signaturesTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case030.json"));
}

// λ($arr)<a<n>>{$arr}([1, 2, \"3\"]) 
TEST_F(function_signaturesTest, case031) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case031.json"));
}

// λ($arr)<a<n>>{$arr}(\"f\")
TEST_F(function_signaturesTest, case032) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case032.json"));
}

// ($fun := λ($arr)<a<n>>{$arr};$fun(\"f\"))
TEST_F(function_signaturesTest, case033) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case033.json"));
}

// λ($arr)<(sa<n>)>>{$arr}([[1]])
TEST_F(function_signaturesTest, case034) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-signatures/case034.json"));
}

