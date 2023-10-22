#include "blopp_test.hpp"

namespace {
    struct test_struct {
        int32_t value_1;
        bool value_2;
        float value_3;
    };
}


template<>
struct blopp::object<test_struct> {
    static void map(auto& context, auto& value) {
        context.map(
            value.value_1,
            value.value_2,
            value.value_3);
    }
};


namespace {
    TEST(type_optional, ok_empty) {
        const auto input = std::optional<int32_t>{};

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::optional<int32_t>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output_value = read_result->value;
        ASSERT_FALSE(output_value.has_value());
    }
    TEST(type_optional, ok_int32) {
        const auto input = std::make_optional<int32_t>(123);

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::optional<int32_t>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output_value = read_result->value;
        ASSERT_TRUE(output_value.has_value());
        EXPECT_EQ(output_value.value(), int32_t{ 123 });
    }

    TEST(type_optional, ok_struct) {
        const auto input = std::make_optional<test_struct>(123, false, 1.0f);

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::optional<test_struct>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output_value = read_result->value;
        ASSERT_TRUE(output_value.has_value());
        EXPECT_EQ(output_value.value().value_1, int32_t{ 123 });
        EXPECT_EQ(output_value.value().value_2, int32_t{ false });
        EXPECT_EQ(output_value.value().value_3, float{ 1.0f });
    }
}