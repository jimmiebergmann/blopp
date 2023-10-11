#include "blopp_test.hpp"

TEST(type_fundamentals, write_read_bool) {
    {
        auto input_bytes = blopp::write(false);
        auto output_result = blopp::read<bool>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), false);
    }
    {
        auto input_bytes = blopp::write(true);
        auto output_result = blopp::read<bool>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), true);
    }
}

TEST(type_fundamentals, write_read_int8) {
    {
        auto input_bytes = blopp::write(int8_t{ 127 });
        auto output_result = blopp::read<int8_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int8_t{ 127 });
    }
    {
        auto input_bytes = blopp::write(int8_t{ -100 });
        auto output_result = blopp::read<int8_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int8_t{ -100 });
    }
}

TEST(type_fundamentals, write_read_int16) {
    {
        auto input_bytes = blopp::write(int16_t{ 1278 });
        auto output_result = blopp::read<int16_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int16_t{ 1278 });
    }
    {
        auto input_bytes = blopp::write(int16_t{ -1008 });
        auto output_result = blopp::read<int16_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int16_t{ -1008 });
    }
}

TEST(type_fundamentals, write_read_int32) {
    {
        auto input_bytes = blopp::write(int32_t{ 2345678 });
        auto output_result = blopp::read<int32_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int32_t{ 2345678 });
    }
    {
        auto input_bytes = blopp::write(int32_t{ -1234567 });
        auto output_result = blopp::read<int32_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int32_t{ -1234567 });
    }
}

TEST(type_fundamentals, write_read_int64) {
    {
        auto input_bytes = blopp::write(int64_t{ 2345678901234LL });
        auto output_result = blopp::read<int64_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int64_t{ 2345678901234LL });
    }
    {
        auto input_bytes = blopp::write(int64_t{ -1234567890123LL });
        auto output_result = blopp::read<int64_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), int64_t{ -1234567890123LL });
    }
}

TEST(type_fundamentals, write_read_uint8) {
    {
        auto input_bytes = blopp::write(uint8_t{ 0 });
        auto output_result = blopp::read<uint8_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint8_t{ 0 });
    }
    {
        auto input_bytes = blopp::write(uint8_t{ 234 });
        auto output_result = blopp::read<uint8_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint8_t{ 234 });
    }
}

TEST(type_fundamentals, write_read_uint16) {
    {
        auto input_bytes = blopp::write(uint16_t{ 0 });
        auto output_result = blopp::read<uint16_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint16_t{ 0 });
    }
    {
        auto input_bytes = blopp::write(uint16_t{ 25678 });
        auto output_result = blopp::read<uint16_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint16_t{ 25678 });
    }
}

TEST(type_fundamentals, write_read_uint32) {
    {
        auto input_bytes = blopp::write(uint32_t{ 0 });
        auto output_result = blopp::read<uint32_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint32_t{ 0 });
    }
    {
        auto input_bytes = blopp::write(uint32_t{ 25678901 });
        auto output_result = blopp::read<uint32_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint32_t{ 25678901 });
    }
}

TEST(type_fundamentals, write_read_uint64) {
    {
        auto input_bytes = blopp::write(uint64_t{ 0ULL });
        auto output_result = blopp::read<uint64_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint64_t{ 0ULL });
    }
    {
        auto input_bytes = blopp::write(uint64_t{ 256789012345ULL });
        auto output_result = blopp::read<uint64_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), uint64_t{ 256789012345ULL });
    }
}

TEST(type_fundamentals, write_read_char) {
    {
        auto input_bytes = blopp::write(char{ '\0' });
        auto output_result = blopp::read<char>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), char{ '\0' });
    }
    {
        auto input_bytes = blopp::write(char{ 'F' });
        auto output_result = blopp::read<char>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), char{ 'F' });
    }
}

TEST(type_fundamentals, write_read_char8) {
    {
        auto input_bytes = blopp::write(char8_t{ '\0' });
        auto output_result = blopp::read<char8_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), char8_t{ '\0' });
    }
    {
        auto input_bytes = blopp::write(char8_t{ 'E' });
        auto output_result = blopp::read<char8_t>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), char8_t{ 'E' });
    }
}

TEST(type_fundamentals, write_read_float) {
    {
        auto input_bytes = blopp::write(float{ 0.0f });
        auto output_result = blopp::read<float>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), float{ 0.0f });
    }
    {
        auto input_bytes = blopp::write(float{ 512.0f });
        auto output_result = blopp::read<float>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), float{ 512.0f });
    }
}

TEST(type_fundamentals, write_read_double) {
    {
        auto input_bytes = blopp::write(double{ 0.0 });
        auto output_result = blopp::read<double>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), double{ 0.0 });
    }
    {
        auto input_bytes = blopp::write(double{ 512.0 });
        auto output_result = blopp::read<double>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), double{ 512.0 });
    }
}