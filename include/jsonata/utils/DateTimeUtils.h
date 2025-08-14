/**
 * jsonata-cpp is the JSONata C++ reference port
 *
 * Copyright Dashjoin GmbH. https://dashjoin.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Derived from original JSONata4Java DateTimeUtils code under this license:
/*
 * (c) Copyright 2018, 2019 IBM Corporation
 * 1 New Orchard Road,
 * Armonk, New York, 10504-1722
 * United States
 * +1 914 499 1900
 * support: Nathaniel Mills wnm3@us.ibm.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#pragma once

#include <chrono>
#include <cstdint>
#include <regex>
#include <string>
#include <vector>

namespace jsonata {
namespace utils {

class DateTimeUtils {
  public:
    // Number to words conversion
    static std::string numberToWords(int64_t value, bool ordinal = false);
    static int64_t wordsToNumber(const std::string& text);
    static int64_t wordsToLong(const std::string& text);

    // Roman numeral conversion
    static int64_t romanToDecimal(const std::string& roman);
    static std::string decimalToRoman(int64_t value);

    // Letter-based number conversion
    static int64_t lettersToDecimal(const std::string& letters, char aChar);
    static std::string decimalToLetters(int64_t value, const std::string& aChar);

    // Integer formatting
    static std::string formatInteger(int64_t value, const std::string& picture);

    // Date/time formatting and parsing
    static std::string formatDateTime(int64_t millis,
                                      const std::string& picture = "",
                                      const std::string& timezone = "");
    static int64_t parseDateTime(const std::string& timestamp,
                              const std::string& picture);

    // Month name helpers
    static int64_t monthNameToNumber(const std::string& monthName);
    static int64_t abbreviatedMonthNameToNumber(const std::string& monthName);

  private:
    // Enums
    enum class Formats { DECIMAL, LETTERS, ROMAN, WORDS, SEQUENCE };

    enum class TCase { UPPER, LOWER, TITLE };

    // Internal structures
    struct GroupingSeparator {
        int64_t position;
        std::string character;

        GroupingSeparator(int64_t pos, const std::string& ch)
            : position(pos), character(ch) {}
    };

    struct Format {
        std::string type = "integer";
        Formats primary = Formats::DECIMAL;
        TCase case_type = TCase::LOWER;
        bool ordinal = false;
        int64_t zeroCode = 0;
        int64_t mandatoryDigits = 0;
        int64_t optionalDigits = 0;
        bool regular = false;
        std::vector<GroupingSeparator> groupingSeparators;
        std::string token;
    };

    struct RomanNumeral {
        int64_t value;
        std::string letters;

        RomanNumeral(int64_t val, const std::string& lett)
            : value(val), letters(lett) {}
        int64_t getValue() const { return value; }
        const std::string& getLetters() const { return letters; }
    };

    struct SpecPart {
        std::string type;
        std::string value;
        char component;
        std::pair<int64_t, int64_t> width;
        std::string presentation1;
        char presentation2;
        bool ordinal = false;
        TCase names;
        bool hasNameFormatting = false;  // Java equivalent of names != null
        Format integerFormat;
        int64_t n;

        SpecPart(const std::string& t, const std::string& v)
            : type(t),
              value(v),
              component(0),
              width({-1, -1}),
              presentation2(0),
              names(TCase::LOWER),
              n(0) {}
        SpecPart(const std::string& t, char c)
            : type(t),
              value(""),
              component(c),
              width({-1, -1}),
              presentation2(0),
              names(TCase::LOWER),
              n(0) {}
    };

    struct PictureFormat {
        std::string type;
        std::vector<SpecPart> parts;

        PictureFormat(const std::string& t) : type(t) {}
        void addLiteral(const std::string& picture, int64_t start, int64_t end);
    };

    struct MatcherPart {
        std::string regex;
        char component;

        MatcherPart(const std::string& r) : regex(r), component(0) {}
        virtual ~MatcherPart() = default;
        virtual int64_t parse(const std::string& value) = 0;
    };

    // Concrete MatcherPart implementations (converted from Java anonymous
    // classes)

    struct LiteralMatcherPart : public MatcherPart {
        LiteralMatcherPart(const std::string& regex) : MatcherPart(regex) {}
        int64_t parse(const std::string& value) override;
    };

    struct TimezoneMatcherPart : public MatcherPart {
        bool isGMT;
        bool hasSeparator;
        std::string separatorChar;

        TimezoneMatcherPart(const std::string& regex, bool gmt, bool sep,
                            const std::string& sepChar = "")
            : MatcherPart(regex),
              isGMT(gmt),
              hasSeparator(sep),
              separatorChar(sepChar) {}
        int64_t parse(const std::string& value) override;
    };

    struct NameLookupMatcherPart : public MatcherPart {
        std::unordered_map<std::string, int64_t> lookup;

        NameLookupMatcherPart(
            const std::string& regex,
            const std::unordered_map<std::string, int64_t>& lookupMap)
            : MatcherPart(regex), lookup(lookupMap) {}
        int64_t parse(const std::string& value) override;
    };

    struct LettersMatcherPart : public MatcherPart {
        bool isUpper;

        LettersMatcherPart(const std::string& regex, bool upper)
            : MatcherPart(regex), isUpper(upper) {}
        int64_t parse(const std::string& value) override;
    };

    struct RomanMatcherPart : public MatcherPart {
        bool isUpper;

        RomanMatcherPart(const std::string& regex, bool upper)
            : MatcherPart(regex), isUpper(upper) {}
        int64_t parse(const std::string& value) override;
    };

    struct WordsMatcherPart : public MatcherPart {
        WordsMatcherPart(const std::string& regex) : MatcherPart(regex) {}
        int64_t parse(const std::string& value) override;
    };

    struct DecimalMatcherPart : public MatcherPart {
        Format formatSpec;

        DecimalMatcherPart(const std::string& regex, const Format& format)
            : MatcherPart(regex), formatSpec(format) {}
        int64_t parse(const std::string& value) override;
    };

    struct PictureMatcher {
        std::vector<std::unique_ptr<MatcherPart>> parts;
    };

    // Static data arrays
    static const std::vector<std::string> few;
    static const std::vector<std::string> ordinals;
    static const std::vector<std::string> decades;
    static const std::vector<std::string> magnitudes;
    static const std::vector<std::string> days;
    static const std::vector<std::string> months;
    static const std::vector<RomanNumeral> romanNumerals;
    static const std::vector<int64_t> decimalGroups;

    // Static maps
    static const std::unordered_map<std::string, int64_t> wordValues;
    static const std::unordered_map<std::string, int64_t> wordValuesLong;
    static const std::unordered_map<std::string, int64_t> romanValues;
    static const std::unordered_map<std::string, std::string> suffix123;
    static const std::unordered_map<char, std::string>
        defaultPresentationModifiers;

    // Static helper functions
    static std::string lookup(int64_t num, bool prev, bool ord);
    static std::string formatInteger(int64_t value, const Format& format);
    static Format analyseIntegerPicture(const std::string& picture);
    static int64_t getRegularRepeat(
        const std::vector<GroupingSeparator>& separators);
    static PictureFormat analyseDateTimePicture(const std::string& picture);
    static int64_t parseWidth(const std::string& wm);
    static std::string formatComponent(
        const std::chrono::system_clock::time_point& date,
        const SpecPart& markerSpec, int64_t offsetHours, int64_t offsetMinutes);
    static std::string getDateTimeFragment(
        const std::chrono::system_clock::time_point& date, char component);
    static void calculateISOWeekDate(int64_t year, int64_t month, int64_t day, int64_t& isoYear,
                                     int64_t& isoWeek);
    static PictureMatcher generateRegex(const PictureFormat& formatSpec);
    static std::unique_ptr<MatcherPart> generateRegex(char component,
                                                      const Format& formatSpec);
    static bool isType(int64_t type, int64_t mask);

    // Static initialization helpers
    static std::unordered_map<std::string, int64_t> createWordValues();
    static std::unordered_map<std::string, int64_t> createWordValuesLong();
    static std::unordered_map<std::string, int64_t> createRomanValues();
    static std::unordered_map<std::string, std::string> createSuffixMap();
    static std::unordered_map<char, std::string>
    createDefaultPresentationModifiers();
};

}  // namespace utils
}  // namespace jsonata