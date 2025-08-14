#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class commentsTest : public JsonataTest {
};

// 
TEST_F(commentsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comments/case000.json"));
}

// 
TEST_F(commentsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comments/case001.json"));
}

// 
TEST_F(commentsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comments/case002.json"));
}

// 
TEST_F(commentsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/comments/case003.json"));
}

