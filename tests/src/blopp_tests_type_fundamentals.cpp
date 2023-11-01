#include "blopp_test.hpp"

namespace {
    TEST(type_fundamentals, ok_bool) {
        {
            auto write_result = blopp::write(false);
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<bool>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, false);
        }
        {
            auto write_result = blopp::write(true);
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<bool>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, true);
        }
    }

    TEST(type_fundamentals, ok_int8) {
        {
            auto write_result = blopp::write(int8_t{ 127 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<int8_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int8_t{ 127 });
        }
        {
            auto write_result = blopp::write(int8_t{ -100 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<int8_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int8_t{ -100 });
        }
    }

    TEST(type_fundamentals, ok_int16) {
        {
            auto write_result = blopp::write(int16_t{ 1278 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 3 });

            auto read_result = blopp::read<int16_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int16_t{ 1278 });
        }
        {
            auto write_result = blopp::write(int16_t{ -1008 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 3 });

            auto read_result = blopp::read<int16_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int16_t{ -1008 });
        }
    }

    TEST(type_fundamentals, ok_int32) {
        {
            auto write_result = blopp::write(int32_t{ 2345678 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 5 });

            auto read_result = blopp::read<int32_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int32_t{ 2345678 });
        }
        {
            auto write_result = blopp::write(int32_t{ -1234567 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 5 });

            auto read_result = blopp::read<int32_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int32_t{ -1234567 });
        }
    }

    TEST(type_fundamentals, ok_int64) {
        {
            auto write_result = blopp::write(int64_t{ 2345678901234LL });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 9 });

            auto read_result = blopp::read<int64_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int64_t{ 2345678901234LL });
        }
        {
            auto write_result = blopp::write(int64_t{ -1234567890123LL });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 9 });

            auto read_result = blopp::read<int64_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, int64_t{ -1234567890123LL });
        }
    }

    TEST(type_fundamentals, ok_uint8) {
        {
            auto write_result = blopp::write(uint8_t{ 0 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<uint8_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint8_t{ 0 });
        }
        {
            auto write_result = blopp::write(uint8_t{ 234 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<uint8_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint8_t{ 234 });
        }
    }

    TEST(type_fundamentals, ok_uint16) {
        {
            auto write_result = blopp::write(uint16_t{ 0 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 3 });

            auto read_result = blopp::read<uint16_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint16_t{ 0 });
        }
        {
            auto write_result = blopp::write(uint16_t{ 25678 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 3 });
            
            auto read_result = blopp::read<uint16_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint16_t{ 25678 });
        }
    }

    TEST(type_fundamentals, ok_uint32) {
        {
            auto write_result = blopp::write(uint32_t{ 0 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 5 });

            auto read_result = blopp::read<uint32_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint32_t{ 0 });
        }
        {
            auto write_result = blopp::write(uint32_t{ 25678901 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 5 });

            auto read_result = blopp::read<uint32_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint32_t{ 25678901 });
        }
    }

    TEST(type_fundamentals, ok_uint64) {
        {
            auto write_result = blopp::write(uint64_t{ 0ULL });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 9 });

            auto read_result = blopp::read<uint64_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint64_t{ 0ULL });
        }
        {
            auto write_result = blopp::write(uint64_t{ 256789012345ULL });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 9 });

            auto read_result = blopp::read<uint64_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, uint64_t{ 256789012345ULL });
        }
    }

    TEST(type_fundamentals, ok_char) {
        {
            auto write_result = blopp::write(char{ '\0' });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<char>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, char{ '\0' });
        }
        {
            auto write_result = blopp::write(char{ 'F' });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<char>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, char{ 'F' });
        }
    }

    TEST(type_fundamentals, ok_char8) {
        {
            auto write_result = blopp::write(char8_t{ '\0' });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<char8_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, char8_t{ '\0' });
        }
        {
            auto write_result = blopp::write(char8_t{ 'E' });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 2 });

            auto read_result = blopp::read<char8_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, char8_t{ 'E' });
        }
    }

    TEST(type_fundamentals, ok_float) {
        {
            auto write_result = blopp::write(float{ 0.0f });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 5 });

            auto read_result = blopp::read<float>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, float{ 0.0f });
        }
        {
            auto write_result = blopp::write(float{ 512.0f });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 5 });

            auto read_result = blopp::read<float>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, float{ 512.0f });
        }
    }

    TEST(type_fundamentals, ok_double) {
        {
            auto write_result = blopp::write(double{ 0.0 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 9 });

            auto read_result = blopp::read<double>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, double{ 0.0 });
        }
        {
            auto write_result = blopp::write(double{ 512.0 });
            ASSERT_TRUE(write_result);
            ASSERT_EQ(write_result->size(), size_t{ 9 });

            auto read_result = blopp::read<double>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, double{ 512.0 });
        }
    }

    TEST(type_fundamentals, ok_size_t) {
        {
            auto write_result = blopp::write(size_t{ 1234567 });
            ASSERT_TRUE(write_result);

            auto read_result = blopp::read<size_t>(*write_result);
            ASSERT_TRUE(read_result);
            EXPECT_EQ(read_result->value, size_t{ 1234567 });
        }
    }
}