#include "blopp_test.hpp"

TEST(type_unique_ptr, ok_unique_ptr_empty) {
    const auto input = std::unique_ptr<int32_t>{};
    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::unique_ptr<int32_t>>(input_bytes);
    ASSERT_TRUE(output_result);
    EXPECT_FALSE(output_result->value);
}
TEST(type_unique_ptr, ok_unique_ptr) {
    const auto input = std::make_unique<int32_t>(123);
    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::unique_ptr<int32_t>>(input_bytes);
    ASSERT_TRUE(output_result);
    EXPECT_EQ((*output_result->value), int32_t{ 123 });
}
