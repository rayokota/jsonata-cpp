#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class transformTest : public JsonataTest {
};

// {'Order': Account.Order.{  'ID': OrderID,  'Product': Product.{    'SKU': ProductID,    'Details': {      'Weight': Description.Weight,      'Dimensions': Description.(Width & ' x ' & Height & ' x ' & Depth)    }  },  'Total Price': $sum(Product.(Price * Quantity))}}
TEST_F(transformTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case000.json"));
}

//          (           $tempReadings := $count(state.tempReadings) = 5 ?               [state.tempReadings[[1..4]], event.t] :               [state.tempReadings, event.t];                    {             \"tempReadings\": $tempReadings,             \"sumTemperatures\": $sum($tempReadings),             \"avgTemperature\": $average($tempReadings) ~> $round(2),             \"maxTemperature\": $max($tempReadings),             \"minTemperature\": $min($tempReadings)           }         )
TEST_F(transformTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case001.json"));
}

// $
TEST_F(transformTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case002.json"));
}

// foo
TEST_F(transformTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case003.json"));
}

// baz
TEST_F(transformTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case004.json"));
}

// Foo
TEST_F(transformTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case005.json"));
}

// food
TEST_F(transformTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case006.json"));
}

// maz.rar
TEST_F(transformTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case007.json"));
}

// jee.par.waa
TEST_F(transformTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case008.json"));
}

// foo
TEST_F(transformTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case009.json"));
}

// foo.bar
TEST_F(transformTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case010.json"));
}

// $
TEST_F(transformTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case011.json"));
}

// bar
TEST_F(transformTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case012.json"));
}

// $
TEST_F(transformTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case013.json"));
}

// $
TEST_F(transformTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case014.json"));
}

// $
TEST_F(transformTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case015.json"));
}

// bar
TEST_F(transformTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case016.json"));
}

// $
TEST_F(transformTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case017.json"));
}

// bar
TEST_F(transformTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case018.json"));
}

// $
TEST_F(transformTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case019.json"));
}

// bar
TEST_F(transformTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case020.json"));
}

// bar
TEST_F(transformTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case021.json"));
}

// $
TEST_F(transformTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case022.json"));
}

// bar
TEST_F(transformTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case023.json"));
}

// $
TEST_F(transformTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case024.json"));
}

// bar
TEST_F(transformTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case025.json"));
}

// $
TEST_F(transformTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case026.json"));
}

// $lowercase(Salutation)
TEST_F(transformTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case027.json"));
}

// $lowercase(敷)
TEST_F(transformTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case028.json"));
}

// $lowercase(Español)
TEST_F(transformTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case029.json"));
}

// $lowercase($.\"NI.Number\")
TEST_F(transformTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case030.json"));
}

// $lowercase(\"COMPENSATION IS : \" & Employment.\"Executive.Compensation\")
TEST_F(transformTest, case031) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case031.json"));
}

// \"Hello\"
TEST_F(transformTest, case032) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case032.json"));
}

// $uppercase(Salutation)
TEST_F(transformTest, case033) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case033.json"));
}

// $uppercase(\"Hello World\")
TEST_F(transformTest, case034) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case034.json"));
}

// $uppercase(\"鯵噂ソ竹\")
TEST_F(transformTest, case035) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case035.json"));
}

// Employment.Name & \" is happy\"
TEST_F(transformTest, case036) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case036.json"));
}

// Employment.Name & \" is pleased to employ \" & Salutation & \" \" & Surname
TEST_F(transformTest, case037) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case037.json"));
}

// $uppercase(\"Hello \" & Salutation & \" \" & Surname)
TEST_F(transformTest, case038) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case038.json"));
}

// $uppercase(Salutation & \" \" & Surname & \" - has \" & Cars & \" registered cars\")
TEST_F(transformTest, case039) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case039.json"));
}

// $uppercase(Employment.ContractType)
TEST_F(transformTest, case040) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case040.json"));
}

// $substringBefore(\"Hola\", \"l\")
TEST_F(transformTest, case041) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case041.json"));
}

// $substringBefore(\"Hola\", 'l')
TEST_F(transformTest, case042) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case042.json"));
}

// $substringBefore(\"Hola\", \"Q\")
TEST_F(transformTest, case043) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case043.json"));
}

// $substringBefore(\"Hola\", \"\")
TEST_F(transformTest, case044) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case044.json"));
}

// $substringBefore(\"鯵噂ソ竹\", \"ソ\")
TEST_F(transformTest, case045) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case045.json"));
}

// $substringAfter(\"Coca\" & \"Cola\", \"ca\")
TEST_F(transformTest, case046) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case046.json"));
}

// $substringAfter(Salutation & \" \" & MiddleName &\" \" & Surname, MiddleName)
TEST_F(transformTest, case047) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case047.json"));
}

// $substringAfter(Salutation & \" \" & Employment.Role, Salutation)
TEST_F(transformTest, case048) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case048.json"));
}

// $substringAfter(\"Hola\", 'l')
TEST_F(transformTest, case049) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case049.json"));
}

// $substringAfter(\"Hola\", \"Q\")
TEST_F(transformTest, case050) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case050.json"));
}

// $lowercase(Employment.Role) & \" (\" & Employment.Role & \")\"
TEST_F(transformTest, case051) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case051.json"));
}

