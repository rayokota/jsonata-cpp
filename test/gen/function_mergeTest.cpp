#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_mergeTest : public JsonataTest {
};

// $merge(nothing)
TEST_F(function_mergeTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-merge/case000.json"));
}

// $merge({\"a\":1})
TEST_F(function_mergeTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-merge/case001.json"));
}

// $merge([{\"a\":1}, {\"b\":2}])
TEST_F(function_mergeTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-merge/case002.json"));
}

// $merge([{\"a\": 1}, {\"b\": 2, \"c\": 3}])
TEST_F(function_mergeTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-merge/case003.json"));
}

// $merge([{\"a\": 1}, {\"b\": 2, \"a\": 3}])
TEST_F(function_mergeTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-merge/case004.json"));
}

