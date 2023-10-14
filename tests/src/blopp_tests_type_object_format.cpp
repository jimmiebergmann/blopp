#include "blopp_test.hpp"

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


template<>
struct blopp::object<vec3_1> {
    static auto format(auto& context, const auto& value) {
        return
            context.format(value.x) &&
            context.format(value.y) &&
            context.format(value.z);
    }
};

template<>
struct blopp::object<vec3_2> {
    static auto map(auto& context, const auto& value) {
        context
            .map(value.x)
            .map(value.y)
            .map(value.z);
    }
};

template<>
struct blopp::object<array_enum_test_1> {
    static auto format(auto& context, auto& value) {
        return context.format(value.data_2) && context.format(value.data_4);
    }
};

template<>
struct blopp::object<array_enum_test_2> {
    static auto format(auto& context, auto& value) {
        return context.format(value.data_2) && context.format(value.data_3);
    }
};

template<>
struct blopp::object<array_uint8_test_1> {
    static auto format(auto& context, auto& value) {
        return context.format(value.low) && context.format(value.high);
    }
};

template<>
struct blopp::object<fundamental_test_1> {
    static auto format(auto& context, auto& value) {
        return 
            context.format(value.value_bool) && 
            context.format(value.value_int) && 
            context.format(value.value_enum);
    }
};


TEST(type_object_format, ok_fundamental_members) {

    const auto input = fundamental_test_1{
        .value_bool = true,
        .value_int = int32_t{ 123 },
        .value_enum = enum_test_2::b
    };
    auto input_bytes = blopp::write(input);
    EXPECT_EQ(input_bytes.size(), size_t{ 12 });

    auto output_result = blopp::read<fundamental_test_1>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    EXPECT_EQ(output.value_bool, input.value_bool);
    EXPECT_EQ(output.value_int, input.value_int);
    EXPECT_EQ(output.value_enum, input.value_enum);
}

TEST(type_object_format, ok_compare_map_and_format_size) {
    const auto input_1 = vec3_1{ 1.0f, 2.0f, 3.0f };
    const auto input_2 = vec3_2{ 1.0f, 2.0f, 3.0f };

    auto input_bytes_1 = blopp::write(input_1);
    EXPECT_EQ(input_bytes_1.size(), size_t{ 15 });

    auto input_bytes_2 = blopp::write(input_2);
    EXPECT_EQ(input_bytes_2.size(), size_t{ 26 });
}

TEST(type_object_format, ok_array_uint8_members) {

    const auto input = array_uint8_test_1{
        .low = { 1, 2, 3, 4, 5, 6, 7, 8 },
        .high = { 11, 12, 13, 14, 15, 16 }
    };
    auto input_bytes = blopp::write(input);
    EXPECT_EQ(input_bytes.size(), size_t{ 17 });

    auto output_result = blopp::read<array_uint8_test_1>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    EXPECT_EQ(output.low, input.low);
    EXPECT_EQ(output.high, input.high);
}


TEST(type_object_format, ok_array_enum_members_1) {
    const auto input = array_enum_test_1{
        .data_2 = { enum_test_1::a, enum_test_1::b },
        .data_4 = { enum_test_1::c, enum_test_1::a, enum_test_1::c, enum_test_1::b }
    };

    auto input_bytes = blopp::write(input);
    EXPECT_EQ(input_bytes.size(), size_t{ 9 });

    auto output_result = blopp::read<array_enum_test_1>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    EXPECT_EQ(output.data_2, input.data_2);
    EXPECT_EQ(output.data_4, input.data_4);
}


TEST(type_object_format, ok_array_enum_members_2) {
    const auto input = array_enum_test_2{
        .data_2 = { enum_test_2::a, enum_test_2::b },
        .data_3 = { enum_test_2::c, enum_test_2::a, enum_test_2::c }
    };

    auto input_bytes = blopp::write(input);
    EXPECT_EQ(input_bytes.size(), size_t{ 23 });

    auto output_result = blopp::read<array_enum_test_2>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;
    EXPECT_EQ(output.data_2, input.data_2);
    EXPECT_EQ(output.data_3, input.data_3);
}