// Employment.Years & \" years of employment\"
TEST_F(transformTest, case052) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case052.json"));
}

// $uppercase(Salutation & \" \" & Surname & \" - has \" & Employment.Years & \" years of employment\")
TEST_F(transformTest, case053) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case053.json"));
}

// $substring(Employment.Role, 7,4)
TEST_F(transformTest, case054) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case054.json"));
}

// $substring(Employment.Role, -4,4)
TEST_F(transformTest, case055) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case055.json"));
}

// $substring(\"Hello World\",Cars,5)
TEST_F(transformTest, case056) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case056.json"));
}

// $lowercase(\"Missing close brackets\"
TEST_F(transformTest, case057) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case057.json"));
}

// $unknown(Salutation)
TEST_F(transformTest, case058) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case058.json"));
}

// $decrypt(Salutation)
TEST_F(transformTest, case059) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case059.json"));
}

// Employment.authentication(Salutation)
TEST_F(transformTest, case060) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case060.json"));
}

// $uppercase(Invalid)
TEST_F(transformTest, case061) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case061.json"));
}

// $uppercase(Employment.Invalid)
TEST_F(transformTest, case062) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case062.json"));
}

// $lowercase(\"Coca\", \"Cola\")
TEST_F(transformTest, case063) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case063.json"));
}

// $lowercase(Salary)
TEST_F(transformTest, case064) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case064.json"));
}

// $lowercase(20)
TEST_F(transformTest, case065) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case065.json"));
}

// $lowercase(20.55)
TEST_F(transformTest, case066) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case066.json"));
}

// $lowercase(Employment)
TEST_F(transformTest, case067) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case067.json"));
}

// $lowercase(Qualifications)
TEST_F(transformTest, case068) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case068.json"));
}

// $uppercase(\"Coca\", \"Cola\")
TEST_F(transformTest, case069) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case069.json"));
}

// $uppercase(Salary)
TEST_F(transformTest, case070) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case070.json"));
}

// $uppercase(28)
TEST_F(transformTest, case071) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case071.json"));
}

// $uppercase(20.55)
TEST_F(transformTest, case072) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case072.json"));
}

// $uppercase(Cars)
TEST_F(transformTest, case073) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case073.json"));
}

// $uppercase(Employment)
TEST_F(transformTest, case074) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case074.json"));
}

// $uppercase(Qualifications)
TEST_F(transformTest, case075) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case075.json"));
}

// $substringBefore(\"Coca\" & \"ca\")
TEST_F(transformTest, case076) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case076.json"));
}

// $substringBefore(Salary,\"xx\")
TEST_F(transformTest, case077) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case077.json"));
}

// $substringBefore(22,\"xx\")
TEST_F(transformTest, case078) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case078.json"));
}

// $substringBefore(22.55,\"xx\")
TEST_F(transformTest, case079) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case079.json"));
}

// $substringBefore(\"22\",2)
TEST_F(transformTest, case080) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case080.json"));
}

// $substringBefore(\"22.55\",5)
TEST_F(transformTest, case081) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case081.json"));
}

// $substringBefore(Employment,\"xx\")
TEST_F(transformTest, case082) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case082.json"));
}

// $substringBefore(Qualifications,\"xx\")
TEST_F(transformTest, case083) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case083.json"));
}

// $substringAfter(\"Coca\" & \"ca\")
TEST_F(transformTest, case084) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case084.json"));
}

// $substringAfter(Salary,\"xx\")
TEST_F(transformTest, case085) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case085.json"));
}

// $substringAfter(22,\"xx\")
TEST_F(transformTest, case086) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case086.json"));
}

// $substringAfter(22.55,\"xx\")
TEST_F(transformTest, case087) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case087.json"));
}

// $substringAfter(\"22\",2)
TEST_F(transformTest, case088) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case088.json"));
}

// $substringAfter(\"22.55\",5)
TEST_F(transformTest, case089) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case089.json"));
}

// $substringAfter(Employment,\"xx\")
TEST_F(transformTest, case090) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case090.json"));
}

// $substringAfter(Qualifications,\"xx\")
TEST_F(transformTest, case091) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case091.json"));
}

// $substring(\"Coca\" & \"ca\", 2, 4, 5)
TEST_F(transformTest, case092) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case092.json"));
}

// $substring(\"Coca\", \"Mr\", 4)
TEST_F(transformTest, case093) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case093.json"));
}

// $substring(\"Coca\", 3, \"Whoops\")
TEST_F(transformTest, case094) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case094.json"));
}

// $substring(Salary,2,4)
TEST_F(transformTest, case095) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case095.json"));
}

// $substring(\"Hello\",\"World\",5)
TEST_F(transformTest, case096) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case096.json"));
}

// $substring(\"Hello\",5,\"World\")
TEST_F(transformTest, case097) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case097.json"));
}

// $substring(\"Hello World\",5.5,5)
TEST_F(transformTest, case098) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case098.json"));
}

// $substring(Employment,\"xx\")
TEST_F(transformTest, case099) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case099.json"));
}

// $substring(Qualifications,6,5)
TEST_F(transformTest, case100) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case100.json"));
}

// detail.contents
TEST_F(transformTest, case101) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case101.json"));
}

// detail.meta
TEST_F(transformTest, case102) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case102.json"));
}

// detail.meta
TEST_F(transformTest, case103) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/transform/case103.json"));
}

