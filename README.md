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
  - Windows: Visual Studio 2022 (17.6.5)
  - Linux: gcc-12 and clang-16 (libc++)
- Cmake - Only for tests.
- Conan - Only for tests.

## Example
``` cpp
#include "blopp.hpp"

// Your data structures.
struct product
{
    uint32_t id;
    std::string name;
    double price;
};

struct store
{
    std::string name;
    std::vector<product> products;
};

// Implement custom object mappers via specializations of blopp::object<T>.
template<>
struct blopp::object<product>
{
    static auto map(auto context, auto value)
    {
        return context
            .map(value.id)
            .map(value.name)
            .map(value.price);
    }
};

template<>
struct blopp::object<store>
{
    static auto map(auto context, auto value)
    {
        return context
            .map(value.id)
            .map(value.products);
    }
};

// Write and read your data structures.
int main()
{
    auto input = store{
        .name = "Fruit store",
        .products = {
            { 
                .id = 1,
                .name = "Apple",
                .price = 1.5
            },
            {
                .id = 2,
                .name = "Orange",
                .price = 2.1
            }
        }
    };
    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<store>(input_bytes);
    if(!output_result) { /* Error checking. */}
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
