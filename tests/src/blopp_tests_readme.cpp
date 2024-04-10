#include "blopp_test.hpp"

namespace {
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
}


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
    }
};


namespace {
    TEST(readme, main) {
        const auto store_value = store{
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

        auto write_result = blopp::write(store_value);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<store>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;

        EXPECT_STREQ(output.name.c_str(), "Fruit store");
        ASSERT_EQ(output.products.size(), size_t{ 2 });
        {
            const auto& product = output.products.at(0);
            EXPECT_EQ(product.id, uint32_t{ 1 });
            EXPECT_STREQ(product.name.c_str(), "Apple");
            EXPECT_EQ(product.price, double{ 1.5 });
            EXPECT_EQ(product.unit, units::kg);
        }
        {
            const auto& product = output.products.at(1);
            EXPECT_EQ(product.id, uint32_t{ 2 });
            EXPECT_STREQ(product.name.c_str(), "Orange");
            EXPECT_EQ(product.price, double{ 4.1 });
            EXPECT_EQ(product.unit, units::lbs);
        }
    }
}