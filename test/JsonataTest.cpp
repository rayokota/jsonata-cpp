#include "JsonataTest.h"
#include <jsonata/Utils.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <set>

namespace jsonata {

// Static member initialization
JsonataTest::TestOverrides* JsonataTest::testOverrides = nullptr;

JsonataTest::JsonataTest() {
    // Initialize debug based on environment or other factors
    // For now, default to false
    debug = false;
    ignoreOverrides = false;
    testFiles = 0;
    testCases = 0;
}

bool JsonataTest::testExpr(const std::string& expr, 
                          nlohmann::ordered_json data, 
                          const std::map<std::string, nlohmann::ordered_json>& bindings,
                          nlohmann::ordered_json expected, 
                          const std::optional<std::string>& code,
                          const std::optional<long>& timelimit,
                          const std::optional<int>& depth,
                          bool unordered) {
    bool success = true;
    try {

        
        if (debug) {
            std::cout << "DEBUG: Full expression=[" << expr << "] Length=" << expr.length() << std::endl;
            std::cout << "Expr=" << expr << " Expected=";
            std::cout << expected.dump();
            std::cout << " ErrorCode=" << (code ? *code : "null") << std::endl;
            std::cout << "Data=" << data.dump() << std::endl;
        }


        // Create Jsonata instance with parsed expression, like Java does
        Jsonata jsonata(expr);
        
        std::shared_ptr<Frame> bindingFrame = nullptr;
        // Java reference: if (bindings!=null) - in C++, bindings is always valid, so always create frame
        // This matches Java logic exactly: create binding frame even for empty bindings map
        bindingFrame = jsonata.createFrame();
        for (const auto& [key, value] : bindings) {
            bindingFrame->bind(key, Jsonata::orderedJsonToAny(value));
        }
        
        // Java reference: set runtime bounds only when explicitly specified in test JSON
        // This matches the exact Java logic where tests like performance tests have no bounds
        if (timelimit.has_value() && depth.has_value()) {
            // Test specifies explicit runtime bounds (like tail_recursionTest.case005) - use them
            if (bindingFrame) {
                bindingFrame->setRuntimeBounds(*timelimit, *depth);
            }
        }
        // Note: If no timelimit/depth specified (like performanceTest.case000), NO runtime bounds are set
        // This allows performance tests to run without artificial limits, matching Java behavior
        
        // Use data directly as JsonValue for the evaluate call
        auto result = jsonata.evaluate(data, bindingFrame);
        
        if (code.has_value()) {
            success = false;  // Expected an error but didn't get one
        }

        bool expectedIsNull = expected.is_null();
        bool resultIsNull = result.is_null();

        if (!expectedIsNull) {
            // Compare using canonical nlohmann::json (unordered object semantics)
            nlohmann::json exp = nlohmann::json::parse(expected.dump());
            nlohmann::json res = nlohmann::json::parse(result.dump());
            
            if (unordered && exp.is_array() && res.is_array()) {
                // Simple unordered compare via multiset of dumps
                if (exp.size() != res.size()) success = false;
                else {
                    std::multiset<std::string> a, b;
                    for (const auto& el : exp) a.insert(el.dump());
                    for (const auto& el : res) b.insert(el.dump());
                    if (a != b) success = false;
                }
            } else {
                if (exp != res) {
                    success = false;
                }
            }
        }

        if (expectedIsNull && !resultIsNull) {
            success = false;
        }

        if (debug && success) {
            std::cout << "--Result = " << result.dump() << std::endl;
        }

        if (!success) {
            std::cout << "--Expr=" << expr << std::endl;
            std::cout << "--Expected=";
            std::cout << expected.dump() << std::endl;
            std::cout << " ErrorCode=" << (code ? *code : "null") << std::endl;
            std::cout << "--Data=" << data.dump() << std::endl;
            std::cout << "--Result = " << result.dump() << std::endl;
            std::cout << "WRONG RESULT" << std::endl;
        }

    } catch (const std::exception& e) {
        if (!code.has_value()) {
            std::cout << "--Expr=" << expr << " Expected=";
            std::cout << expected.dump();
            std::cout << " ErrorCode=" << (code ? *code : "null") << std::endl;
            std::cout << "--Data=" << data.dump() << std::endl;

            if (auto* je = dynamic_cast<const JException*>(&e)) {
                std::cout << "--Exception     = " << je->getError() << "  --> " << e.what() << std::endl;
            } else {
                std::cout << "--Exception     = " << e.what() << std::endl;
            }

            std::cout << "--ExpectedError = " << (code ? *code : "null") << " Expected=" << expected.dump() << std::endl;
            std::cout << "WRONG RESULT (exception)" << std::endl;
            success = false;
        }
        if (debug && success) {
            std::cout << "--Exception = " << e.what() << std::endl;
        }
    }
    return success;
}

nlohmann::ordered_json JsonataTest::toJson(const std::string& jsonStr) {
    return nlohmann::ordered_json::parse(jsonStr);
}

// Helper function to preprocess JSON content to fix common issues
std::string JsonataTest::preprocessJsonContent(const std::string& content) {
    // Check if content contains UTF-8 sequences (high bit set)
    // If so, skip preprocessing to avoid corrupting Unicode characters
    for (unsigned char c : content) {
        if (c > 127) {
            // Contains UTF-8, return original content unchanged
            return content;
        }
    }
    
    std::string result = content;
    
    // Replace unescaped control characters in strings
    // This is a simple approach - replace unescaped newlines, tabs, etc.
    bool inString = false;
    bool escaped = false;
    
    for (size_t i = 0; i < result.length(); ++i) {
        char c = result[i];
        
        if (escaped) {
            escaped = false;
            continue;
        }
        
        if (c == '\\' && inString) {
            escaped = true;
            continue;
        }
        
        if (c == '"') {
            inString = !inString;
            continue;
        }
        
        // If we're in a string and encounter control characters, escape them
        if (inString && c < 0x20) {
            switch (c) {
                case '\n':
                    result[i] = 'n';
                    result.insert(i, "\\");
                    i++; // Skip the inserted backslash
                    break;
                case '\r':
                    result[i] = 'r';
                    result.insert(i, "\\");
                    i++;
                    break;
                case '\t':
                    result[i] = 't';
                    result.insert(i, "\\");
                    i++;
                    break;
                default:
                    result[i] = ' '; // Replace with space
                    break;
            }
        }
    }
    
    return result;
}

