#include "blopp_test.hpp"

enum class test_enum_1 : uint8_t {
    a = 0,
    b = 1,
    c = 2
};

enum class test_enum_2 : int32_t {
    a = 4,
    b = 1337
};

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
};


template<>
struct blopp::object<test_object_empty> {
    static void map(auto&, auto&) {
    }
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
            .map(value.object_empty_list);
    }
};


TEST(blopp, write_read_bool) {
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

TEST(blopp, write_read_int8) {
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

TEST(blopp, write_read_int16) {
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

TEST(blopp, write_read_int32) {
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

TEST(blopp, write_read_int64) {
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

TEST(blopp, write_read_uint8) {
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

TEST(blopp, write_read_uint16) {
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

TEST(blopp, write_read_uint32) {
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

TEST(blopp, write_read_uint64) {
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

TEST(blopp, write_read_char) {
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

TEST(blopp, write_read_char8) {
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

TEST(blopp, write_read_float) {
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

TEST(blopp, write_read_double) {
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

TEST(blopp, write_read_enum) {
    {
        auto input_bytes = blopp::write(test_enum_1::b);
        ASSERT_EQ(input_bytes.size(), size_t{ 2 });
        auto output_result = blopp::read<test_enum_1>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), test_enum_1::b);
    }
    {
        auto input_bytes = blopp::write(test_enum_2::a);
        ASSERT_EQ(input_bytes.size(), size_t{ 5 });
        auto output_result = blopp::read<test_enum_2>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ(output_result.value(), test_enum_2::a);
    }
}

TEST(blopp, write_read_string) {
    {
        auto input_bytes = blopp::write(std::string{ "" });
        auto output_result = blopp::read<std::string>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_STREQ(output_result.value().c_str(), "");
    }
    {
        auto input_bytes = blopp::write(std::string{ "Hello world" });
        auto output_result = blopp::read<std::string>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_STREQ(output_result.value().c_str(), "Hello world");
    }
}

TEST(blopp, write_read_unique_ptr) {
    {
        const auto input = std::unique_ptr<int32_t>{};
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::unique_ptr<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_FALSE(output_result.value());
    }
    {
        const auto input = std::make_unique<int32_t>(123);
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::unique_ptr<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);
        EXPECT_EQ((*output_result.value()), int32_t{ 123 });
    }  
}

TEST(blopp, write_read_test_object_empty) {
    const auto input = test_object_empty{};
    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<test_object_empty>(input_bytes);
    ASSERT_TRUE(output_result);
}

TEST(blopp, write_read_test_object_1) {
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
        .object_empty_list = { {}, {}, {} }
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
}

TEST(blopp, write_read_list_int32) {
    const auto input = std::list<int32_t>{ 1337, -1337, 0, 123 };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::list<int32_t>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);

    ASSERT_EQ(output.size(), size_t{ 4 });
    EXPECT_EQ((*std::next(output.begin(), 0)), int32_t{ 1337 });
    EXPECT_EQ((*std::next(output.begin(), 1)), int32_t{ -1337 });
    EXPECT_EQ((*std::next(output.begin(), 2)), int32_t{ 0 });
    EXPECT_EQ((*std::next(output.begin(), 3)), int32_t{ 123 });
}

TEST(blopp, write_read_vector_bool) {
    const auto input = std::vector<bool>{ true, false, true, false, false, true };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::vector<bool>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    ASSERT_EQ(output.size(), size_t{ 6 });
    EXPECT_TRUE(output.at(0));
    EXPECT_FALSE(output.at(1));
    EXPECT_TRUE(output.at(2));
    EXPECT_FALSE(output.at(3));
    EXPECT_FALSE(output.at(4));
    EXPECT_TRUE(output.at(5));
}

TEST(blopp, write_read_vector_int32) {
    const auto input = std::vector<int32_t>{ 1337, -1337, 0, 123 };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::vector<int32_t>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    ASSERT_EQ(output.size(), size_t{ 4 });
    EXPECT_EQ(output.at(0), int32_t{ 1337 });
    EXPECT_EQ(output.at(1), int32_t{ -1337 });
    EXPECT_EQ(output.at(2), int32_t{ 0 });
    EXPECT_EQ(output.at(3), int32_t{ 123 });
}

TEST(blopp, write_read_vector_unique_ptr) {
    auto input = std::vector<std::unique_ptr<int32_t>>{};
    input.emplace_back(nullptr);
    input.emplace_back(std::make_unique<int32_t>(123));
    input.emplace_back(nullptr);

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<std::vector<std::unique_ptr<int32_t>>>(input_bytes);
    ASSERT_TRUE(output_result);

    auto output = std::move(*output_result);
    ASSERT_EQ(output.size(), size_t{ 3 });
    EXPECT_EQ(output.at(0), nullptr);
    EXPECT_NE(output.at(1), nullptr);
    EXPECT_EQ((*output.at(1)), int32_t{ 123 });
    EXPECT_EQ(output.at(2), nullptr);
}
