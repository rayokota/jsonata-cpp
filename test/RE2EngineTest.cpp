/**
 * jsonata-cpp is the JSONata C++ reference port
 *
 * Copyright Robert Yokota
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
#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>

#include "RE2Engine.h"

using jsonata::Jsonata;

TEST(RE2EngineTest, Match) {
    // A single match is returned as a bare object, not wrapped in an array
    // (standard JSONata singleton-sequence unwrapping).
    Jsonata expr("$match(\"hello world\", /o w/)", re2RegexEngine());
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["match"], "o w");
    EXPECT_EQ(result["index"], 4);
}

TEST(RE2EngineTest, MatchCaseInsensitiveFlag) {
    Jsonata expr("$match(\"HELLO\", /hello/i)", re2RegexEngine());
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["match"], "HELLO");
}

TEST(RE2EngineTest, Contains) {
    Jsonata expr("$contains(\"hello\", /ell/)", re2RegexEngine());
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_boolean());
    EXPECT_TRUE(result.get<bool>());
}

TEST(RE2EngineTest, ReplaceWithString) {
    Jsonata expr("$replace(\"abc123def\", /[0-9]+/, \"#\")", re2RegexEngine());
    auto result = expr.evaluate(nullptr);
    EXPECT_EQ(result, "abc#def");
}

TEST(RE2EngineTest, ReplaceWithFunction) {
    Jsonata expr(
        "$replace(\"abc123\", /[0-9]+/, function($m) { $m.match & \"!\" })",
        re2RegexEngine());
    auto result = expr.evaluate(nullptr);
    EXPECT_EQ(result, "abc123!");
}

TEST(RE2EngineTest, Split) {
    Jsonata expr("$split(\"a1b2c3\", /[0-9]/)", re2RegexEngine());
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_array());
    std::vector<std::string> values;
    for (const auto& v : result) values.push_back(v.get<std::string>());
    EXPECT_EQ(values, (std::vector<std::string>{"a", "b", "c", ""}));
}

TEST(RE2EngineTest, RejectsBackreferences) {
    // Proves RE2 is actually compiling the pattern rather than silently
    // falling back to std::regex: backreferences can't run in RE2's
    // guaranteed-linear-time engine, so this must fail when the regex
    // literal is compiled (during expression construction).
    EXPECT_THROW(
        { Jsonata expr("$match(\"xaay\", /(a)\\1/)", re2RegexEngine()); },
        std::exception);
}

TEST(RE2EngineTest, DefaultEngineStillStdRegex) {
    // No engine argument -> falls back to the default std::regex-backed
    // engine, which *does* support backreferences. Confirms the default
    // path is unaffected.
    Jsonata expr("$match(\"xaay\", /(a)\\1/)");
    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_object());
    EXPECT_EQ(result["match"], "aa");
}
