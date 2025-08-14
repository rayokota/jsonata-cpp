#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <memory>
#include <string>
#include <sstream>
#include <functional>

namespace jsonata {

class SerializationTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    class SerializableExpression {
    public:
        std::string expression;
        std::unique_ptr<Jsonata> jsonata;

        explicit SerializableExpression(const std::string& expr) { init(expr); }

        void init(const std::string& expr) {
            expression = expr;
            jsonata = std::make_unique<Jsonata>(expr);
            JFunction hi;
            hi.implementation = [](const Utils::JList&, const std::any&, std::shared_ptr<Frame>) -> std::any {
                return std::string("hello world");
            };
            jsonata->registerFunction("hi", hi);
        }

        std::string serialize() const { return expression; }
        static SerializableExpression deserialize(const std::string& s) { return SerializableExpression(s); }
    };
};

TEST_F(SerializationTest, testJFunction) {
    Jsonata expr("$foo");
    JFunction foo;
    foo.implementation = [](const Utils::JList&, const std::any&, std::shared_ptr<Frame>) -> std::any {
        return std::any{};
    };
    expr.registerFunction("foo", foo);

    auto result = expr.evaluate(nullptr);
    // null result serializes to "null"
    EXPECT_TRUE(result.is_null());
}

TEST_F(SerializationTest, testSerializable) {
    SerializableExpression expr("$hi() & '!'");
    auto result = expr.jsonata->evaluate(nullptr);
    ASSERT_TRUE(result.is_string());
    EXPECT_EQ(result.get<std::string>(), "hello world!");

    std::string serialized = expr.serialize();
    SerializableExpression clone = SerializableExpression::deserialize(serialized);

    auto originalResult = expr.jsonata->evaluate(nullptr);
    auto cloneResult = clone.jsonata->evaluate(nullptr);

    ASSERT_TRUE(originalResult.is_string());
    ASSERT_TRUE(cloneResult.is_string());
    EXPECT_EQ(originalResult.get<std::string>(), cloneResult.get<std::string>());
}

} // namespace jsonata
