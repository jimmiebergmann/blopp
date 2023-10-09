# blopp
![version](https://img.shields.io/badge/Version-v0.1.0-blue) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT) ![GitHub Workflow Status (master)](https://img.shields.io/github/workflow/status/jimmiebergmann/blopp/Build/master?label=Github&logo=Github) [![AppVeyor Status (master)](https://img.shields.io/appveyor/ci/jimmiebergmann/blopp/master?label=AppVeyor&logo=AppVeyor)](https://ci.appveyor.com/project/jimmiebergmann/blopp/branch/master)  
Single header C++23 binary reader/writer.

## Installation
```
git clone https://github.com/jimmiebergmann/blopp.git
or copy include/blopp.hpp to your project.
```

## Requirements
- C++23 compiler, tested with:
  - Windows: Visual Studio 2022 
  - Linux: gcc-12
  - Mac: clang-16
- Cmake - Only for tests.
- Conan - Only for tests.


## Supported types
* Fundamental data types, including 
    `bool`, `char`, `int8_t`, `int16_t`, `int32_t`, `int64_t`, `uint8_t`, 
    `uint16_t`, `uint32_t`, `uint64_t`, `float`, `double`
* `enum`
* `struct` or `class`
* `std::string`, `std::vector`, `std::list`, `std::array`
* `std::unique_ptr`

## Example
``` cpp
#include "blopp.hpp"

// Your data structures.
enum class units : uint8_t {
    kg,
    lbs
};

struct product {
    uint32_t id;
    std::string name;
    double price;
};

struct store {
    std::string name;
    std::vector<product> products;
};

// Implement custom object mappers via specializations of blopp::object<T>.
template<>
struct blopp::object<product> {
    static void map(auto& context, auto& value) {
        context
            .map(value.id)
            .map(value.name)
            .map(value.price)
            .map(value.unit);
    }
};

template<>
struct blopp::object<store> {
    static void map(auto& context, auto& value) {
        context
            .map(value.name)
            .map(value.products);
    }
};

// Write and read your data structures.
int main()
{
    const auto input = store{
        .name = "Fruit store",
        .products = {
            {
                .id = 1,
                .name = "Apple",
                .price = 1.5,
                .unit = units::kg
            },
            {
                .id = 2,
                .name = "Orange",
                .price = 4.1,
                .unit = units::lbs
            }
        }
    };
    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<store>(input_bytes);
    if(!output_result) { /* Error handling. */}
    auto output = *output_result;
}
```

## Build tests
```
cd tests
conan install . --output-folder=build --build=missing --settings=build_type=Debug
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Debug
```
