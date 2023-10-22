#include "blopp_test.hpp"

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
}