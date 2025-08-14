#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class encodingTest : public JsonataTest {
};

// $base64encode(\"hello:world\")
TEST_F(encodingTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/encoding/case000.json"));
}

// $base64encode()
TEST_F(encodingTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/encoding/case001.json"));
}

// $base64decode(\"aGVsbG86d29ybGQ=\")
TEST_F(encodingTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/encoding/case002.json"));
}

// $base64decode()
TEST_F(encodingTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/encoding/case003.json"));
}

