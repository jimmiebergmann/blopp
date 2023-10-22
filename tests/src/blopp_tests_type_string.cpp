#include "blopp_test.hpp"

namespace {
    TEST(type_string, ok_string) {
        auto write_result = blopp::write(std::string{ "Hello world" });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::string>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_STREQ(output.c_str(), "Hello world");
    }

    TEST(type_string, ok_string_empty) {
        auto write_result = blopp::write(std::string{ "" });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::string>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_STREQ(output.c_str(), "");
    }
}