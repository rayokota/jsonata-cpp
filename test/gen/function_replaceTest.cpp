#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class function_replaceTest : public JsonataTest {
};

// $replace(\"Hello World\", \"World\", \"Everyone\")
TEST_F(function_replaceTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case000.json"));
}

// $replace(\"the cat sat on the mat\", \"at\", \"it\")
TEST_F(function_replaceTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case001.json"));
}

// $replace(\"the cat sat on the mat\", \"at\", \"it\", 0)
TEST_F(function_replaceTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case002.json"));
}

// $replace(\"the cat sat on the mat\", \"at\", \"it\", 2)
TEST_F(function_replaceTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case003.json"));
}

// $replace(nothing, \"at\", \"it\", 2)
TEST_F(function_replaceTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case004.json"));
}

// $replace(\"hello\")
TEST_F(function_replaceTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case005.json"));
}

// $replace(\"hello\", \"l\", \"1\", null)
TEST_F(function_replaceTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case006.json"));
}

// $replace(\"hello\", \"l\", \"1\", -2)
TEST_F(function_replaceTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case007.json"));
}

// $replace(\"hello\", 1)
TEST_F(function_replaceTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case008.json"));
}

// $replace(\"hello\", \"\", \"bye\")
TEST_F(function_replaceTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case009.json"));
}

// $replace(\"hello\", 2, 1)
TEST_F(function_replaceTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case010.json"));
}

// $replace(123, 2, 1)
TEST_F(function_replaceTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/function-replace/case011.json"));
}

