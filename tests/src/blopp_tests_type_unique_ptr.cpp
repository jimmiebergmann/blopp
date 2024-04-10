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

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::unique_ptr<int32_t>>(*write_result);
        ASSERT_TRUE(read_result);

        ASSERT_FALSE(read_result->value);
    }
    TEST(type_unique_ptr, ok_int32) {
        const auto input = std::make_unique<int32_t>(123);

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::unique_ptr<int32_t>>(*write_result);
        ASSERT_TRUE(read_result);

        ASSERT_TRUE(read_result->value);
        EXPECT_EQ((*read_result->value), int32_t{ 123 });
    }

    TEST(type_unique_ptr, ok_struct) {
        const auto input = std::make_unique<test_struct>(123, false, 1.0f);

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::unique_ptr<test_struct>>(*write_result);
        ASSERT_TRUE(read_result);

        ASSERT_TRUE(read_result->value);
        EXPECT_EQ(read_result->value->value_1, int32_t{ 123 });
        EXPECT_EQ(read_result->value->value_2, int32_t{ false });
        EXPECT_EQ(read_result->value->value_3, float{ 1.0f });
    }
}