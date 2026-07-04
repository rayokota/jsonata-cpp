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
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace jsonata {

// A single match, engine-agnostic (the equivalent of std::smatch, but not
// tied to std::regex so alternative engines such as RE2 can produce it too).
struct RegexMatch {
    std::string text;
    size_t position = 0;
    size_t length = 0;
    // groups[0] is capture group 1, matching std::smatch's [1] indexing
    // offset by the implicit full-match group 0.
    std::vector<std::optional<std::string>> groups;
};

// Flags parsed from a JSONata regex literal's /pattern/flags suffix. Engines
// translate these into their own native flag representation.
struct RegexFlags {
    bool caseInsensitive = false;
    bool multiline = false;
};

// Engine-agnostic compiled regex. JSONata regex literals (and patterns given
// as plain strings to functions like $match/$replace/$split) are compiled
// into this interface, so the evaluator never depends on std::regex
// directly. The default engine wraps std::regex (see StdRegex.h); a custom
// engine (e.g. RE2) can be injected via Jsonata's constructor.
class IRegex {
  public:
    virtual ~IRegex() = default;

    // True if the pattern matches anywhere in str. Backs $contains.
    virtual bool test(const std::string& str) const = 0;

    // The first match, if any. Backs $replace's first/limited-match paths.
    virtual std::optional<RegexMatch> findFirst(const std::string& str) const = 0;

    // All non-overlapping matches, in order. Backs $match and $contains.
    virtual std::vector<RegexMatch> findAll(const std::string& str) const = 0;

    // Splits str on every match, returning the segments between matches
    // (including empty leading/trailing segments). Backs $split.
    virtual std::vector<std::string> split(const std::string& str) const = 0;
};

// Compiles a pattern into an IRegex. Used both for JSONata regex literals
// and for patterns supplied as plain strings at runtime.
using RegexEngine = std::function<std::shared_ptr<IRegex>(
    const std::string& pattern, RegexFlags flags)>;

// The built-in std::regex-backed engine; this is jsonata-cpp's default and
// preserves its pre-existing regex behavior exactly.
RegexEngine defaultRegexEngine();

}  // namespace jsonata
