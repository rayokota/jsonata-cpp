#include <gtest/gtest.h>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>
#include <jsonata/JException.h>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

namespace jsonata {

class ThreadTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ThreadTest, testReuse) {
    Jsonata expr("a");
    nlohmann::ordered_json data = nlohmann::ordered_json::object({{"a", 1}});

    auto result1 = expr.evaluate(data);
    ASSERT_TRUE(result1.is_number());
    EXPECT_EQ(result1.get<int>(), 1);

    auto result2 = expr.evaluate(data);
    ASSERT_TRUE(result2.is_number());
    EXPECT_EQ(result2.get<int>(), 1);
}

TEST_F(ThreadTest, testNow) {
    Jsonata now("$now()");
    auto r1 = now.evaluate(nullptr);
    std::this_thread::sleep_for(std::chrono::milliseconds(42));
    auto r2 = now.evaluate(nullptr);

    ASSERT_TRUE(r1.is_string());
    ASSERT_TRUE(r2.is_string());
    EXPECT_NE(r1.get<std::string>(), r2.get<std::string>());
}

TEST_F(ThreadTest, testReuseWithVariable) {
    Jsonata expr("($x := a; $wait(a); $x)");

    JFunction waitFn;
    waitFn.implementation = [](const Utils::JList& args, const std::any&, std::shared_ptr<Frame>) -> std::any {
        int sleepMs = 0;
        if (!args.empty() && args[0].has_value()) {
            if (args[0].type() == typeid(double)) sleepMs = static_cast<int>(std::any_cast<double>(args[0]));
            else if (args[0].type() == typeid(long long)) sleepMs = static_cast<int>(std::any_cast<long long>(args[0]));
            else if (args[0].type() == typeid(long)) sleepMs = static_cast<int>(std::any_cast<long>(args[0]));
            else if (args[0].type() == typeid(int)) sleepMs = std::any_cast<int>(args[0]);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
        return std::any{};
    };
    expr.registerFunction("wait", waitFn);

    auto outerFuture = std::async(std::launch::async, [&expr]() {
        nlohmann::ordered_json data = nlohmann::ordered_json::object({{"a", 100}});
        return expr.evaluate(data);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    nlohmann::ordered_json data30 = nlohmann::ordered_json::object({{"a", 30}});
    auto result30 = expr.evaluate(data30);
    ASSERT_TRUE(result30.is_number());
    EXPECT_EQ(result30.get<int>(), 30);

    auto outerResult = outerFuture.get();
    ASSERT_TRUE(outerResult.is_number());
    EXPECT_EQ(outerResult.get<int>(), 100);
}

TEST_F(ThreadTest, testAddEnvAndInput) {
    Jsonata expr("$eval('$count($keys($))')");

    nlohmann::ordered_json obj1 = nlohmann::ordered_json::object({{"input", 1}});
    nlohmann::ordered_json obj2 = nlohmann::ordered_json::object({{"input", 2}, {"other", 3}});

    const int count = 200; // keep the test fast

    // Now safe to use the same Jsonata instance across threads
    auto outerFuture = std::async(std::launch::async, [&expr, obj1, count]() {
        int sum = 0;
        for (int i = 0; i < count; i++) {
            auto result = expr.evaluate(obj1);
            if (result.is_number()) sum += result.get<int>();
        }
        return sum;
    });

    int sum = 0;
    for (int i = 0; i < count; i++) {
        auto result = expr.evaluate(obj2);
        if (result.is_number()) sum += result.get<int>();
    }

    int outerSum = outerFuture.get();
    EXPECT_EQ(outerSum, count);
    EXPECT_EQ(sum, 2 * count);
}

} // namespace jsonata
