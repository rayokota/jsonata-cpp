#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class hof_mapTest : public JsonataTest {
};

// (  $data := {    \"one\": [1,2,3,4,5],    \"two\": [5,4,3,2,1]  };  $add := function($x){$x*$x};  $map($data.one, $add) )  
TEST_F(hof_mapTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case000.json"));
}

// (  $data := {    \"one\": [1,2,3,4,5],    \"two\": [5,4,3,2,1]  };  $add := function($x){$x*$x};  $map($add) )  
TEST_F(hof_mapTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case001.json"));
}

// ( $square := function($x){$x*$x}; $map([1], $square)[] )
TEST_F(hof_mapTest, case0010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case0010.json"));
}

// ( $data := [1]; $square := function($x){$x*$x}; $data ~> $map($square)[] )
TEST_F(hof_mapTest, case0011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case0011.json"));
}

// $map([1,2,3], $string)
TEST_F(hof_mapTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case002.json"));
}

// Account.Order.Product ~> $map(λ($prod, $index) { $index+1 & \": \" & $prod.\"Product Name\" })
TEST_F(hof_mapTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case003.json"));
}

// Account.Order.Product ~> $map(λ($prod, $index, $arr) { $index+1 & \"/\" & $count($arr) & \": \" & $prod.\"Product Name\" })
TEST_F(hof_mapTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case004.json"));
}

// $map(Phone, function($v, $i) {$i[$v.type=\"office\"]})
TEST_F(hof_mapTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case005.json"));
}

// $map(Phone, function($v, $i) {$v.type=\"office\" ? $i})
TEST_F(hof_mapTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case006.json"));
}

// $map(Phone, function($v, $i) {$v.type=\"office\" ? $i: null})
TEST_F(hof_mapTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case007.json"));
}

// $map(Phone, function($v, $i) {$v.type=\"office\" ? $i: null})
TEST_F(hof_mapTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case008.json"));
}

// $map([0, 1, 2], $boolean)
TEST_F(hof_mapTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-map/case009.json"));
}

