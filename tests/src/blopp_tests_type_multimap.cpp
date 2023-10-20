#include "blopp_test.hpp"

TEST(type_multimap, ok_int32_string_empty) {
    const auto input = std::multimap<int32_t, std::string>{ };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::multimap<int32_t, std::string>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    ASSERT_EQ(output.size(), size_t{ 0 });
}

TEST(type_multimap, ok_int32_string) {
    const auto input = std::multimap<int32_t, std::string>{
        { 1, "Hello world" },
        { 777, "Foo bar" },
        { 777, "Another Foo bar" },
        { 33, "This is not the last item." }
    };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::multimap<int32_t, std::string>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    ASSERT_EQ(output.size(), size_t{ 4 });

    auto it = output.begin();
    EXPECT_EQ(it->first, int32_t{ 1 });
    EXPECT_STREQ(it->second.c_str(), "Hello world");

    it = std::next(it);
    EXPECT_EQ(it->first, int32_t{ 33 });
    EXPECT_STREQ(it->second.c_str(), "This is not the last item.");

    it = std::next(it);
    EXPECT_EQ(it->first, int32_t{ 777 });
    EXPECT_STREQ(it->second.c_str(), "Foo bar");

    it = std::next(it);
    EXPECT_EQ(it->first, int32_t{ 777 });
    EXPECT_STREQ(it->second.c_str(), "Another Foo bar");
}

TEST(type_multimap, ok_string_string) {
    const auto input = std::multimap<std::string, std::string>{
        { "key_1", "Hello world" },
        { "key_3", "Foo bar" },
        { "key_2", "This is not the last item." },
        { "key_2", "Neither is this item." }
    };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::multimap<std::string, std::string>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    ASSERT_EQ(output.size(), size_t{ 4 });

    auto it = output.begin();
    EXPECT_STREQ(it->first.c_str(), "key_1");
    EXPECT_STREQ(it->second.c_str(), "Hello world");

    it = std::next(it);
    EXPECT_STREQ(it->first.c_str(), "key_2");
    EXPECT_STREQ(it->second.c_str(), "This is not the last item.");

    it = std::next(it);
    EXPECT_STREQ(it->first.c_str(), "key_2");
    EXPECT_STREQ(it->second.c_str(), "Neither is this item.");

    it = std::next(it);
    EXPECT_STREQ(it->first.c_str(), "key_3");
    EXPECT_STREQ(it->second.c_str(), "Foo bar");
}
