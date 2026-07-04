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

#include <re2/re2.h>

#include "jsonata/IRegex.h"

// Demonstration IRegex implementation backed by Google's RE2, showing that
// jsonata-cpp's regex engine hook (see IRegex.h/StdRegex.h) is not tied to
// std::regex. Lives under test/ since it's only built for the optional RE2
// test target (JSONATA_BUILD_RE2_TEST) and isn't part of the jsonata library.
class RE2Regex : public jsonata::IRegex {
  public:
    RE2Regex(const std::string& pattern, jsonata::RegexFlags flags);

    bool test(const std::string& str) const override;
    std::optional<jsonata::RegexMatch> findFirst(const std::string& str) const override;
    std::vector<jsonata::RegexMatch> findAll(const std::string& str) const override;
    std::vector<std::string> split(const std::string& str) const override;

  private:
    RE2 re_;
    static jsonata::RegexMatch toRegexMatch(
        const re2::StringPiece& input,
        const std::vector<re2::StringPiece>& submatch, int numGroups);
};

// Factory for use with Jsonata's constructor, e.g.
// Jsonata("/foo/i", re2RegexEngine()).
jsonata::RegexEngine re2RegexEngine();
