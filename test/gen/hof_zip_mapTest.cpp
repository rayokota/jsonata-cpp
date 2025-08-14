#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class hof_zip_mapTest : public JsonataTest {
};

// (  $data := {    \"one\": [1,2,3,4,5],    \"two\": [5,4,3,2,1]  };  $map($zip($data.one, $data.two), $sum)) 
TEST_F(hof_zip_mapTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-zip-map/case000.json"));
}

// (  $data := {    \"one\": [1,2,3,4,5],    \"two\": [5,4,3,2,1]  };  $data.$zip(one, two) ~> $map($sum)) 
TEST_F(hof_zip_mapTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-zip-map/case001.json"));
}

// (  $data := {    \"one\": [1],    \"two\": [5]  };  $data.$zip(one, two) ~> $map($sum)) 
TEST_F(hof_zip_mapTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-zip-map/case002.json"));
}

// (  $data := {    \"one\": 1,    \"two\": 5  };  $data.$zip(one, two) ~> $map($sum)) 
TEST_F(hof_zip_mapTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/hof-zip-map/case003.json"));
}

