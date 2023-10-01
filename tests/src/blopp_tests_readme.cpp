#include "blopp_test.hpp"

// Your data structures.
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
            .map(value.price);
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

TEST(blopp_readme, main) {
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
    ASSERT_TRUE(output_result);
}