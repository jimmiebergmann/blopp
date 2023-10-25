#include "blopp_test.hpp"
#include "blopp_test_binary_format_types.hpp"

namespace {
    TEST(type_string, ok_string_empty) {
        auto write_result = blopp::write(std::string{ "" });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::string>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.size(), size_t{ 0 });
    }

    TEST(type_string, ok_ui16string_empty) {
        auto write_result = blopp::write(std::u16string{ });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::u16string>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.size(), size_t{ 0 });
    }

    TEST(type_string, ok_string) {
        auto write_result = blopp::write(std::string{ "Hello world" });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::string>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_STREQ(output.c_str(), "Hello world");
    }

    TEST(type_string, ok_u16string) {
        auto input = std::u16string(size_t{ 200 }, char16_t{ 0xBEEF });

        auto write_result = blopp::write(std::u16string{ input });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::u16string>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_TRUE(output == input);
    }

    TEST(type_string, fail_string_offset_overflow) {
        auto input = std::string(size_t{ 255 }, 'A');
        auto write_result = blopp::write<blopp_test::minimal_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::string_offset_overflow);
    }

    TEST(type_string, fail_u16string_offset_overflow) {
        auto input = std::u16string(size_t{ 128 }, char16_t{ 'A' });
        auto write_result = blopp::write<blopp_test::minimal_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::string_offset_overflow);
    }

    TEST(type_string, fail_mismatching_string_char_size_1) {
        auto input = std::string(size_t{ 10 }, 'A');
        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::u16string>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::mismatching_string_char_size);
    }

    TEST(type_string, fail_mismatching_string_char_size_2) {
        auto input = std::u16string(size_t{ 10 }, char16_t{ 'A' });
        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::string>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::mismatching_string_char_size);
    }
}