# jsonata-cpp

[![Build Status][github-actions-shield]][github-actions-link]

[github-actions-shield]: https://github.com/rayokota/jsonata-cpp/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=master
[github-actions-link]: https://github.com/rayokota/jsonata-cpp/actions

C++ implementation of JSONata.

This is a C++ port of the  [JSONata reference implementation](https://github.com/jsonata-js/jsonata), 
and also borrows from the [Dashjoin Java port](https://github.com/dashjoin/jsonata-java).

This implementation supports 100% of the language features of JSONata, using [nlohmann_json](https://github.com/nlohmann/json).
The JSONata documentation can be found [here](https://jsonata.org).


## Installation

Installation uses CMake as follows:

```
include(FetchContent)

FetchContent_Declare(
    jsonata
    URL https://github.com/rayokota/jsonata-cpp/archive/refs/tags/v0.1.0.zip
    URL_HASH SHA256=3ee1798f28a29d36ebbb273853979926716a384e4d491a6bd408e1f6de51760d  # Optional
)
FetchContent_MakeAvailable(jsonata)

# Use the library
target_link_libraries(your_target jsonata::jsonata)
```

## Getting Started

A very simple start:

```
#include <iostream>
#include <jsonata/Jsonata.h>
#include <nlohmann/json.hpp>

int main() {
    // Create the JSON data
    auto data = nlohmann::ordered_json::parse(R"({
        "example": [
            {"value": 4}, 
            {"value": 7}, 
            {"value": 13}
        ]
    })");

    // Create the JSONata expression
    jsonata::Jsonata expr("$sum(example.value)");
    
    // Evaluate the expression with the data
    auto result = expr.evaluate(data);
    
    // Print the result
    std::cout << "Result: " << result << std::endl;
    
    return 0;
}
```

## Guardrails

JSONata is Turing-complete, so it's possible to write expressions that loop forever or exhaust memory. If you evaluate
untrusted expressions, configure these guardrails (see the JS reference implementation's
[guardrails docs](https://docs.jsonata.org/guardrails) for more background):

- **Stack overflow** — the `stack` argument caps the depth of the eval-apply cycle. Exceeding it raises a `JException`
  with error `D1011`.
- **Excessive execution time** — the `timeout` argument (in milliseconds) catches tail-recursive infinite loops that
  `stack` can't. Exceeding it raises `D1012`.
- **Rogue regular expressions** — the `regexEngine` argument lets you swap in a linear-time engine (e.g.
  [RE2](https://github.com/google/re2)) to protect against [ReDoS](https://en.wikipedia.org/wiki/ReDoS), since the
  `timeout` guardrail can't interrupt a regex match in progress.

To swap in a linear-time regex engine, you can wrap RE2 with the `IRegex` interface. 
A complete, working example lives in `test/RE2Engine.h`/`.cpp`; the shape is:

```cpp
#include <jsonata/IRegex.h>
#include <re2/re2.h>

class RE2Regex : public jsonata::IRegex {
  public:
    RE2Regex(const std::string& pattern, jsonata::RegexFlags flags);
    bool test(const std::string& str) const override;
    std::optional<jsonata::RegexMatch> findFirst(const std::string& str, size_t pos) const override;
    std::vector<jsonata::RegexMatch> findAll(const std::string& str) const override;
    std::vector<std::string> split(const std::string& str) const override;
  private:
    RE2 re_;
};

jsonata::RegexEngine re2RegexEngine() {
    return [](const std::string& pattern, jsonata::RegexFlags flags) {
        return std::make_shared<RE2Regex>(pattern, flags);
    };
}
```

```cpp
#include <jsonata/Jsonata.h>

jsonata::Jsonata expr("<JSONata expression>", re2RegexEngine(),
                      /*timeout=*/1000, /*stack=*/500);
auto result = expr.evaluate(data);
```

## Running Tests

This project uses the repository of the reference implementation as a submodule. This allows referencing the current version of the unit tests. To clone this repository, run:

```
git clone --recurse-submodules https://github.com/rayokota/jsonata-cpp
```

To build and run the unit tests:

```
cmake -DJSONATA_BUILD_TESTS=ON -S . -B build
cmake --build build
ctest --test-dir build
```
