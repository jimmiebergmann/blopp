#include "blopp_test.hpp"

namespace {
    TEST(type_list, ok_list_int32_empty) {
        const auto input = std::list<int32_t>{ };

        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::list<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);

        auto& output = output_result->value;
        ASSERT_EQ(output.size(), size_t{ 0 });
    }

    TEST(type_list, ok_list_int32) {
        const auto input = std::list<int32_t>{ 1337, -1337, 0, 123 };

        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::list<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);

        auto& output = output_result->value;
        ASSERT_EQ(output.size(), size_t{ 4 });
        EXPECT_EQ((*std::next(output.begin(), 0)), int32_t{ 1337 });
        EXPECT_EQ((*std::next(output.begin(), 1)), int32_t{ -1337 });
        EXPECT_EQ((*std::next(output.begin(), 2)), int32_t{ 0 });
        EXPECT_EQ((*std::next(output.begin(), 3)), int32_t{ 123 });
    }
}