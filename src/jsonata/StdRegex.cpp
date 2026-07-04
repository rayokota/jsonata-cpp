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
#include "jsonata/StdRegex.h"

namespace jsonata {

namespace {

std::regex_constants::syntax_option_type toSyntaxOptions(RegexFlags flags) {
    auto options = std::regex_constants::ECMAScript;
    if (flags.caseInsensitive) {
        options |= std::regex_constants::icase;
    }
    // Note: the 'm' (multiline) flag is intentionally not translated here,
    // preserving jsonata-cpp's pre-existing std::regex behavior exactly.
    // RegexFlags.multiline is still populated for custom engines that do
    // want to honor it.
    return options;
}

}  // namespace

StdRegex::StdRegex(const std::string& pattern, RegexFlags flags)
    : regex_(pattern, toSyntaxOptions(flags)) {}

RegexMatch StdRegex::toRegexMatch(const std::smatch& m) {
    RegexMatch result;
    result.text = m.str();
    result.position = static_cast<size_t>(m.position());
    result.length = static_cast<size_t>(m.length());
    for (size_t g = 1; g < m.size(); ++g) {
        if (m[g].matched) {
            result.groups.push_back(m[g].str());
        } else {
            result.groups.push_back(std::nullopt);
        }
    }
    return result;
}

bool StdRegex::test(const std::string& str) const {
    return std::regex_search(str, regex_);
}

std::optional<RegexMatch> StdRegex::findFirst(const std::string& str) const {
    std::smatch m;
    if (!std::regex_search(str, m, regex_)) {
        return std::nullopt;
    }
    return toRegexMatch(m);
}

std::vector<RegexMatch> StdRegex::findAll(const std::string& str) const {
    std::vector<RegexMatch> results;
    std::sregex_iterator it(str.begin(), str.end(), regex_);
    std::sregex_iterator end;
    for (; it != end; ++it) {
        results.push_back(toRegexMatch(*it));
    }
    return results;
}

std::vector<std::string> StdRegex::split(const std::string& str) const {
    std::vector<std::string> results;
    std::sregex_token_iterator it(str.begin(), str.end(), regex_, -1);
    std::sregex_token_iterator end;
    for (; it != end; ++it) {
        results.push_back(*it);
    }
    return results;
}

RegexEngine defaultRegexEngine() {
    return [](const std::string& pattern, RegexFlags flags) -> std::shared_ptr<IRegex> {
        return std::make_shared<StdRegex>(pattern, flags);
    };
}

}  // namespace jsonata
