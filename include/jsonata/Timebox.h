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
#pragma once

#include <any>
#include <chrono>
#include <functional>

namespace jsonata {

// Forward declarations
class Frame;

/**
 * Configure max runtime / max recursion depth.
 * See Frame.setRuntimeBounds - usually not used directly
 */
class Timebox {
  private:
    int64_t timeout_;  // timeout in milliseconds (default: 5000ms)
    int64_t maxDepth_;  // max recursion depth (default: 100)

    std::chrono::steady_clock::time_point startTime_;
    int64_t depth_;

  public:
    /**
     * Protect the process from a runaway expression
     * i.e. Infinite loop (tail recursion), or excessive stack growth
     *
     * Default timeout: 5000ms, default max depth: 100
     */
    Timebox(Frame& expr);

    /**
     * Protect the process from a runaway expression
     * with custom timeout and max depth
     */
    Timebox(Frame& expr, int64_t timeout, int64_t maxDepth);

    // Getters
    int64_t getTimeout() const { return timeout_; }
    int64_t getMaxDepth() const { return maxDepth_; }
    int64_t getCurrentDepth() const { return depth_; }

    // Runtime check method
    void checkRunnaway();

    // Callback handlers (called by Frame during evaluation)
    void onEvaluateEntry();
    void onEvaluateExit();

  private:
    void initialize(Frame& expr);
};

}  // namespace jsonata