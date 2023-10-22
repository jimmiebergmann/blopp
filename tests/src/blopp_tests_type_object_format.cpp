#include "blopp_test.hpp"

namespace {
    enum class enum_test_1 : uint8_t {
        a, b, c
    };

    enum class enum_test_2 : uint32_t {
        a, b, c
    };

    struct array_enum_test_1 {
        std::array<enum_test_1, 2> data_2;
        std::array<enum_test_1, 4> data_4;
    };

    struct array_enum_test_2 {
        std::array<enum_test_2, 2> data_2;
        std::array<enum_test_2, 3> data_3;
    };

    struct array_uint8_test_1 {
        std::array<uint8_t, 8> low;
        std::array<uint8_t, 6> high;
    };

    struct fundamental_test_1 {
        bool value_bool;
        int32_t value_int;
        enum_test_2 value_enum;
    };

    struct vec3_1 {
        float x, y, z;
    };

    struct vec3_2 {
        float x, y, z;
    };

    struct test_user_defined_failure_ok {
        int32_t value_1;
    };

    struct test_user_defined_failure_fail {
        int32_t value_1;
    };

    struct test_format_size_overflow {
        std::array<int32_t, 38> value_1;
        std::array<int32_t, 38> value_2;
    };
}


template<>
struct blopp::object<vec3_1> {
    static auto format(auto& context, const auto& value) {
        context.format(
            value.x,
            value.y,
            value.z);
    }
};

template<>
struct blopp::object<vec3_2> {
    static auto map(auto& context, const auto& value) {
        context.map(
            value.x,
            value.y,
            value.z);
    }
};

template<>
struct blopp::object<array_enum_test_1> {
    static auto format(auto& context, auto& value) {
        context.format(
            value.data_2,
            value.data_4);
    }
};

template<>
struct blopp::object<array_enum_test_2> {
    static auto format(auto& context, auto& value) {
        context.format(
            value.data_2,
            value.data_3);
    }
};

template<>
struct blopp::object<array_uint8_test_1> {
    static auto format(auto& context, auto& value) {
        context.format(
            value.low,
            value.high);
    }
};

template<>
struct blopp::object<fundamental_test_1> {
    static auto format(auto& context, auto& value) {
        context.format(
            value.value_bool,
            value.value_int,
            value.value_enum);
    }
};

template<>
struct blopp::object<test_user_defined_failure_ok> {
    static auto format(auto& context, auto& value) {
        context.format(value.value_1);
        return true;
    }
};

template<>
struct blopp::object<test_user_defined_failure_fail> {
    static auto format(auto& context, auto& value) {
        context.format(value.value_1);
        return false;
    }
};

template<>
struct blopp::object<test_format_size_overflow> {
    static auto format(auto& context, auto& value) {
        context.format(value.value_1);
        context.format(value.value_2);
    }
};


namespace {
    TEST(type_object_format, ok_fundamental_members) {

        const auto input = fundamental_test_1{
            .value_bool = true,
            .value_int = int32_t{ 123 },
            .value_enum = enum_test_2::b
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<fundamental_test_1>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.value_bool, input.value_bool);
        EXPECT_EQ(output.value_int, input.value_int);
        EXPECT_EQ(output.value_enum, input.value_enum);
    }

    TEST(type_object_format, ok_compare_map_and_format_size) {
        const auto input_1 = vec3_1{ 1.0f, 2.0f, 3.0f };
        const auto input_2 = vec3_2{ 1.0f, 2.0f, 3.0f };

        auto write_result_1 = blopp::write(input_1);
        ASSERT_TRUE(write_result_1);
        EXPECT_EQ(write_result_1->size(), size_t{ 15 });

        auto write_result_2 = blopp::write(input_2);
        ASSERT_TRUE(write_result_2);
        EXPECT_EQ(write_result_2->size(), size_t{ 26 });
    }

    TEST(type_object_format, ok_array_uint8_members) {

        const auto input = array_uint8_test_1{
            .low = { 1, 2, 3, 4, 5, 6, 7, 8 },
            .high = { 11, 12, 13, 14, 15, 16 }
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);
        EXPECT_EQ(write_result->size(), size_t{ 17 });     

        auto read_result = blopp::read<array_uint8_test_1>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.low, input.low);
        EXPECT_EQ(output.high, input.high);
    }


    TEST(type_object_format, ok_array_enum_members_1) {
        const auto input = array_enum_test_1{
            .data_2 = {
                enum_test_1::a,
                enum_test_1::b
            },
            .data_4 = {
                enum_test_1::c,
                enum_test_1::a,
                enum_test_1::c,
                enum_test_1::b
            }
        };

        auto write_result = blopp::write(input);
        EXPECT_EQ(write_result->size(), size_t{ 9 });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<array_enum_test_1>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.data_2, input.data_2);
        EXPECT_EQ(output.data_4, input.data_4);
    }


    TEST(type_object_format, ok_array_enum_members_2) {
        const auto input = array_enum_test_2{
            .data_2 = {
                enum_test_2::a,
                enum_test_2::b
            },
            .data_3 = {
                enum_test_2::c,
                enum_test_2::a,
                enum_test_2::c
            }
        };

        auto write_result = blopp::write(input);
        EXPECT_EQ(write_result->size(), size_t{ 23 });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<array_enum_test_2>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.data_2, input.data_2);
        EXPECT_EQ(output.data_3, input.data_3);
    }

    TEST(type_object_format, fail_read_user_defined_failure) {
        auto write_result = blopp::write(test_user_defined_failure_ok{ 100 });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_user_defined_failure_fail>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::user_defined_failure);
    }

    TEST(type_object_format, fail_write_user_defined_failure) {
        auto write_result = blopp::write(test_user_defined_failure_fail{ 200 });
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::user_defined_failure);
    }

    TEST(type_object_format, ok_read_user_defined_failure) {
        auto write_result = blopp::write(test_user_defined_failure_ok{ 300 });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_user_defined_failure_ok>(*write_result);
        ASSERT_TRUE(read_result);
        EXPECT_EQ(read_result->value.value_1, int32_t{ 300 });
    }

    TEST(type_object_format, fail_format_size_overflow) {
        auto write_result = blopp::write<blopp::compact_default_options>(test_format_size_overflow{ });
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::format_size_overflow);
    }

}