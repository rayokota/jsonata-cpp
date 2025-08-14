#include <gtest/gtest.h>
#include "JsonataTest.h"

using namespace jsonata;

class regexTest : public JsonataTest {
};

// $split(\"ababbxabbcc\",/b+/)
TEST_F(regexTest, case000) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case000.json"));
}

// $split(\"ababbxabbcc\",/b+/, 2)
TEST_F(regexTest, case001) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case001.json"));
}

// $split(\"ababbxabbcc\",/d+/)
TEST_F(regexTest, case002) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case002.json"));
}

// $contains(\"ababbxabbcc\",/ab+/)
TEST_F(regexTest, case003) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case003.json"));
}

// $contains(\"ababbxabbcc\",/ax+/)
TEST_F(regexTest, case004) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case004.json"));
}

// Account.Order.Product[$contains($.\"Product Name\", /hat/)].ProductID
TEST_F(regexTest, case005) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case005.json"));
}

// Account.Order.Product[$contains($.\"Product Name\", /hat/i)].ProductID
TEST_F(regexTest, case006) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case006.json"));
}

// $replace(\"ababbxabbcc\",/b+/, \"yy\")
TEST_F(regexTest, case007) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case007.json"));
}

// $replace(\"ababbxabbcc\",/b+/, \"yy\", 2)
TEST_F(regexTest, case008) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case008.json"));
}

// $replace(\"ababbxabbcc\",/b+/, \"yy\", 0)
TEST_F(regexTest, case009) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case009.json"));
}

// $replace(\"ababbxabbcc\",/d+/, \"yy\")
TEST_F(regexTest, case010) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case010.json"));
}

// $replace(\"John Smith\", /(\\w+)\\s(\\w+)/, \"$2, $1\")
TEST_F(regexTest, case011) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case011.json"));
}

// $replace(\"265USD\", /([0-9]+)USD/, \"$$$1\")
TEST_F(regexTest, case012) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case012.json"));
}

// $replace(\"265USD\", /([0-9]+)USD/, \"$w\")
TEST_F(regexTest, case013) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case013.json"));
}

// $replace(\"265USD\", /([0-9]+)USD/, \"$0 -> $$$1\")
TEST_F(regexTest, case014) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case014.json"));
}

// $replace(\"265USD\", /([0-9]+)USD/, \"$0$1$2\")
TEST_F(regexTest, case015) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case015.json"));
}

// $replace(\"abcd\", /(ab)|(a)/, \"[1=$1][2=$2]\")
TEST_F(regexTest, case016) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case016.json"));
}

// $replace(\"abracadabra\", /bra/, \"*\")
TEST_F(regexTest, case017) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case017.json"));
}

// $replace(\"abracadabra\", /a.*a/, \"*\")
TEST_F(regexTest, case018) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case018.json"));
}

// $replace(\"abracadabra\", /a.*?a/, \"*\")
TEST_F(regexTest, case019) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case019.json"));
}

// $replace(\"abracadabra\", /a/, \"\")
TEST_F(regexTest, case020) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case020.json"));
}

// $replace(\"abracadabra\", /a(.)/, \"a$1$1\")
TEST_F(regexTest, case021) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case021.json"));
}

// $replace(\"abracadabra\", /.*?/, \"$1\")
TEST_F(regexTest, case022) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case022.json"));
}

// $replace(\"AAAA\", /A+/, \"b\")
TEST_F(regexTest, case023) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case023.json"));
}

// $replace(\"AAAA\", /A+?/, \"b\")
TEST_F(regexTest, case024) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case024.json"));
}

// $replace(\"darted\", /^(.*?)d(.*)$/, \"$1c$2\")
TEST_F(regexTest, case025) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case025.json"));
}

// $replace(\"abcdefghijklmno\", /(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)/, \"$8$5$12$12$18$123\")
TEST_F(regexTest, case026) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case026.json"));
}

// $replace(\"abcdefghijklmno\", /xyz/, \"$8$5$12$12$18$123\")
TEST_F(regexTest, case027) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case027.json"));
}

// $replace(\"abcdefghijklmno\", /ijk/, \"$8$5$12$12$18$123\")
TEST_F(regexTest, case028) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case028.json"));
}

// $replace(\"abcdefghijklmno\", /(ijk)/, \"$8$5$12$12$18$123\")
TEST_F(regexTest, case029) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case029.json"));
}

// $replace(\"abcdefghijklmno\", /ijk/, \"$x\")
TEST_F(regexTest, case030) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case030.json"));
}

// $replace(\"abcdefghijklmno\", /(ijk)/, \"$x$\")
TEST_F(regexTest, case031) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case031.json"));
}

// Account.Order.Product.$replace($.\"Product Name\", /hat/i, function($match) { \"foo\" })
TEST_F(regexTest, case032) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case032.json"));
}

// Account.Order.Product.$replace($.\"Product Name\", /(h)(at)/i, function($match) { $uppercase($match.match) })
TEST_F(regexTest, case033) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case033.json"));
}

// $replace(\"temperature = 68F today\", /(-?\\d+(?:\\.\\d*)?)F\\b/, function($m) { ($number($m.groups[0]) - 32) * 5/9 & \"C\" })
TEST_F(regexTest, case034) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case034.json"));
}

// Account.Order.Product.$replace($.\"Product Name\", /hat/i, function($match) { true })
TEST_F(regexTest, case035) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case035.json"));
}

// Account.Order.Product.$replace($.\"Product Name\", /hat/i, function($match) { 42 })
TEST_F(regexTest, case036) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case036.json"));
}

// $map($, $match(?, /^(\\w*\\s\\w*)/)).match
TEST_F(regexTest, case037) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case037.json"));
}

// $map($, function($i){ $match($i, /^(\\w*\\s\\w*)/) }).match
TEST_F(regexTest, case038) {
    EXPECT_NO_THROW(runCase("jsonata/test/test-suite/groups/regex/case038.json"));
}

