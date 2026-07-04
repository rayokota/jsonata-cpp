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
#include "RE2Engine.h"

#include <stdexcept>

using jsonata::IRegex;
using jsonata::RegexEngine;
using jsonata::RegexFlags;
using jsonata::RegexMatch;

namespace {

RE2::Options toRE2Options(RegexFlags flags) {
    RE2::Options options;
    options.set_case_sensitive(!flags.caseInsensitive);
    // RE2's one_line defaults to false (^/$ match at line boundaries); set it
    // true when the JSONata pattern did NOT request the 'm' flag, so ^/$
    // only match the start/end of the whole string.
    options.set_one_line(!flags.multiline);
    options.set_log_errors(false);
    return options;
}

}  // namespace

RE2Regex::RE2Regex(const std::string& pattern, RegexFlags flags)
    : re_(pattern, toRE2Options(flags)) {
    if (!re_.ok()) {
        throw std::runtime_error("RE2 compile error: " + re_.error());
    }
}

RegexMatch RE2Regex::toRegexMatch(const re2::StringPiece& input,
                                  const std::vector<re2::StringPiece>& submatch,
                                  int numGroups) {
    RegexMatch result;
    result.text.assign(submatch[0].data(), submatch[0].size());
    result.position = static_cast<size_t>(submatch[0].data() - input.data());
    result.length = submatch[0].size();
    for (int g = 1; g <= numGroups; ++g) {
        if (submatch[g].data() == nullptr) {
            result.groups.push_back(std::nullopt);
        } else {
            result.groups.push_back(
                std::string(submatch[g].data(), submatch[g].size()));
        }
    }
    return result;
}

bool RE2Regex::test(const std::string& str) const {
    return RE2::PartialMatch(str, re_);
}

std::optional<RegexMatch> RE2Regex::findFirst(const std::string& str, size_t pos) const {
    if (pos > str.size()) {
        return std::nullopt;
    }
    int numGroups = re_.NumberOfCapturingGroups();
    std::vector<re2::StringPiece> submatch(numGroups + 1);
    re2::StringPiece input(str);
    // RE2::Match takes startpos against the full `input`, so ^ still only
    // matches true position 0 -- no special anchoring flags needed, unlike
    // std::regex when searching a sub-range.
    if (!re_.Match(input, pos, str.size(), RE2::UNANCHORED, submatch.data(),
                   numGroups + 1)) {
        return std::nullopt;
    }
    return toRegexMatch(input, submatch, numGroups);
}

std::vector<RegexMatch> RE2Regex::findAll(const std::string& str) const {
    std::vector<RegexMatch> results;
    size_t pos = 0;
    while (auto match = findFirst(str, pos)) {
        // Guard against zero-length matches to avoid an infinite loop.
        pos = match->position + (match->length == 0 ? 1 : match->length);
        results.push_back(std::move(*match));
    }
    return results;
}

std::vector<std::string> RE2Regex::split(const std::string& str) const {
    std::vector<std::string> result;
    size_t lastEnd = 0;
    for (const auto& match : findAll(str)) {
        result.push_back(str.substr(lastEnd, match.position - lastEnd));
        lastEnd = match.position + match.length;
    }
    result.push_back(str.substr(lastEnd));
    return result;
}

RegexEngine re2RegexEngine() {
    return [](const std::string& pattern, RegexFlags flags) -> std::shared_ptr<IRegex> {
        return std::make_shared<RE2Regex>(pattern, flags);
    };
}
