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

#include <regex>

#include "jsonata/IRegex.h"

namespace jsonata {

// Default IRegex implementation, backed by std::regex (ECMAScript grammar).
class StdRegex : public IRegex {
  public:
    StdRegex(const std::string& pattern, RegexFlags flags);

    bool test(const std::string& str) const override;
    std::optional<RegexMatch> findFirst(const std::string& str) const override;
    std::vector<RegexMatch> findAll(const std::string& str) const override;
    std::vector<std::string> split(const std::string& str) const override;

  private:
    std::regex regex_;
    static RegexMatch toRegexMatch(const std::smatch& m);
};

}  // namespace jsonata
