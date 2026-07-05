/**
 * jsonata-cpp is the JSONata C++ reference port
 *
 * Copyright Dashjoin GmbH. https://dashjoin.com
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
#include "jsonata/Timebox.h"

#include "jsonata/JException.h"
#include "jsonata/Jsonata.h"
#include "jsonata/Parser.h"

namespace jsonata {

Timebox::Timebox(Frame& expr) : Timebox(expr, 5000L, 100) {}

Timebox::Timebox(Frame& expr, std::optional<int64_t> timeout,
                 std::optional<int64_t> maxDepth)
    : timeout_(timeout), maxDepth_(maxDepth), depth_(0) {
    startTime_ = std::chrono::steady_clock::now();
    initialize(expr);
}

void Timebox::initialize(Frame& expr) {
    // Register callbacks with the Frame using environment lookup (Java
    // reference: same logic as Timebox.java lines 51-60) Store the callbacks in
    // the frame's environment for lookup during evaluation
    EntryCallback entryCallback = [this](std::shared_ptr<Parser::Symbol> exp,
                                         const std::any& input,
                                         std::shared_ptr<Frame> env) {
        // Java reference: if (_env.isParallelCall) return; (Timebox.java line
        // 52)
        if (env->isParallelCall) return;
        this->onEvaluateEntry();
    };

    ExitCallback exitCallback =
        [this](std::shared_ptr<Parser::Symbol> exp, const std::any& input,
               std::shared_ptr<Frame> env, const std::any& result) {
            // Java reference: if (_env.isParallelCall) return; (Timebox.java
            // line 57)
            if (env->isParallelCall) return;
            this->onEvaluateExit();
        };

    // Store callbacks in the environment for lookup during evaluation
    expr.bind("__evaluate_entry", entryCallback);
    expr.bind("__evaluate_exit", exitCallback);
}

void Timebox::onEvaluateEntry() {
    depth_++;
    checkRunnaway();
}

void Timebox::onEvaluateExit() {
    depth_--;
    checkRunnaway();
}

void Timebox::checkRunnaway() {
    // Check stack depth
    if (maxDepth_.has_value() && depth_ > *maxDepth_) {
        throw JException("D1011", -1);
    }

    // Check timeout
    if (timeout_.has_value()) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           currentTime - startTime_)
                           .count();

        if (elapsed > *timeout_) {
            throw JException("D1012", -1, *timeout_);
        }
    }
}

}  // namespace jsonata