#include "blopp_test.hpp"

TEST(type_object_map, test) {

}

struct test_object_empty {
};

struct test_object_nested_1 {
    int32_t nested_value_1;
    double nested_value_2;
};

struct test_object_nested_2 {
    test_object_nested_1 object_nested_1_value;
};

struct test_object_1 {
    bool bool_value;
    int8_t int8_value;
    int16_t int16_value;
    int32_t int32_value;
    int64_t int64_value;
    uint8_t uint8_value;
    uint16_t uint16_value;
    uint32_t uint32_value;
    uint64_t uint64_value;
    char char_value;
    float float32_value;
    double float64_value;
    std::string string_value;
    test_object_empty object_empty_value;
    test_object_nested_2 object_nested_2_value;
    std::vector<test_object_empty> object_empty_vector;
    std::list<test_object_empty> object_empty_list;
    std::array<int32_t, 5> array_int32;
};


template<>
struct blopp::object<test_object_empty> {
    static void map(auto&, auto&) {}
};

template<>
struct blopp::object<test_object_nested_1> {
    static void map(auto& context, auto& value) {
        context
            .map(value.nested_value_1)
            .map(value.nested_value_2);
    }
};

template<>
struct blopp::object<test_object_nested_2> {
    static void map(auto& context, auto& value) {
        context
            .map(value.object_nested_1_value);
    }
};

template<>
struct blopp::object<test_object_1> {
    static void map(auto& context, auto& value) {
        context
            .map(value.bool_value)
            .map(value.int8_value)
            .map(value.int16_value)
            .map(value.int32_value)
            .map(value.int64_value)
            .map(value.uint8_value)
            .map(value.uint16_value)
            .map(value.uint32_value)
            .map(value.uint64_value)
            .map(value.char_value)
            .map(value.float32_value)
            .map(value.float64_value)
            .map(value.string_value)
            .map(value.object_empty_value)
            .map(value.object_nested_2_value)
            .map(value.object_empty_vector)
            .map(value.object_empty_list)
            .map(value.array_int32);
    }
};



TEST(type_object_map, write_read_test_object_empty) {
    const auto input = test_object_empty{};
    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<test_object_empty>(input_bytes);
    ASSERT_TRUE(output_result);
}

TEST(type_object_map, write_read_test_object_1) {
    const auto input = test_object_1{
        .bool_value = true,
        .int8_value = 123,
        .int16_value = 1'223,
        .int32_value = 1'234'567'890,
        .int64_value = 1'234'567'890'123LL,
        .uint8_value = 223,
        .uint16_value = 2'223,
        .uint32_value = 2'234'567'890,
        .uint64_value = 2'234'567'890'123ULL,
        .char_value = 'A',
        .float32_value = 1.25f,
        .float64_value = 2.5,
        .string_value = "The quick brown fox jumps over the lazy dog",
        .object_empty_value = {},
        .object_nested_2_value = test_object_nested_2{
            .object_nested_1_value = test_object_nested_1{
                .nested_value_1 = 1,
                .nested_value_2 = 2.0
            }
        },
        .object_empty_vector = { {}, {}, {} },
        .object_empty_list = { {}, {}, {} },
        .array_int32 = { 1, 2, 3, 4, 5 }
    };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<test_object_1>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);

    EXPECT_EQ(output.bool_value, input.bool_value);
    EXPECT_EQ(output.int8_value, input.int8_value);
    EXPECT_EQ(output.int16_value, input.int16_value);
    EXPECT_EQ(output.int32_value, input.int32_value);
    EXPECT_EQ(output.int64_value, input.int64_value);
    EXPECT_EQ(output.uint8_value, input.uint8_value);
    EXPECT_EQ(output.uint16_value, input.uint16_value);
    EXPECT_EQ(output.uint32_value, input.uint32_value);
    EXPECT_EQ(output.uint64_value, input.uint64_value);
    EXPECT_EQ(output.char_value, input.char_value);
    EXPECT_EQ(output.float32_value, input.float32_value);
    EXPECT_EQ(output.float64_value, input.float64_value);
    EXPECT_STREQ(output.string_value.c_str(), input.string_value.c_str());
    EXPECT_EQ(
        output.object_nested_2_value.object_nested_1_value.nested_value_1,
        input.object_nested_2_value.object_nested_1_value.nested_value_1);
    EXPECT_EQ(
        output.object_nested_2_value.object_nested_1_value.nested_value_2,
        input.object_nested_2_value.object_nested_1_value.nested_value_2);
    EXPECT_EQ(output.object_empty_vector.size(), input.object_empty_vector.size());
    EXPECT_EQ(output.object_empty_list.size(), input.object_empty_list.size());
    EXPECT_EQ(output.array_int32, input.array_int32);
}