  nlohmann::ordered_json JsonataTest::readJson(const std::string& name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + name);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    try {
        return nlohmann::ordered_json::parse(content);
    } catch (const std::exception&) {
        // Try a light preprocessing pass
        try {
            std::string processedContent = preprocessJsonContent(content);
            return nlohmann::ordered_json::parse(processedContent);
        } catch (const std::exception&) {
            // Fallback: salvage minimal fields from malformed JSON (e.g., invalid \u surrogates)
            // Extract expr
            auto findValueString = [](const std::string& src, const std::string& key) -> std::optional<std::string> {
                std::string needle = "\"" + key + "\"";
                size_t pos = src.find(needle);
                if (pos == std::string::npos) return std::nullopt;
                pos = src.find('"', pos + needle.size());
                if (pos == std::string::npos) return std::nullopt;
                size_t start = pos + 1;
                size_t end = src.find('"', start);
                if (end == std::string::npos) return std::nullopt;
                return src.substr(start, end - start);
            };

            nlohmann::ordered_json fallback;
            if (auto exprStr = findValueString(content, "expr")) {
                // Keep raw expression including escapes (Jsonata parser will handle \u escapes)
                fallback["expr"] = *exprStr;
            }
            if (auto datasetStr = findValueString(content, "dataset")) {
                fallback["dataset"] = *datasetStr;
            }
            if (auto codeStr = findValueString(content, "code")) {
                nlohmann::ordered_json err;
                err["code"] = *codeStr;
                fallback["error"] = err;
            }
            if (fallback.empty()) {
                // As a last resort, rethrow original error
                throw;
            }
            return fallback;
        }
    }
}

