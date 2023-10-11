#include "blopp_test.hpp"

TEST(type_string, write_read_string) {
    auto input_bytes = blopp::write(std::string{ "Hello world" });
    auto output_result = blopp::read<std::string>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    EXPECT_STREQ(output.c_str(), "Hello world");
}

TEST(type_string, write_read_string_empty) {
    auto input_bytes = blopp::write(std::string{ "" });
    auto output_result = blopp::read<std::string>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    EXPECT_STREQ(output.c_str(), "");
}