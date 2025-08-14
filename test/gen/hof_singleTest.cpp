#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class hof_singleTest : public JsonataTest {
};

// (library.books~>$single(λ($v, $i, $a) {$v.price = $max($a.price)})).isbn
TEST_F(hof_singleTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case000.json"));
}

// nothing~>$single(λ($v, $i, $a) {$v.price = $max($a.price)})
TEST_F(hof_singleTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case001.json"));
}

// $single(data, function($d) { true })
TEST_F(hof_singleTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case002.json"));
}

// $single([0, 1, 2], $boolean)
TEST_F(hof_singleTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case003.json"));
}

// $single([0, 1, 2], function($v) {$v = 3})
TEST_F(hof_singleTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case004.json"));
}

// $single([0, 1, 2])
TEST_F(hof_singleTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case005.json"));
}

// $single([0])
TEST_F(hof_singleTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case006.json"));
}

// $single([])
TEST_F(hof_singleTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case007.json"));
}

// $single([true, false, true], function($v) {$v=false})
TEST_F(hof_singleTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case008.json"));
}

// $single(['zero', 'one', 'two'], function($v, $i, $k) {$i=1})
TEST_F(hof_singleTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case009.json"));
}

// Phone ~> $filter(function($p) {$p.type = 'home'}) ~> $single(function($p) {$p.number = '0203 544 1234'})
TEST_F(hof_singleTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-single/case010.json"));
}