void JsonataTest::runCase(const std::string& name) {
    try {
        if (!runTestSuite(name)) {
            throw std::runtime_error("Test case failed: " + name);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in runCase(" << name << "): " << e.what() << std::endl;
        throw;
    }
}

void JsonataTest::runSubCase(const std::string& name, int subNr) {
    try {
        auto cases = readJson(name);
        if (!cases.is_array()) {
            throw std::runtime_error("Test file does not contain array: " + name);
        }
        
        const auto& caseArray = cases;
        if (subNr < 0 || subNr >= static_cast<int>(caseArray.size())) {
            throw std::runtime_error("Invalid subcase index: " + std::to_string(subNr));
        }
        
        auto testCase = caseArray[subNr];
        if (!testCase.is_object()) {
            throw std::runtime_error("Test case is not an object");
        }
        
        const auto& testObj = testCase;
        std::map<std::string, nlohmann::ordered_json> testDef;
        for (auto it = testObj.begin(); it != testObj.end(); ++it) testDef[it.key()] = it.value();
        
        if (!runTestCase(name + "_" + std::to_string(subNr), testDef)) {
            throw std::runtime_error("Test subcase failed: " + name + "_" + std::to_string(subNr));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in runSubCase(" << name << ", " << subNr << "): " << e.what() << std::endl;
        throw;
    }
}

bool JsonataTest::runTestSuite(const std::string& name) {
    try {
        testFiles++;
        bool success = true;

        auto testCase = readJson(name);
        if (testCase.is_null()) {
            return false;
        }
        
        if (testCase.is_array()) {
            // Some cases contain a list of test cases
            const auto& testArray = testCase;
            for (const auto& testDef : testArray) {
                if (testDef.is_object()) {
                    std::map<std::string, nlohmann::ordered_json> testDefMap;
                    for (auto it = testDef.begin(); it != testDef.end(); ++it) testDefMap[it.key()] = it.value();
                    std::cout << "Running sub-test" << std::endl;
                    success &= runTestCase(name, testDefMap);
                }
            }
        } else if (testCase.is_object()) {
            const auto& testObj = testCase;
            std::map<std::string, nlohmann::ordered_json> testDefMap;
            for (auto it = testObj.begin(); it != testObj.end(); ++it) testDefMap[it.key()] = it.value();
            success &= runTestCase(name, testDefMap);
        }
        
        return success;
    } catch (const std::exception& e) {
        std::cerr << "Error in runTestSuite(" << name << "): " << e.what() << std::endl;
        return false;
    }
}

void JsonataTest::replaceNulls(nlohmann::ordered_json& o) {
    if (o.is_array()) {
        for (auto& item : o) replaceNulls(item);
    } else if (o.is_object()) {
        for (auto it = o.begin(); it != o.end(); ++it) replaceNulls(it.value());
    } else if (o.is_null()) {
        o = "__JSON_NULL_VALUE__";
    }
}

JsonataTest::TestOverrides* JsonataTest::getTestOverrides() {
    if (testOverrides != nullptr) {
        return testOverrides;
    }

    try {
        // Try different possible paths for test-overrides.json
        std::vector<std::string> possiblePaths = {
            "test/test-overrides.json",
            "../test/test-overrides.json",
            "../../test/test-overrides.json"
        };

        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

        std::ifstream file;
        std::string actualPath;
        for (const auto& path : possiblePaths) {
            file.open(path);
            if (file.is_open()) {
                actualPath = path;
                break;
            }
        }
        
        if (!file.is_open()) {
            std::cerr << "Warning: Cannot find test-overrides.json, using empty overrides" << std::endl;
            testOverrides = new TestOverrides();
            return testOverrides;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        
        auto jsonValue = nlohmann::ordered_json::parse(content);
        if (!jsonValue.is_object()) {
            std::cerr << "Warning: Invalid test-overrides.json format, using empty overrides" << std::endl;
            testOverrides = new TestOverrides();
            return testOverrides;
        }
        const auto& root = jsonValue;
        auto overrideIt = root.find("override");
        if (overrideIt == root.end() || !overrideIt->is_array()) {
            std::cerr << "Warning: Missing or invalid 'override' array in test-overrides.json, using empty overrides" << std::endl;
            testOverrides = new TestOverrides();
            return testOverrides;
        }

        testOverrides = new TestOverrides();
        const auto& overrideArray = *overrideIt;
        for (const auto& item : overrideArray) {
            if (!item.is_object()) continue;

            const auto& overrideObj = item;
            TestOverride to;

            auto nameIt = overrideObj.find("name");
            if (nameIt != overrideObj.end() && nameIt->is_string()) {
                to.name = nameIt->get<std::string>();
            }

            auto ignoreErrorIt = overrideObj.find("ignoreError");
            if (ignoreErrorIt != overrideObj.end() && ignoreErrorIt->is_boolean()) {
                to.ignoreError = ignoreErrorIt->get<bool>();
            }

            auto alternateResultIt = overrideObj.find("alternateResult");
            if (alternateResultIt != overrideObj.end()) {
                to.alternateResult = *alternateResultIt;
            }

            auto alternateCodeIt = overrideObj.find("alternateCode");
            if (alternateCodeIt != overrideObj.end() && alternateCodeIt->is_string()) {
                to.alternateCode = alternateCodeIt->get<std::string>();
            }

            // Optional depth override
            auto depthOverrideIt = overrideObj.find("depth");
            if (depthOverrideIt != overrideObj.end()) {
                const auto& d = *depthOverrideIt;
                if (d.is_number_float()) {
                    to.overrideDepth = static_cast<int>(d.get<double>());
                } else if (d.is_number_integer()) {
                    to.overrideDepth = static_cast<int>(d.get<long long>());
                } else if (d.is_number_unsigned()) {
                    to.overrideDepth = static_cast<int>(d.get<unsigned long long>());
                }
            }

            auto reasonIt = overrideObj.find("reason");
            if (reasonIt != overrideObj.end() && reasonIt->is_string()) {
                to.reason = reasonIt->get<std::string>();
            }

            testOverrides->override.push_back(to);
        }
        
        std::cout << "Loaded " << testOverrides->override.size() << " test overrides from " << actualPath << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Warning: Error loading test overrides: " << e.what() << ", using empty overrides" << std::endl;
        if (!testOverrides) {
            testOverrides = new TestOverrides();
        }
    }
    
    return testOverrides;
}

  JsonataTest::TestOverride* JsonataTest::getOverrideForTest(const std::string& name) {
    if (ignoreOverrides) return nullptr;

    try {
        TestOverrides* tos = getTestOverrides();
        if (!tos) return nullptr;
        
        for (auto& to : tos->override) {
            const std::string& needle = to.name;
            if (name.size() >= needle.size() &&
                name.compare(name.size() - needle.size(), needle.size(), needle) == 0) {
                return &to;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in getOverrideForTest: " << e.what() << std::endl;
    }
    return nullptr;
}

bool JsonataTest::runTestCase(const std::string& name, const std::map<std::string, nlohmann::ordered_json>& testDef) {
    try {

        testCases++;
        if (debug) {
            std::cout << "\nRunning test " << name << std::endl;
        }

        // Extract expression
        auto exprIt = testDef.find("expr");
        std::string expr;
        if (exprIt != testDef.end() && exprIt->second.is_string()) {
            expr = exprIt->second.get<std::string>();
            // DEBUG output removed to prevent binary characters in stdout
        } else {
            // Check for expr-file
            auto exprFileIt = testDef.find("expr-file");
            if (exprFileIt != testDef.end() && exprFileIt->second.is_string()) {
                std::string exprFile = exprFileIt->second.get<std::string>();
                size_t lastSlash = name.find_last_of("/");
                std::string fileName = (lastSlash != std::string::npos) ? 
                    name.substr(0, lastSlash) + "/" + exprFile : exprFile;
                
                std::ifstream file(fileName);
                if (file.is_open()) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();
                    expr = buffer.str();
                }
            }
        }

        // Extract dataset
        std::string dataset;
        auto datasetIt = testDef.find("dataset");
        if (datasetIt != testDef.end() && datasetIt->second.is_string()) {
            dataset = datasetIt->second.get<std::string>();
        }

        // Extract bindings
        std::map<std::string, nlohmann::ordered_json> bindings;
        auto bindingsIt = testDef.find("bindings");
        if (bindingsIt != testDef.end() && bindingsIt->second.is_object()) {
            const auto& bindingsObj = bindingsIt->second;
            for (auto it = bindingsObj.begin(); it != bindingsObj.end(); ++it) {
                bindings[it.key()] = it.value();
            }
        }

        // Extract result
        nlohmann::ordered_json result; // null by default
        auto resultIt = testDef.find("result");
        if (resultIt != testDef.end()) {
            result = resultIt->second;
        }

        // Extract error code
        std::optional<std::string> code;
        auto codeIt = testDef.find("code");
        if (codeIt != testDef.end() && codeIt->second.is_string()) {
            code = codeIt->second.get<std::string>();
        } else {
            auto errorIt = testDef.find("error");
            if (errorIt != testDef.end() && errorIt->second.is_object()) {
                const auto& errorObj = errorIt->second;
                auto errorCodeIt = errorObj.find("code");
                if (errorCodeIt != errorObj.end() && errorCodeIt->is_string()) {
                    code = errorCodeIt->get<std::string>();
                }
            }
        }

        // Extract unordered flag
        bool unordered = false;
        auto unorderedIt = testDef.find("unordered");
        if (unorderedIt != testDef.end() && unorderedIt->second.is_boolean()) {
            unordered = unorderedIt->second.get<bool>();
        }

        // Extract timelimit and depth for runtime bounds (Java reference logic)
        std::optional<long> timelimit;
        std::optional<int> depth;
        
        auto timelimitIt = testDef.find("timelimit");
        if (timelimitIt != testDef.end()) {
            const auto& t = timelimitIt->second;
            if (t.is_number_float()) {
                timelimit = static_cast<long>(t.get<double>());
            } else if (t.is_number_integer()) {
                timelimit = static_cast<long>(t.get<long long>());
            } else if (t.is_number_unsigned()) {
                timelimit = static_cast<long>(t.get<unsigned long long>());
            }
        }
        
        auto depthIt = testDef.find("depth");
        if (depthIt != testDef.end()) {
            const auto& d = depthIt->second;
            if (d.is_number_float()) {
                depth = static_cast<int>(d.get<double>());
            } else if (d.is_number_integer()) {
                depth = static_cast<int>(d.get<long long>());
            } else if (d.is_number_unsigned()) {
                depth = static_cast<int>(d.get<unsigned long long>());
            }
        }

        // Extract data
        nlohmann::ordered_json data;
        auto dataIt = testDef.find("data");
        if (dataIt != testDef.end()) {
            std::cout << "DEBUG: Using inline data from test definition" << std::endl;
            data = dataIt->second;
        } else if (!dataset.empty()) {
            std::cout << "DEBUG: Loading dataset: " << dataset << std::endl;
            try {
                std::string datasetPath = "jsonata/test/test-suite/datasets/" + dataset + ".json";
                std::cout << "DEBUG: Dataset path: " << datasetPath << std::endl;
                data = readJson(datasetPath);
                std::cout << "DEBUG: Dataset loaded successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to load dataset: " << dataset << " - " << e.what() << std::endl;
            }
        } else {
            std::cout << "DEBUG: No data or dataset specified" << std::endl;
        }

        // Check for test overrides
        TestOverride* to = getOverrideForTest(name);
        if (to != nullptr) {
            std::cout << "OVERRIDE used : " << to->name << " for " << name 
                      << " reason=" << to->reason << std::endl;
            if (to->alternateResult) {
                result = to->alternateResult;
            }
            if (to->alternateCode) {
                code = *to->alternateCode;
            }
            if (to->overrideDepth) {
                depth = *to->overrideDepth;
            }
        }

        bool res;
        if (debug && expr == "(  $inf := function(){$inf()};  $inf())") {
            std::cerr << "DEBUG MODE: skipping infinity test: " << expr << std::endl;
            res = true;
        } else {
            res = testExpr(expr, data, bindings, result, code, timelimit, depth, unordered);
        }

        if (to != nullptr) {
            // There is an override/alternate result for this defined...
            if (!res && to->ignoreError && *to->ignoreError) {
                std::cout << "Test " << name << " failed, but override allows failure" << std::endl;
                res = true;
            }
        }

        return res;
    } catch (const std::exception& e) {
        std::cerr << "Error in runTestCase(" << name << "): " << e.what() << std::endl;
        return false;
    }
}

bool JsonataTest::runTestGroup(const std::string& group) {
    try {
        std::filesystem::path dir(groupDir + group);
        std::cout << "Run group " << dir << std::endl;
        
        if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
            std::cerr << "Group directory does not exist: " << dir << std::endl;
            return false;
        }
        
        std::vector<std::filesystem::path> files;
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                files.push_back(entry.path());
            }
        }
        std::sort(files.begin(), files.end());
        
        bool success = true;
        int count = 0, good = 0;
        for (const auto& f : files) {
            std::string name = f.filename().string();
            if (name.size() >= 5 && name.substr(name.size() - 5) == ".json") {
                bool res = runTestSuite(groupDir + group + "/" + name);
                success &= res;
                count++;
                if (res) good++;
            }
        }
        
        int successPercentage = (count > 0) ? (100 * good / count) : 100;
        std::cout << "Success: " << good << " / " << count << " = " << successPercentage << "%" << std::endl;
        
        // Note: In C++, we don't throw assertion errors like JUnit's assertEquals
        // Instead, we return the success status
        return success;
    } catch (const std::exception& e) {
        std::cerr << "Error in runTestGroup(" << group << "): " << e.what() << std::endl;
        return false;
    }
}

} // namespace jsonata