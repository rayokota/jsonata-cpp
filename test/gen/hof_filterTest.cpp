#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class hof_filterTest : public JsonataTest {
};

// (library.books~>$filter(λ($v, $i, $a) {$v.price = $max($a.price)})).isbn
TEST_F(hof_filterTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-filter/case000.json"));
}

// nothing~>$filter(λ($v, $i, $a) {$v.price = $max($a.price)})
TEST_F(hof_filterTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-filter/case001.json"));
}

// $filter(data, function($d) { true })
TEST_F(hof_filterTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-filter/case002.json"));
}

// $filter([0, 1, 2], $boolean)
TEST_F(hof_filterTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-filter/case003.json"));
}

