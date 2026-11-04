#include <gtest/gtest.h>
#include <jsonata/JException.h>
#include <jsonata/Jsonata.h>

#include <any>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

// Regression tests for https://github.com/rayokota/jsonata-cpp/issues/10:
// closures hold their defining frame, so binding one into that frame used to
// form a shared_ptr cycle that leaked the frame, the closure and the input on
// every evaluate() call.

namespace jsonata {
namespace {

using nlohmann::ordered_json;

// Registers $probe() on an expression. Each call records weak references to
// the frame it was called from, every enclosing frame up to (but excluding)
// the instance environment, and every lambda passed to it, then returns true.
// After evaluate() returns, all of them should have been freed.
class FrameProbe {
  public:
    explicit FrameProbe(Jsonata& expr) : root_(expr.getEnvironment().get()) {
        expr.registerFunction(
            "probe", JFunction([this](const Utils::JList& args, const std::any&,
                                      std::shared_ptr<Frame> env) -> std::any {
                for (auto frame = env; frame && frame.get() != root_;
                     frame = frame->getParent()) {
                    frames_.push_back(frame);
                }
                for (const auto& arg : args) {
                    if (arg.type() == typeid(std::shared_ptr<Parser::Symbol>)) {
                        lambdas_.push_back(
                            std::any_cast<std::shared_ptr<Parser::Symbol>>(
                                arg));
                    }
                }
                return true;
            }));
    }

    void expectReleased() const {
        ASSERT_FALSE(frames_.empty()) << "$probe() was never called";
        for (size_t i = 0; i < frames_.size(); ++i) {
            EXPECT_TRUE(frames_[i].expired())
                << "frame " << i << " is still alive";
        }
        for (size_t i = 0; i < lambdas_.size(); ++i) {
            EXPECT_TRUE(lambdas_[i].expired())
                << "lambda " << i << " is still alive";
        }
    }

  private:
    const Frame* root_;
    std::vector<std::weak_ptr<Frame>> frames_;
    std::vector<std::weak_ptr<Parser::Symbol>> lambdas_;
};

// Evaluates the expression twice on the same instance, checking the result
// each time (so cleanup after the first call can't break the second) and
// that every probed frame and lambda has been freed.
void expectReleased(const std::string& expression, const ordered_json& expected,
                    const ordered_json& input =
                        ordered_json::parse(R"({"a":1,"b":{"c":"d"}})")) {
    Jsonata expr(expression);
    FrameProbe probe(expr);
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(expr.evaluate(input), expected) << "evaluation " << i;
    }
    probe.expectReleased();
}

TEST(FrameLifetimeTest, lambdaBoundInBlock) {
    expectReleased("($f := function($s){$s}; $probe($f); 1)", 1);
}

TEST(FrameLifetimeTest, lambdaBoundAtTopLevel) {
    expectReleased("$probe($f := function($s){$s})", true);
}

TEST(FrameLifetimeTest, recursiveLambda) {
    expectReleased(
        "($fact := function($n){ $n <= 1 ? ($probe(); 1) : $n * $fact($n - 1) "
        "}; $fact(5))",
        120);
}

TEST(FrameLifetimeTest, tailRecursiveLambda) {
    expectReleased(
        "($loop := function($n){ $n = 0 ? $probe() : $loop($n - 1) }; "
        "$loop(100))",
        true);
}

TEST(FrameLifetimeTest, lambdaReturnedFromBlock) {
    expectReleased("($g := (function($x){ $probe() ? $x : 0 }); $g(7))", 7);
}

TEST(FrameLifetimeTest, lambdaStoredInObject) {
    expectReleased(
        "($o := {\"f\": function($x){ $probe() ? $x : 0 }}; $h := $o.f; "
        "$h(3))",
        3);
}

TEST(FrameLifetimeTest, partialApplication) {
    expectReleased(
        "($add := function($a, $b){ $probe() ? $a + $b : 0 }; "
        "$inc := $add(1, ?); $inc(2))",
        3);
}

TEST(FrameLifetimeTest, higherOrderFunction) {
    expectReleased(
        "($sq := function($v){ $probe() ? $v * $v : 0 }; "
        "$map([1, 2, 3], $sq))",
        ordered_json::parse("[1, 4, 9]"));
}

TEST(FrameLifetimeTest, transform) {
    expectReleased("($t := |$|{\"y\": true}|; $probe($t); $t({\"x\": 1}))",
                   ordered_json::parse(R"({"x": 1, "y": true})"));
}

TEST(FrameLifetimeTest, eval) {
    expectReleased("($f := function(){1}; $eval(\"$probe()\"))", true);
}

// Enough frames to trigger compaction of the per-evaluation frame registry.
TEST(FrameLifetimeTest, manyFrames) {
    expectReleased(
        "($f := function($v){ $v * 2 }; $probe($f); "
        "$sum($map([1..5000], $f)))",
        25005000);
}

TEST(FrameLifetimeTest, exceptionPath) {
    Jsonata expr("($f := function($s){$s}; $probe($f); $error(\"boom\"))");
    FrameProbe probe(expr);
    EXPECT_THROW(expr.evaluate(ordered_json::parse(R"({"a":1})")), JException);
    probe.expectReleased();
    // The thread-local evaluation context must not keep the input or frames
    // alive after a failed evaluation either.
    EXPECT_EQ(expr.getCurrentEnvironment(), nullptr);
    EXPECT_FALSE(expr.getCurrentInput().has_value());
}

TEST(FrameLifetimeTest, unorderedJson) {
    Jsonata expr("($f := function($s){$s}; $probe($f); a)");
    FrameProbe probe(expr);
    auto input = nlohmann::json::parse(R"({"a":1,"b":{"c":"d"}})");
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(expr.evaluate(input), nlohmann::json(1));
    }
    probe.expectReleased();
}

// A custom function that runs a separate top-level evaluation must not
// disturb the outer one: each evaluation's frames are released when that
// evaluation returns, and the outer one still completes normally.
TEST(FrameLifetimeTest, nestedEvaluation) {
    Jsonata inner("($g := function(){ $probe() }; $g())");
    FrameProbe innerProbe(inner);

    Jsonata outer("($f := function($s){$s}; $probe($f); $inner() ? a : 0)");
    FrameProbe outerProbe(outer);
    outer.registerFunction(
        "inner",
        JFunction([&inner, &innerProbe](const Utils::JList&, const std::any&,
                                        std::shared_ptr<Frame>) -> std::any {
            auto result = inner.evaluate(nullptr).get<bool>();
            // Released as soon as the inner evaluate() returns, not when the
            // outer one does.
            innerProbe.expectReleased();
            return result;
        }));

    EXPECT_EQ(outer.evaluate(ordered_json::parse(R"({"a":5})")), 5);
    outerProbe.expectReleased();
}

// Frames supplied by the caller are copied from, never cleared.
TEST(FrameLifetimeTest, callerBindingsUntouched) {
    Jsonata expr("($f := function(){ $x }; $f())");
    auto bindings = expr.createFrame();
    bindings->bind("x", std::string("hi"));
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(expr.evaluate(nullptr, bindings), "hi");
    }
    EXPECT_EQ(std::any_cast<std::string>(bindings->lookup("x")), "hi");
}

}  // namespace
}  // namespace jsonata
