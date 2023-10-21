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
    TEST(type_unique_ptr, ok_empty) {
        const auto input = std::unique_ptr<int32_t>{};
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::unique_ptr<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_FALSE(output_result->value);
    }
    TEST(type_unique_ptr, ok_int32) {
        const auto input = std::make_unique<int32_t>(123);
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::unique_ptr<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_TRUE(output_result->value);
        EXPECT_EQ((*output_result->value), int32_t{ 123 });
    }

    TEST(type_unique_ptr, ok_struct) {
        const auto input = std::make_unique<test_struct>(123, false, 1.0f);
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::unique_ptr<test_struct>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_TRUE(output_result->value);
        EXPECT_EQ(output_result->value->value_1, int32_t{ 123 });
        EXPECT_EQ(output_result->value->value_2, int32_t{ false });
        EXPECT_EQ(output_result->value->value_3, float{ 1.0f });
    }
}