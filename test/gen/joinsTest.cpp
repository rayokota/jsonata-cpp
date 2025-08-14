#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class joinsTest : public JsonataTest {
};

// Employee@$e.(Contact)[ssn = $e.SSN].{ 'name': $e.(FirstName & ' ' & Surname), 'phone': Phone[type='mobile'].number }
TEST_F(joinsTest, employee_map_reduce_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 0));
}

// Employee@$e.(Contact)[ssn = $e.SSN]{ $e.FirstName: Phone[type='mobile'].number }
TEST_F(joinsTest, employee_map_reduce_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 1));
}

// Employee@$e.Contact@$c[$c.ssn = $e.SSN]{ $e.(FirstName & ' ' & Surname): $join($c.Phone.number, ', ') }
TEST_F(joinsTest, employee_map_reduce_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 2));
}

// Employee@$e.Contact@$c[$c.ssn = $e.SSN]{ $e.(FirstName & ' ' & Surname): $c.Phone[type != 'home'].number ~> $join(', ') }
TEST_F(joinsTest, employee_map_reduce_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 3));
}

// Employee@$e.Contact@$c[$c.ssn = $e.SSN]{ $e.(FirstName & ' ' & Surname): $c.Phone[type != 'home'].number }
TEST_F(joinsTest, employee_map_reduce_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 4));
}

// Employee@$e.Contact@$c[$c.ssn = $e.SSN]{ $e.(FirstName & ' ' & Surname): $c.Phone[type != 'home'].number[] }
TEST_F(joinsTest, employee_map_reduce_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 5));
}

// Employee@$e.Contact@$c[$c.ssn = $e.SSN]{ $e.FirstName: $c.Phone.number }
TEST_F(joinsTest, employee_map_reduce_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 6));
}

// Employee@$e^($e.Surname).Contact@$c[$e.SSN=$c.ssn].{ 'name': $e.Surname, 'phone': $c.Phone.number }
TEST_F(joinsTest, employee_map_reduce_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 7));
}

// Employee@$e^($e.FirstName).Contact@$c[$e.SSN=$c.ssn].{ 'name': $e.Surname, 'phone': $c.Phone.number }
TEST_F(joinsTest, employee_map_reduce_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 8));
}

// Employee@$e.Contact@$c[$e.SSN=$c.ssn]{ $e.Surname: $c.Phone.number^($) }
TEST_F(joinsTest, employee_map_reduce_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 9));
}

// Employee@$e.Contact@$c[$e.SSN=$c.ssn]{ $e.Surname: $c.Phone.number^(>$) }
TEST_F(joinsTest, employee_map_reduce_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 10));
}

// $@$i
TEST_F(joinsTest, employee_map_reduce_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/employee-map-reduce.json", 11));
}

// Account.Order@o.Product
TEST_F(joinsTest, errors_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/errors.json", 0));
}

// Account.Order@$o#i.Product
TEST_F(joinsTest, errors_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/errors.json", 1));
}

// Account.Order[1]@$o.Product
TEST_F(joinsTest, errors_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/errors.json", 2));
}

// Account.Order^(>OrderID)@$o.Product.{ 'name':`Product Name`, 'orderid':$o.OrderID }
TEST_F(joinsTest, errors_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/errors.json", 3));
}

// $#$pos[$pos<3]
TEST_F(joinsTest, index_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 0));
}

// $#$pos[$pos<3] = $[[0..2]]
TEST_F(joinsTest, index_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 1));
}

// $.$#$pos[$pos<3]
TEST_F(joinsTest, index_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 2));
}

// $.$#$pos[$pos<3] = $.$[[0..2]]
TEST_F(joinsTest, index_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 3));
}

// $#$pos[$pos<3]^($)
TEST_F(joinsTest, index_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 4));
}

// $#$pos[$pos<3]^(>$)
TEST_F(joinsTest, index_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 5));
}

// $^($)#$pos[$pos<3] 
TEST_F(joinsTest, index_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 6));
}

// $#$pos[$pos<3][1]
TEST_F(joinsTest, index_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 7));
}

// $#$pos[$pos<3][1][]
TEST_F(joinsTest, index_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 8));
}

// $#$pos[$pos<3]^($)[-1]
TEST_F(joinsTest, index_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 9));
}

// $#$pos[][$pos<3]^($)[-1]
TEST_F(joinsTest, index_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 10));
}

// $#$pos[$pos<3]^($)[-1][]
TEST_F(joinsTest, index_case_11) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 11));
}

// $[[1..4]]#$pos[$pos>=2]
TEST_F(joinsTest, index_case_12) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 12));
}

// Account.Order#$o.Product[ProductID=858383].{ 'Product': `Product Name`, 'Order Index': $o }
TEST_F(joinsTest, index_case_13) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 13));
}

// Account.Order#$i.Missing
TEST_F(joinsTest, index_case_14) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 14));
}

// Account.Order#$i.Product{ $string(ProductID): $i }
TEST_F(joinsTest, index_case_15) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/index.json", 15));
}

// 
TEST_F(joinsTest, library_joins_case_0) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 0));
}

// 
TEST_F(joinsTest, library_joins_case_1) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 1));
}

// library.books#$pos.$[$substring(title,0,3) = 'The'].$pos
TEST_F(joinsTest, library_joins_case_2) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 2));
}

// 
TEST_F(joinsTest, library_joins_case_3) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 3));
}

// 
TEST_F(joinsTest, library_joins_case_4) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 4));
}

// 
TEST_F(joinsTest, library_joins_case_5) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 5));
}

// 
TEST_F(joinsTest, library_joins_case_6) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 6));
}

// 
TEST_F(joinsTest, library_joins_case_7) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 7));
}

// 
TEST_F(joinsTest, library_joins_case_8) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 8));
}

// 
TEST_F(joinsTest, library_joins_case_9) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 9));
}

// 
TEST_F(joinsTest, library_joins_case_10) {
    EXPECT_NO_THROW(runSubCase("jsonata/test/test-suite/groups/joins/library-joins.json", 10));
}

