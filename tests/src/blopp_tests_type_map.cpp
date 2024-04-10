#include "blopp_test.hpp"
#include "blopp_test_binary_format_types.hpp"

namespace {
    TEST(type_map, ok_int32_string_empty) {
        const auto input = std::map<int32_t, std::string>{ };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::map<int32_t, std::string>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        ASSERT_EQ(output.size(), size_t{ 0 });
    }

    TEST(type_map, ok_int32_string) {
        const auto input = std::map<int32_t, std::string>{
            { 1, "Hello world" },
            { 777, "Foo bar" },
            { 33, "This is not the last item." }
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::map<int32_t, std::string>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        ASSERT_EQ(output.size(), size_t{ 3 });

        auto it = output.begin();
        EXPECT_EQ(it->first, int32_t{ 1 });
        EXPECT_STREQ(it->second.c_str(), "Hello world");

        it = std::next(it);
        EXPECT_EQ(it->first, int32_t{ 33 });
        EXPECT_STREQ(it->second.c_str(), "This is not the last item.");

        it = std::next(it);
        EXPECT_EQ(it->first, int32_t{ 777 });
        EXPECT_STREQ(it->second.c_str(), "Foo bar");
    }

    TEST(type_map, ok_string_string) {
        const auto input = std::map<std::string, std::string>{
            { "key_1", "Hello world" },
            { "key_3", "Foo bar" },
            { "key_2", "This is not the last item." }
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::map<std::string, std::string>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        ASSERT_EQ(output.size(), size_t{ 3 });

        auto it = output.begin();
        EXPECT_STREQ(it->first.c_str(), "key_1");
        EXPECT_STREQ(it->second.c_str(), "Hello world");

        it = std::next(it);
        EXPECT_STREQ(it->first.c_str(), "key_2");
        EXPECT_STREQ(it->second.c_str(), "This is not the last item.");

        it = std::next(it);
        EXPECT_STREQ(it->first.c_str(), "key_3");
        EXPECT_STREQ(it->second.c_str(), "Foo bar");
    }

    TEST(type_map, fail_map_offset_overflow) {
        const auto input = std::map<std::string, std::string>{
            { "key_1", std::string(size_t{ 70 }, 'A') },
            { "key_3", std::string(size_t{ 100 }, 'B') },
            { "key_2", std::string(size_t{ 130 }, 'C') }
        };

        auto write_result = blopp::write<blopp_test::minimal_offset_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::map_offset_overflow);
    }

    TEST(type_map, fail_map_element_count_overflow) {
        auto input = std::map<int32_t, int32_t>{};
        for (int32_t i = 0; i < 256; i++) {
            input.emplace(std::make_pair(i, i));
        }

        auto write_result = blopp::write<blopp_test::minimal_count_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::map_element_count_overflow);
    }
}