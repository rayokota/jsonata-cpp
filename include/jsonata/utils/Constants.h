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
#pragma once

#include <string>

namespace jsonata {
namespace utils {

/**
 * Constants required by DateTimeUtils
 */
class Constants {
  public:
    static const std::string ERR_MSG_SEQUENCE_UNSUPPORTED;
    static const std::string ERR_MSG_DIFF_DECIMAL_GROUP;
    static const std::string ERR_MSG_NO_CLOSING_BRACKET;
    static const std::string ERR_MSG_UNKNOWN_COMPONENT_SPECIFIER;
    static const std::string ERR_MSG_INVALID_NAME_MODIFIER;
    static const std::string ERR_MSG_TIMEZONE_FORMAT;
    static const std::string ERR_MSG_MISSING_FORMAT;
    static const std::string ERR_MSG_INVALID_OPTIONS_SINGLE_CHAR;
    static const std::string ERR_MSG_INVALID_OPTIONS_STRING;

    /**
     * Collection of decimal format strings that defined by xsl:decimal-format.
     *
     * <pre>
     *     &lt;!ELEMENT xsl:decimal-format EMPTY&gt;
     *     &lt;!ATTLIST xsl:decimal-format
     *       name %qname; #IMPLIED
     *       decimal-separator %char; "."
     *       grouping-separator %char; ","
     *       infinity CDATA "Infinity"
     *       minus-sign %char; "-"
     *       NaN CDATA "NaN"
     *       percent %char; "%"
     *       per-mille %char; "&#x2030;"
     *       zero-digit %char; "0"
     *       digit %char; "#"
     *       pattern-separator %char; ";"&GT;
     * </pre>
     *
     * http://www.w3.org/TR/xslt#format-number} to explain format-number in XSLT
     *      Specification xsl.usage advanced
     */
    static const std::string SYMBOL_DECIMAL_SEPARATOR;
    static const std::string SYMBOL_GROUPING_SEPARATOR;
    static const std::string SYMBOL_INFINITY;
    static const std::string SYMBOL_MINUS_SIGN;
    static const std::string SYMBOL_NAN;
    static const std::string SYMBOL_PERCENT;
    static const std::string SYMBOL_PER_MILLE;
    static const std::string SYMBOL_ZERO_DIGIT;
    static const std::string SYMBOL_DIGIT;
    static const std::string SYMBOL_PATTERN_SEPARATOR;
};

}  // namespace utils
}  // namespace jsonata