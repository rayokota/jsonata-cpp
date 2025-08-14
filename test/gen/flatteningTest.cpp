#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class flatteningTest : public JsonataTest {
};

// phone
TEST_F(flatteningTest, array_inputs_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 0));
}

// phone[0]
TEST_F(flatteningTest, array_inputs_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 1));
}

// phone[0][]
TEST_F(flatteningTest, array_inputs_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 2));
}

// phone[0].number
TEST_F(flatteningTest, array_inputs_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 3));
}

// (phone)[0].number
TEST_F(flatteningTest, array_inputs_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 4));
}

// $.phone[0].number
TEST_F(flatteningTest, array_inputs_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 5));
}

// {'Hello':'World'}
TEST_F(flatteningTest, array_inputs_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 6));
}

// $.{'Hello':'World'}
TEST_F(flatteningTest, array_inputs_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/array-inputs.json", 7));
}

// Account.Order.[Product.Price]
TEST_F(flatteningTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case000.json"));
}

// $.nest0
TEST_F(flatteningTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case001.json"));
}

// nest0
TEST_F(flatteningTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case002.json"));
}

// $[0]
TEST_F(flatteningTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case003.json"));
}

// $[1]
TEST_F(flatteningTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case004.json"));
}

// $[-1]
TEST_F(flatteningTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case005.json"));
}

// $[0].nest0
TEST_F(flatteningTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case006.json"));
}

// $[1].nest0
TEST_F(flatteningTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case007.json"));
}

// $[0].nest0[0]
TEST_F(flatteningTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case008.json"));
}

// nest0.[nest1.[nest2.[nest3]]]
TEST_F(flatteningTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case009.json"));
}

// nest0.nest1.[nest2.[nest3]]
TEST_F(flatteningTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case010.json"));
}

// nest0.[nest1.nest2.[nest3]]
TEST_F(flatteningTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case011.json"));
}

// nest0.[nest1.[nest2.nest3]]
TEST_F(flatteningTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case012.json"));
}

// nest0.[nest1.nest2.nest3]
TEST_F(flatteningTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case013.json"));
}

// nest0.nest1.[nest2.nest3]
TEST_F(flatteningTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case014.json"));
}

// nest0.nest1.nest2.[nest3]
TEST_F(flatteningTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case015.json"));
}

// nest0.nest1.nest2.nest3
TEST_F(flatteningTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case016.json"));
}

// nest0.[nest1.[nest2.[nest3]]]
TEST_F(flatteningTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case017.json"));
}

// nest0.nest1.[nest2.[nest3]]
TEST_F(flatteningTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case018.json"));
}

// nest0.[nest1.nest2.[nest3]]
TEST_F(flatteningTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case019.json"));
}

// nest0.[nest1.[nest2.nest3]]
TEST_F(flatteningTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case020.json"));
}

// nest0.[nest1.nest2.nest3]
TEST_F(flatteningTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case021.json"));
}

// nest0.nest1.[nest2.nest3]
TEST_F(flatteningTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case022.json"));
}

// nest0.nest1.nest2.[nest3]
TEST_F(flatteningTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case023.json"));
}

// nest0.nest1.nest2.nest3
TEST_F(flatteningTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case024.json"));
}

// {\"a\": 1 }.a
TEST_F(flatteningTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case025.json"));
}

// a
TEST_F(flatteningTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case026.json"));
}

// {\"a\": [1] }.a
TEST_F(flatteningTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case027.json"));
}

// a
TEST_F(flatteningTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case028.json"));
}

// {\"a\": [[1]] }.a
TEST_F(flatteningTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case029.json"));
}

// a
TEST_F(flatteningTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case030.json"));
}

// [{\"a\":[1,2]}, {\"a\":[3]}].a
TEST_F(flatteningTest, case031) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case031.json"));
}

// a
TEST_F(flatteningTest, case032) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case032.json"));
}

// [{\"a\":[{\"b\":[1]}, {\"b\":[2]}]}, {\"a\":[{\"b\":[3]}, {\"b\":[4]}]}].a[0].b
TEST_F(flatteningTest, case033) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case033.json"));
}

// a[0].b
TEST_F(flatteningTest, case034) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case034.json"));
}

// $.a[0].b
TEST_F(flatteningTest, case034a) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case034a.json"));
}

// a.b[0]
TEST_F(flatteningTest, case035) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case035.json"));
}

// Phone[type=\"mobile\"].number
TEST_F(flatteningTest, case036) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case036.json"));
}

// Phone[type=\"mobile\"][].number
TEST_F(flatteningTest, case037) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case037.json"));
}

// Phone[][type=\"mobile\"].number
TEST_F(flatteningTest, case038) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case038.json"));
}

// Phone[type=\"office\"][].number
TEST_F(flatteningTest, case039) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case039.json"));
}

// Phone{type: number}
TEST_F(flatteningTest, case040) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case040.json"));
}

// Phone{type: number[]}
TEST_F(flatteningTest, case041) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case041.json"));
}

// $[type='command'][]
TEST_F(flatteningTest, case042) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case042.json"));
}

// $[][type='command']
TEST_F(flatteningTest, case043) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case043.json"));
}

// $filter($, function($e) { $e != 0 })[]
TEST_F(flatteningTest, case044) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case044.json"));
}

// $.tags[title='example'][]
TEST_F(flatteningTest, case045) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/flattening/case045.json"));
}

// ( $data := { 'number': [1..2e5].$string() }; $values := $data.number; $count($values) )
TEST_F(flatteningTest, large_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/large.json", 0));
}

// ( $data := { 'number': [1..2e5].$string() }; $values := $lookup($data, 'number'); $count($values) )
TEST_F(flatteningTest, large_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/large.json", 1));
}

// $.[value,epochSeconds]
TEST_F(flatteningTest, sequence_of_arrays_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/sequence-of-arrays.json", 0));
}

// $.[value,epochSeconds]
TEST_F(flatteningTest, sequence_of_arrays_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/sequence-of-arrays.json", 1));
}

// $.[value,epochSeconds][]
TEST_F(flatteningTest, sequence_of_arrays_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/sequence-of-arrays.json", 2));
}

// $.[value,epochSeconds][]
TEST_F(flatteningTest, sequence_of_arrays_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/flattening/sequence-of-arrays.json", 3));
}

