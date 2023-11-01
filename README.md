# blopp
![version](https://img.shields.io/badge/Version-v0.1.0-blue) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT) ![GitHub Workflow Status (master)](https://img.shields.io/github/workflow/status/jimmiebergmann/blopp/Build/master?label=Github&logo=Github) [![AppVeyor Status (master)](https://img.shields.io/appveyor/ci/jimmiebergmann/blopp/master?label=AppVeyor&logo=AppVeyor)](https://ci.appveyor.com/project/jimmiebergmann/blopp/branch/master)  
Single header C++20 binary data serializer/deserializer.

See [format_specification.md](https://github.com/jimmiebergmann/blopp/blob/main/format_specification.md) for a detailed description of the file format.

## Installation
```
git clone https://github.com/jimmiebergmann/blopp.git
or copy include/blopp.hpp to your project.
```

## Requirements
- C++20 compiler, tested with:
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
* `struct`, `class`
* `std::basic_string`, `std::vector`, `std::list`, `std::array`, `c-style array`
* `std::map`, `std::multimap`
* `std::optional`, `std::unique_ptr`, `std::shared_ptr`
* `std::variant`

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
    units unit;
};

struct store {
    std::string name;
    std::vector<product> products;
};

// Implement custom object mappers via specializations of blopp::object<T>.
template<>
struct blopp::object<product> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.id,
            value.name,
            value.price,
            value.unit);
    }
};

template<>
struct blopp::object<store> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.name,
            value.products);

        // Same as:
        // context.map(value.name);
        // context.map(value.products);
    }
};

// Write and read your data structures.
int main()
{
    const auto store_input = store{
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

    auto write_result = blopp::write(store_input /*, file_path*/);
    if(!write_result) { /* Error handling. */}

    auto read_result = blopp::read<store>(*write_result /*or file_path*/);
    if(!read_result) { /* Error handling. */}

    auto& store_output = read_result->value;
}
```


## FAQ
#### Does `blopp::read` return `std::expected`?
Yes, if your compiler supports it, else a small std::expected-like class called `result_wrapper` is returned.

#### Why isn't c-style arrays supported at root? 
Due to limitations of std::expected.

#### How can I convert members of my custom struct while writing/reading? 
Use `context.template map_as<T>` instead of `context.map`.  
`blopp::read` and `blopp::write` will return blopp::write/read_error_code::`conversion_overflow` if conversion results in an integer over or under-flow.

#### How can I map my custom type without representing it as an object?
Use `format` instead of `map` method in your object template specialization. 
Return void or bool. Returning false will result in blopp::write/read_error_code::`user_defined_failure`.
Here's an example:

``` cpp
struct vec3 {
    float x, y, z;
};

template<>
struct blopp::object<vec3> {
    static auto format(auto& context, auto& value) {
        context.format(value.x, value.y, value.z);
    }
};
```

Writing this object will result in a file size of `15` bytes, compared to `26` if map method were used.  
Using `format` is great for small size objects, but is limited to fundamental and array data types only.

## Build tests
```
cd tests
conan install . --output-folder=build --build=missing --settings=build_type=Debug
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Debug
```
