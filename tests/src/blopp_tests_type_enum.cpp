#include "blopp_test.hpp"

namespace {
    enum class test_enum_1 : uint8_t {
        a = 0,
        b = 1,
        c = 2
    };

    enum class test_enum_2 : int32_t {
        a = 4,
        b = 1337
    };


    TEST(type_enum, ok_enum_1) {
        auto write_result = blopp::write(test_enum_1::b);
        ASSERT_TRUE(write_result);
        ASSERT_EQ(write_result->size(), size_t{ 2 });

        auto read_result = blopp::read<test_enum_1>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output, test_enum_1::b);
    }

    TEST(type_enum, ok_enum_2) {
        auto write_result = blopp::write(test_enum_2::a);
        ASSERT_TRUE(write_result);
        ASSERT_EQ(write_result->size(), size_t{ 5 });

        auto read_result = blopp::read<test_enum_2>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output, test_enum_2::a);
    }
}