TEST(type_object_map, write_read_test_object_1_compare_options) {
    const auto input = test_object_1{
        .bool_value = true,
        .int8_value = 123,
        .int16_value = 1'223,
        .int32_value = 1'234'567'890,
        .int64_value = 1'234'567'890'123LL,
        .uint8_value = 223,
        .uint16_value = 2'223,
        .uint32_value = 2'234'567'890,
        .uint64_value = 2'234'567'890'123ULL,
        .char_value = 'A',
        .float32_value = 1.25f,
        .float64_value = 2.5,
        .string_value = "The quick brown fox jumps over the lazy dog",
        .object_empty_value = {},
        .object_nested_2_value = test_object_nested_2{
            .object_nested_1_value = test_object_nested_1{
                .nested_value_1 = 1,
                .nested_value_2 = 2.0
            }
        },
        .object_empty_vector = { {}, {}, {} },
        .object_empty_list = { {}, {}, {} },
        .array_int32 = { 1, 2, 3, 4, 5 }
    };

    auto input_bytes_1 = blopp::write<blopp::default_write_options>(input);
    auto input_bytes_2 = blopp::write<blopp::compact_write_options>(input);
    ASSERT_TRUE(input_bytes_1.size() > input_bytes_2.size());

    auto output_result_1 = blopp::read<blopp::default_write_options, test_object_1>(input_bytes_1);
    ASSERT_TRUE(output_result_1);
    auto output_result_2 = blopp::read<blopp::compact_write_options, test_object_1>(input_bytes_2);
    ASSERT_TRUE(output_result_2);
}

TEST(type_object_map, write_read_test_object_1_erase_inputs) {
    const auto input = test_object_1{
        .bool_value = true,
        .int8_value = 123,
        .int16_value = 1'223,
        .int32_value = 1'234'567'890,
        .int64_value = 1'234'567'890'123LL,
        .uint8_value = 223,
        .uint16_value = 2'223,
        .uint32_value = 2'234'567'890,
        .uint64_value = 2'234'567'890'123ULL,
        .char_value = 'A',
        .float32_value = 1.25f,
        .float64_value = 2.5,
        .string_value = "The quick brown fox jumps over the lazy dog",
        .object_empty_value = {},
        .object_nested_2_value = test_object_nested_2{
            .object_nested_1_value = test_object_nested_1{
                .nested_value_1 = 1,
                .nested_value_2 = 2.0
            }
        },
        .object_empty_vector = { {}, {}, {} },
        .object_empty_list = { {}, {}, {} },
        .array_int32 = { 1, 2, 3, 4, 5 }
    };

    {
        auto input_bytes = blopp::write(input);
        const auto input_size = input_bytes.size();

        for (size_t i = 0; i < input_size; i++) {
            input_bytes.erase(input_bytes.begin());
            auto output_result = blopp::read<test_object_1>(input_bytes);
            ASSERT_FALSE(output_result);
        }
    }
    {
        auto input_bytes = blopp::write(input);
        const auto input_size = input_bytes.size();

        for (size_t i = 0; i < input_size; i++) {
            input_bytes.erase(input_bytes.cbegin());
            auto output_result = blopp::read<test_object_1>(input_bytes);
            ASSERT_FALSE(output_result);
        }
    }
}