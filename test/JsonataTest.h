#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>

namespace jsonata {

class JsonataTest : public ::testing::Test {
public:
    struct TestOverride {
        std::string name;
        std::optional<bool> ignoreError;
        std::optional<nlohmann::ordered_json> alternateResult;
        std::optional<std::string> alternateCode;
        std::optional<int> overrideDepth;
        std::string reason;
    };

    struct TestOverrides {
        std::vector<TestOverride> override;
    };

private:
    static TestOverrides* testOverrides;
    bool debug = false;
    bool ignoreOverrides = false;
    int testFiles = 0;
    int testCases = 0;
    std::string groupDir = "jsonata/test/test-suite/groups/";

    // Helper methods
    bool testExpr(const std::string& expr, 
                  nlohmann::ordered_json data, 
                  const std::map<std::string, nlohmann::ordered_json>& bindings,
                  nlohmann::ordered_json expected, 
                  const std::optional<std::string>& code,
                  const std::optional<long>& timelimit = std::nullopt,
                  const std::optional<int>& depth = std::nullopt,
                  bool unordered = false);

    nlohmann::ordered_json toJson(const std::string& jsonStr);
    nlohmann::ordered_json readJson(const std::string& name);
    std::string preprocessJsonContent(const std::string& content);
    
    bool runTestSuite(const std::string& name);
    bool runTestCase(const std::string& name, const std::map<std::string, nlohmann::ordered_json>& testDef);
    
    void replaceNulls(nlohmann::ordered_json& o);

    static TestOverrides* getTestOverrides();
    TestOverride* getOverrideForTest(const std::string& name);
    
    bool runTestGroup(const std::string& group);

public:
    JsonataTest();
    virtual ~JsonataTest() = default;

    // Main public interface methods
    void runCase(const std::string& name);
    void runSubCase(const std::string& name, int subNr);
    
    // Test configuration
    void setDebug(bool debug) { this->debug = debug; }
    void setIgnoreOverrides(bool ignore) { this->ignoreOverrides = ignore; }
    
    // Test statistics
    int getTestFiles() const { return testFiles; }
    int getTestCases() const { return testCases; }
};

} // namespace jsonata
