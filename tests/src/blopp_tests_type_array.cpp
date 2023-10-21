#include "blopp_test.hpp"

namespace {
    TEST(type_array, ok_array_int32) {
        auto input = std::array<int32_t, 5>{ 11, 22, 33, 44, 55 };
        auto input_bytes = blopp::write(input);

        auto output_result = blopp::read<std::array<int32_t, 5>>(input_bytes);
        ASSERT_TRUE(output_result);

        auto& output = output_result->value;
        EXPECT_EQ(output.at(0), int32_t{ 11 });
        EXPECT_EQ(output.at(1), int32_t{ 22 });
        EXPECT_EQ(output.at(2), int32_t{ 33 });
        EXPECT_EQ(output.at(3), int32_t{ 44 });
        EXPECT_EQ(output.at(4), int32_t{ 55 });
    }

    TEST(type_array, fail_mismatching_array_size) {
        auto input = std::array<int32_t, 5>{};
        auto input_bytes = blopp::write(input);

        auto output_result_1 = blopp::read<std::array<int32_t, 5>>(input_bytes);
        ASSERT_TRUE(output_result_1);

        auto output_result_2 = blopp::read<std::array<int32_t, 4>>(input_bytes);
        ASSERT_FALSE(output_result_2);
        ASSERT_EQ(output_result_2.error(), blopp::read_error_code::mismatching_array_size);

        auto output_result_3 = blopp::read<std::array<int32_t, 6>>(input_bytes);
        ASSERT_FALSE(output_result_3);
        ASSERT_EQ(output_result_3.error(), blopp::read_error_code::mismatching_array_size);
    }
}