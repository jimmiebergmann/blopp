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
        auto input_bytes = blopp::write(test_enum_1::b);
        ASSERT_EQ(input_bytes.size(), size_t{ 2 });
        auto output_result = blopp::read<test_enum_1>(input_bytes);
        ASSERT_TRUE(output_result);

        auto& output = output_result->value;
        EXPECT_EQ(output, test_enum_1::b);
    }

    TEST(type_enum, ok_enum_2) {
        auto input_bytes = blopp::write(test_enum_2::a);
        ASSERT_EQ(input_bytes.size(), size_t{ 5 });
        auto output_result = blopp::read<test_enum_2>(input_bytes);
        ASSERT_TRUE(output_result);

        auto& output = output_result->value;
        EXPECT_EQ(output, test_enum_2::a);
    }
}