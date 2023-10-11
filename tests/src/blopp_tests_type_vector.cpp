#include "blopp_test.hpp"

TEST(type_object_vector, write_read_vector_bool) {
    const auto input = std::vector<bool>{ true, false, true, false, false, true };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::vector<bool>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    ASSERT_EQ(output.size(), size_t{ 6 });
    EXPECT_TRUE(output.at(0));
    EXPECT_FALSE(output.at(1));
    EXPECT_TRUE(output.at(2));
    EXPECT_FALSE(output.at(3));
    EXPECT_FALSE(output.at(4));
    EXPECT_TRUE(output.at(5));
}

TEST(type_object_vector, write_read_vector_int32) {
    const auto input = std::vector<int32_t>{ 1337, -1337, 0, 123 };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::vector<int32_t>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    ASSERT_EQ(output.size(), size_t{ 4 });
    EXPECT_EQ(output.at(0), int32_t{ 1337 });
    EXPECT_EQ(output.at(1), int32_t{ -1337 });
    EXPECT_EQ(output.at(2), int32_t{ 0 });
    EXPECT_EQ(output.at(3), int32_t{ 123 });
}

TEST(type_object_vector, write_read_vector_unique_ptr) {
    auto input = std::vector<std::unique_ptr<int32_t>>{};
    input.emplace_back(nullptr);
    input.emplace_back(std::make_unique<int32_t>(123));
    input.emplace_back(nullptr);

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::vector<std::unique_ptr<int32_t>>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    ASSERT_EQ(output.size(), size_t{ 3 });
    EXPECT_EQ(output.at(0), nullptr);
    EXPECT_NE(output.at(1), nullptr);
    EXPECT_EQ((*output.at(1)), int32_t{ 123 });
    EXPECT_EQ(output.at(2), nullptr);
}