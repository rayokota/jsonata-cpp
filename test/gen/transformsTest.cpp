#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class transformsTest : public JsonataTest {
};

// $ ~> |Account.Order.Product|{\"Total\":Price*Quantity},[\"Description\", \"SKU\"]|
TEST_F(transformsTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case000.json"));
}

// Account.Order ~> |Product|{\"Total\":Price*Quantity},[\"Description\", \"SKU\"]|
TEST_F(transformsTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case001.json"));
}

// $ ~> |Account.Order.Product|{\"Total\":Price*Quantity, \"Price\": Price * 1.2}|
TEST_F(transformsTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case002.json"));
}

// $ ~> |Account.Order.Product|{},\"Description\"|
TEST_F(transformsTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case003.json"));
}

// $ ~> |Account.Order.Product|nomatch,\"Description\"|
TEST_F(transformsTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case004.json"));
}

// $ ~> |(Account.Order.Product)[0]|{\"Description\":\"blah\"}|
TEST_F(transformsTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case005.json"));
}

// $ ~> |foo.bar|{\"Description\":\"blah\"}|
TEST_F(transformsTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case006.json"));
}

// Account ~> |Order|{\"Product\":\"blah\"},nomatch|
TEST_F(transformsTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case007.json"));
}

// foo ~> |foo.bar|{\"Description\":\"blah\"}|
TEST_F(transformsTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case008.json"));
}

// Account ~> |Order|5|
TEST_F(transformsTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case009.json"));
}

// Account ~> |Order|{},5|
TEST_F(transformsTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case010.json"));
}

// Account ~> |Order|{\"Product\":\"blah\"},nomatch|
TEST_F(transformsTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case011.json"));
}

// {} ~> |$|['one', 'two', 'three']|
TEST_F(transformsTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case012.json"));
}

// $ ~> | ** ['hi'] | {'hello': hi}, ['hi'] |
TEST_F(transformsTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case013.json"));
}

// $ ~> | ** [hi] | {'hello': hi}, ['hi'] |
TEST_F(transformsTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transforms/case014.json"));
}

