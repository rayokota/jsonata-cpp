/**
 * JSONata C++ Test Generator
 * 
 * Ports the Java Generate.java functionality to C++
 * Reads JSONata test cases from jsonata/test/test-suite/groups and generates
 * individual C++ Google Test files in test/gen/
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>

using nlohmann::ordered_json;
namespace fs = std::filesystem;

class JsonataTestGenerator {
public:
    // Helper function to sanitize names for C++ identifiers
    static std::string sanitizeName(const std::string& name) {
        std::string result = name;
        std::replace(result.begin(), result.end(), '-', '_');
        std::replace(result.begin(), result.end(), '.', '_');
        return result;
    }

    // Helper function to read JSON file content
    static std::string readFileContent(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // Helper function to preprocess JSON content to fix common issues
    static std::string preprocessJsonContent(const std::string& content) {
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

    // Helper function to escape strings for C++ literals
    static std::string escapeString(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '\n': result += ' '; break;  // Replace newlines with spaces like Java
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c; break;
            }
        }
        return result;
    }

    // Extract expression from test case for comment
    static std::string getExpressionComment(const ordered_json& testCase) {
        if (testCase.is_object() && testCase.contains("expr") && testCase["expr"].is_string())
            return escapeString(testCase["expr"].get<std::string>());
        return "";
    }

    // Extract expression from std::any that contains JsonObject
    static std::string getExpressionCommentFromAny(const std::any&) { return ""; }

    // Generate test method for a single test case
    static void generateTestMethod(std::stringstream& output, 
                                   const std::string& suiteName,
                                   const std::string& caseName,
                                   int caseIndex,
                                   const std::string& expr) {
        std::string methodName;
        if (caseIndex >= 0) {
            methodName = sanitizeName(caseName) + "_case_" + std::to_string(caseIndex);
        } else {
            methodName = sanitizeName(caseName);
        }

        output << "// " << expr << "\n";
        output << "TEST_F(" << sanitizeName(suiteName) << "Test, " << methodName << ") {\n";
        
        if (caseIndex >= 0) {
            output << "    EXPECT_NO_THROW(runSubCase(\"jsonata/test/test-suite/groups/" << suiteName 
                   << "/" << caseName << ".json\", " << caseIndex << "));\n";
        } else {
            output << "    EXPECT_NO_THROW(runCase(\"jsonata/test/test-suite/groups/" << suiteName 
                   << "/" << caseName << ".json\"));\n";
        }
        
        output << "}\n\n";
    }

    // Extract expression from content using simple string parsing as fallback
    static std::string extractExpressionFallback(const std::string& content) {
        // Find the "expr" field manually
        size_t exprPos = content.find("\"expr\"");
        if (exprPos == std::string::npos) return "";
        
        size_t colonPos = content.find(":", exprPos);
        if (colonPos == std::string::npos) return "";
        
        size_t quoteStart = content.find("\"", colonPos);
        if (quoteStart == std::string::npos) return "";
        
        size_t quoteEnd = quoteStart + 1;
        int escapeCount = 0;
        while (quoteEnd < content.size()) {
            if (content[quoteEnd] == '\\') {
                escapeCount++;
            } else if (content[quoteEnd] == '"' && escapeCount % 2 == 0) {
                break;
            } else if (content[quoteEnd] != '\\') {
                escapeCount = 0;
            }
            quoteEnd++;
        }
        
        if (quoteEnd >= content.size()) return "";
        
        std::string expr = content.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        return escapeString(expr);
    }

    // Determine if content is an array by checking if it starts with '['
    static bool isJsonArray(const std::string& content) {
        for (char c : content) {
            if (std::isspace(c)) continue;
            return c == '[';
        }
        return false;
    }

    // Count test cases in an array by counting objects
    static int countArrayTestCases(const std::string& content) {
        int count = 0;
        bool inString = false;
        bool escaped = false;
        int braceLevel = 0;
        
        for (size_t i = 0; i < content.size(); ++i) {
            char c = content[i];
            
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
            
            if (inString) continue;
            
            if (c == '{') {
                if (braceLevel == 0) count++;
                braceLevel++;
            } else if (c == '}') {
                braceLevel--;
            }
        }
        
        return count;
    }

    // Process a single test case file
    static int processTestFile(std::stringstream& output,
                               const std::string& suiteName,
                               const std::string& filePath) {
        std::string fileName = fs::path(filePath).stem().string();
        std::string content = readFileContent(filePath);
        
        try {
            // First try parsing the content as-is
            auto jsonValue = ordered_json::parse(content);

            int testCount = 0;

            if (jsonValue.is_array()) {
                for (size_t i = 0; i < jsonValue.size(); ++i) {
                    std::string expr = getExpressionComment(jsonValue[i]);
                    generateTestMethod(output, suiteName, fileName, static_cast<int>(i), expr);
                    testCount++;
                }
            } else {
                std::string expr = getExpressionComment(jsonValue);
                generateTestMethod(output, suiteName, fileName, -1, expr);
                testCount++;
            }

            return testCount;
        } catch (const std::exception& e) {
            // Parsing failed - try preprocessing the content first
            std::cerr << "JSON parsing failed for " << filePath << ": " << e.what() << std::endl;
            std::cerr << "Trying with preprocessed content..." << std::endl;
            
            try {
                std::string processedContent = preprocessJsonContent(content);
                auto jsonValue = ordered_json::parse(processedContent);
                int testCount = 0;
                if (jsonValue.is_array()) {
                    for (size_t i = 0; i < jsonValue.size(); ++i) {
                        std::string expr = getExpressionComment(jsonValue[i]);
                        generateTestMethod(output, suiteName, fileName, static_cast<int>(i), expr);
                        testCount++;
                    }
                } else {
                    std::string expr = getExpressionComment(jsonValue);
                    generateTestMethod(output, suiteName, fileName, -1, expr);
                    testCount++;
                }
                return testCount;
            } catch (const std::exception& e2) {
                std::cerr << "Preprocessing also failed: " << e2.what() << std::endl;
            }
            
            // Both attempts failed - use fallback method to still generate tests
            std::cerr << "File content preview (first 200 chars): " << content.substr(0, 200) << "..." << std::endl;
            std::cerr << "Using fallback method to generate tests..." << std::endl;
            
            int testCount = 0;
            std::string expr = extractExpressionFallback(content);
            
            if (isJsonArray(content)) {
                // Multiple test cases in array
                int arrayCount = countArrayTestCases(content);
                for (int i = 0; i < arrayCount; ++i) {
                    generateTestMethod(output, suiteName, fileName, i, expr);
                    testCount++;
                }
            } else {
                // Single test case
                generateTestMethod(output, suiteName, fileName, -1, expr);
                testCount++;
            }
            
            return testCount;
        }
    }

public:
    static void generateTests(bool verbose = false) {
        // Create output directory
        fs::create_directories("test/gen");

        std::string suitesPath = "jsonata/test/test-suite/groups";
        
        if (!fs::exists(suitesPath)) {
            std::cerr << "Error: Test suite directory not found: " << suitesPath << std::endl;
            return;
        }

        int totalTests = 0;
        int totalSuites = 0;

        // Get all suite directories and sort them
        std::vector<fs::path> suiteDirs;
        for (const auto& entry : fs::directory_iterator(suitesPath)) {
            if (entry.is_directory()) {
                suiteDirs.push_back(entry.path());
            }
        }
        std::sort(suiteDirs.begin(), suiteDirs.end());

        for (const auto& suiteDir : suiteDirs) {
            std::string suiteName = suiteDir.filename().string();
            
            std::stringstream output;
            
            // Generate file header
            output << "#include <gtest/gtest.h>\n";
            output << "#include \"JsonataTest.h\"\n\n";
            
            output << "using namespace jsonata;\n\n";
            
            // Generate test class inheriting from JsonataTest
            output << "class " << sanitizeName(suiteName) << "Test : public JsonataTest {\n";
            output << "};\n\n";

            // Process all JSON files in the suite directory
            int suiteTestCount = 0;
            std::vector<fs::path> testFiles;
            
            for (const auto& entry : fs::directory_iterator(suiteDir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    testFiles.push_back(entry.path());
                }
            }
            std::sort(testFiles.begin(), testFiles.end());

            for (const auto& testFile : testFiles) {
                int count = processTestFile(output, suiteName, testFile.string());
                suiteTestCount += count;
            }

            totalTests += suiteTestCount;
            totalSuites++;

            // Write the generated test file
            std::string outputFileName = "test/gen/" + sanitizeName(suiteName) + "Test.cpp";
            std::ofstream outFile(outputFileName);
            if (outFile.is_open()) {
                outFile << output.str();
                outFile.close();
                
                if (verbose) {
                    std::cout << output.str() << std::endl;
                }
                
                std::cout << "Generated suite '" << suiteName << "' tests=" << suiteTestCount << std::endl;
            } else {
                std::cerr << "Error: Could not write to " << outputFileName << std::endl;
            }
        }

        std::cout << "Generated SUITES=" << totalSuites << " TOTAL=" << totalTests << std::endl;
    }
};

int main(int argc, char* argv[]) {
    bool verbose = (argc > 1) && (std::string(argv[1]).find("-v") == 0);
    
    try {
        JsonataTestGenerator::generateTests(verbose);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}