#include "blopp_test.hpp"

namespace {
    struct test_object_c_array_map
    {
        int32_t c_array[3];
    };

    struct test_object_c_array_format
    {
        int32_t c_array[3];
    };
}


template<>
struct blopp::object<test_object_c_array_map> {
    static auto map(auto& context, auto& value) {
        context.map(value.c_array);
    }
};

template<>
struct blopp::object<test_object_c_array_format> {
    static auto format(auto& context, auto& value) {
        return context.format(value.c_array);
    }
};


namespace {
    TEST(type_c_array, ok_c_array_object_map) {
        const auto input = test_object_c_array_map{
            .c_array = { 1, 3, 8 }
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);
        auto read_result = blopp::read<test_object_c_array_map>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.c_array[0], int32_t{ 1 });
        EXPECT_EQ(output.c_array[1], int32_t{ 3 });
        EXPECT_EQ(output.c_array[2], int32_t{ 8 });
    }

    TEST(type_c_array, ok_c_array_object_format) {
        const auto input = test_object_c_array_format{
            .c_array = { 1, 3, 8 }
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);
        auto read_result = blopp::read<test_object_c_array_format>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.c_array[0], int32_t{ 1 });
        EXPECT_EQ(output.c_array[1], int32_t{ 3 });
        EXPECT_EQ(output.c_array[2], int32_t{ 8 });
    }
}