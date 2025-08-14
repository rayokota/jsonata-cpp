#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class performanceTest : public JsonataTest {
};

// {\"items\": items.{\"foo\": \"bar\",\"count\": $count($$.items[text!=\"\" and text!=\"foo\"].row)}}
TEST_F(performanceTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/performance/case000.json"));
}

// {\"items\": items#$i.{\"foo\": \"bar\",\"label\": $$.items[$i].label & \" \" & $$.items[text!=\"\" and text!=\"test\"][$i].text}}
TEST_F(performanceTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/performance/case001.json"));
}

