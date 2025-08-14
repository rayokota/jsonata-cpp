#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class missing_pathsTest : public JsonataTest {
};

// fdf
TEST_F(missing_pathsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/missing-paths/case000.json"));
}

// fdf.ett
TEST_F(missing_pathsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/missing-paths/case001.json"));
}

// fdf.ett[10]
TEST_F(missing_pathsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/missing-paths/case002.json"));
}

// fdf.ett[vc > 10]
TEST_F(missing_pathsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/missing-paths/case003.json"));
}

// fdf.ett + 27
TEST_F(missing_pathsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/missing-paths/case004.json"));
}

// $fdsd
TEST_F(missing_pathsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/missing-paths/case005.json"));
}

