#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace jsonata {

class SignatureTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(SignatureTest, testParametersAreConvertedToArrays) {
    Jsonata expr("$greet(1,null,3)");

    JFunction greetFn;
    greetFn.signature = std::make_shared<utils::Signature>("<a?a?a?a?>", "greet");
    greetFn.implementation = [](const Utils::JList& args, const std::any&, std::shared_ptr<Frame>) -> std::any {
        auto formatOne = [](const std::any& a) -> std::string {
            if (a.type() == typeid(Utils::JList)) {
                const auto& arr = std::any_cast<Utils::JList>(a);
                if (arr.empty()) return "[null]";
                const auto& el = arr[0];
                if (!el.has_value()) return "[null]";
                if (el.type() == typeid(double)) return "[" + std::to_string(static_cast<int>(std::any_cast<double>(el))) + "]";
                if (el.type() == typeid(long long)) return "[" + std::to_string(std::any_cast<long long>(el)) + "]";
                if (el.type() == typeid(long)) return "[" + std::to_string(std::any_cast<long>(el)) + "]";
                if (el.type() == typeid(int)) return "[" + std::to_string(std::any_cast<int>(el)) + "]";
                if (el.type() == typeid(std::nullptr_t)) return "[null]";
            }
            return "[]";
        };

        std::string out = "[";
        for (size_t i = 0; i < 4; ++i) {
            if (i > 0) out += ", ";
            out += (i < args.size()) ? formatOne(args[i]) : std::string("[null]");
        }
        out += "]";
        return out;
    };
    expr.registerFunction("greet", greetFn);

    auto result = expr.evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "[[1], [null], [3], [null]]");
}

TEST_F(SignatureTest, testError) {
    Jsonata expr("$foo()");

    JFunction fooFn;
    fooFn.signature = std::make_shared<utils::Signature>("(sao)", "foo");
    fooFn.implementation = [](const Utils::JList&, const std::any&, std::shared_ptr<Frame>) -> std::any {
        return std::any{};
    };
    expr.registerFunction("foo", fooFn);

    EXPECT_THROW(expr.evaluate(nullptr), jsonata::JException);
    EXPECT_THROW(expr.evaluate(nlohmann::ordered_json(true)), jsonata::JException);
}

TEST_F(SignatureTest, testVarArg) {
    Jsonata expression("$sumvar(1,2,3)");

    JFunction sumFn;
    sumFn.signature = std::make_shared<utils::Signature>("<n+:n>", "sumvar");
    sumFn.implementation = [](const Utils::JList& args, const std::any&, std::shared_ptr<Frame>) -> std::any {
        long long sum = 0;
        for (const auto& a : args) {
            if (!a.has_value()) continue;
            if (a.type() == typeid(double)) sum += static_cast<long long>(std::any_cast<double>(a));
            else if (a.type() == typeid(long long)) sum += std::any_cast<long long>(a);
            else if (a.type() == typeid(long)) sum += std::any_cast<long>(a);
            else if (a.type() == typeid(int)) sum += std::any_cast<int>(a);
        }
        return sum;
    };
    expression.registerFunction("sumvar", sumFn);

    auto result = expression.evaluate(nullptr);
    ASSERT_TRUE(result.is_number_integer());
    EXPECT_EQ(result.get<long long>(), 6);
}

} // namespace jsonata
