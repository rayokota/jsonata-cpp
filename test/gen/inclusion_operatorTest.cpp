#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class inclusion_operatorTest : public JsonataTest {
};

// 1 in [1,2]
TEST_F(inclusion_operatorTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case000.json"));
}

// 3 in [1,2]
TEST_F(inclusion_operatorTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case001.json"));
}

// \"hello\" in [1,2]
TEST_F(inclusion_operatorTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case002.json"));
}

// \"world\" in [\"hello\", \"world\"]
TEST_F(inclusion_operatorTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case003.json"));
}

// in in [\"hello\", \"world\"]
TEST_F(inclusion_operatorTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case004.json"));
}

// \"world\" in in
TEST_F(inclusion_operatorTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case005.json"));
}

// \"hello\" in \"hello\"
TEST_F(inclusion_operatorTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case006.json"));
}

// library.books[\"Aho\" in authors].title
TEST_F(inclusion_operatorTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case007.json"));
}

// content.integration.$lowercase(name)
TEST_F(inclusion_operatorTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/inclusion-operator/case008.json"));
}

