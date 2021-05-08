# blopp
![version](https://img.shields.io/badge/Version-v0.1.0-blue) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT) [![Travis CI](https://img.shields.io/travis/jimmiebergmann/blopp/main?label=Travis%20CI)](https://travis-ci.com/jimmiebergmann/blopp) [![AppVeyor](https://img.shields.io/appveyor/ci/jimmiebergmann/blopp/main?label=AppVeyor)](https://ci.appveyor.com/project/jimmiebergmann/blopp/branch/main)  
Single header C++17 binary serializer/deserializer with respect to member alignments.

## Installation
```
git clone https://github.com/jimmiebergmann/blopp.git
Include blopp/blopp.hpp in your project.
```

## Requirements
- C++17 compiler, tested with:
  - clang-8
  - gcc-8
  - mvc 19.28 (Visual Studio 19)
- Cmake - only for tests.

## Example
``` cpp
struct data
{
    int32_t a;
    bool b; // Example of alignment, offsetof(data, c) == 8, not 5.
    double c;

    // Add this function to your struct.
    template<typename t_context>
    static constexpr auto blopp_map(t_context context)
    {
        // Map data members in expected binary order.
        return context.map(
            blopp_map_member(data, a),
            blopp_map_member(data, b),
            blopp_map_member(data, c));
    }
};

int main()
{
    // Serializing data and deserializing it back.
    auto input = data{ 123, true, 128.75 };
    auto raw_input = blopp::serialize(input);
    auto output = blopp::deserialize<data>(raw_input);
}
```
