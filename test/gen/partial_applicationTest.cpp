#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class partial_applicationTest : public JsonataTest {
};

// (  $add := function($x, $y){$x+$y};  $add2 := $add(?, 2);  $add2(3))
TEST_F(partial_applicationTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/partial-application/case000.json"));
}

// (  $add := function($x, $y){$x+$y};  $add2 := $add(2, ?);  $add2(4))
TEST_F(partial_applicationTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/partial-application/case001.json"));
}

// (  $firstn := $substring(?, 0, ?);  $first5 := $firstn(?, 5);  $first5(\"Hello World\"))
TEST_F(partial_applicationTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/partial-application/case002.json"));
}

// substring(?, 0, ?)
TEST_F(partial_applicationTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/partial-application/case003.json"));
}

// unknown(?)
TEST_F(partial_applicationTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/partial-application/case004.json"));
}

