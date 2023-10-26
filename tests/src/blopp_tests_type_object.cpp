#include "blopp_test.hpp"
#include "blopp_test_binary_format_types.hpp"

namespace {

    struct disallow_object_excess_bytes_options : blopp::default_binary_format_types {
        static constexpr auto allow_object_excess_bytes = false;
        static constexpr auto allow_more_object_members = true;
    };

    struct allow_more_members_options : blopp::default_binary_format_types {
        static constexpr auto allow_object_excess_bytes = true;
        static constexpr auto allow_more_object_members = true;
    };

    struct disallow_more_members_options : blopp::default_binary_format_types {
        static constexpr auto allow_object_excess_bytes = true;
        static constexpr auto allow_more_object_members = false;
    };


    struct test_object_empty {
    };

    struct test_object_nested_1 {
        int32_t nested_value_1 = {};
        double nested_value_2 = {};
    };

    struct test_object_nested_2 {
        test_object_nested_1 object_nested_1_value;
    };

    struct test_object_1 {
        bool bool_value = {};
        int8_t int8_value = {};
        int16_t int16_value = {};
        int32_t int32_value = {};
        int64_t int64_value = {};
        uint8_t uint8_value = {};
        uint16_t uint16_value = {};
        uint32_t uint32_value = {};
        uint64_t uint64_value = {};
        char char_value = {};
        float float32_value = {};
        double float64_value = {};
        std::string string_value = {};
        test_object_empty object_empty_value = {};
        test_object_nested_2 object_nested_2_value = {};
        std::vector<test_object_empty> object_empty_vector = {};
        std::list<test_object_empty> object_empty_list = {};
        std::array<int32_t, 5> array_int32 = {};
    };

    struct test_mapped_256_properties {
        int32_t value = 0;
    };

    struct test_missmatch_property_count_2_properties {
        int32_t value_1 = 0;
        int32_t value_2 = 0;
    };

    struct test_single_mapped_missmatch_property_count_3_properties {
        int32_t value_1 = 0;
        int32_t value_2 = 0;
        int32_t value_3 = 0;
    };

    struct test_multi_mapped_missmatch_property_count_3_properties {
        int32_t value_1 = 0;
        int32_t value_2 = 0;
        int32_t value_3 = 0;
    };

    struct test_member_count_2_members {
        int32_t value_1;
        int32_t value_2;
    };

    struct test_member_count_3_members {
        int32_t value_1;
        int32_t value_2;
        int32_t value_3;
    };

    struct test_user_defined_failure_ok {
        int32_t value_1;
    };

    struct test_user_defined_failure_fail {
        int32_t value_1;
    };
}

template<>
struct blopp::object<test_object_empty> {
    static auto map(auto&, auto&) {}
};

template<>
struct blopp::object<test_object_nested_1> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.nested_value_1,
            value.nested_value_2
        );
    }
};

template<>
struct blopp::object<test_object_nested_2> {
    static auto map(auto& context, auto& value) {
        context.map(value.object_nested_1_value);
    }
};

template<>
struct blopp::object<test_object_1> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.bool_value,
            value.int8_value,
            value.int16_value,
            value.int32_value,
            value.int64_value,
            value.uint8_value,
            value.uint16_value,
            value.uint32_value,
            value.uint64_value,
            value.char_value,
            value.float32_value,
            value.float64_value,
            value.string_value,
            value.object_empty_value,
            value.object_nested_2_value,
            value.object_empty_vector,
            value.object_empty_list,
            value.array_int32);
    }
};

template<>
struct blopp::object<test_mapped_256_properties> {
    static auto map(auto& context, auto& value) {
        for (size_t i = 0; i < 256; i++) {
            context.map(value.value);
        }
    }
};

template<>
struct blopp::object<test_missmatch_property_count_2_properties> {
    static auto map(auto& context, auto& value) {
        context.map(value.value_1, value.value_2);
    }
};

template<>
struct blopp::object<test_single_mapped_missmatch_property_count_3_properties> {
    static auto map(auto& context, auto& value) {
        context.map(value.value_1);
        context.map(value.value_2);
        context.map(value.value_3);
    }
};

template<>
struct blopp::object<test_multi_mapped_missmatch_property_count_3_properties> {
    static auto map(auto& context, auto& value) {
        for (size_t i = 0; i < 256; i++) {
            context.map(
                value.value_1,
                value.value_2,
                value.value_3);
        }
    }
};

template<>
struct blopp::object<test_member_count_2_members> {
    static void map(auto& context, auto& value) {
        context.map(
            value.value_1,
            value.value_2);
    }
};

template<>
struct blopp::object<test_member_count_3_members> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.value_1,
            value.value_2,
            value.value_3);
    }
};

template<>
struct blopp::object<test_user_defined_failure_ok> {
    static auto map(auto& context, auto& value) {
        context.map(value.value_1);
        return true;
    }
};

template<>
struct blopp::object<test_user_defined_failure_fail> {
    static auto map(auto& context, auto& value) {
        context.map(value.value_1);
        return false;
    }
};


namespace {
    TEST(type_object, ok_test_object_empty) {
        const auto input = test_object_empty{};

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_object_empty>(*write_result);
        ASSERT_TRUE(read_result);
    }

    TEST(type_object, ok_test_object_1) {
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

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_object_1>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;

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

    TEST(type_object, ok_test_object_1_compare_options) {
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

        auto write_result_1 = blopp::write<blopp::default_options>(input);
        ASSERT_TRUE(write_result_1);
        auto write_result_2 = blopp::write<blopp::compact_default_options>(input);
        ASSERT_TRUE(write_result_2);

        ASSERT_TRUE(write_result_1->size() > write_result_2->size());

        auto read_result_1 = blopp::read<blopp::default_options, test_object_1>(*write_result_1);
        ASSERT_TRUE(read_result_1);
        auto read_result_2 = blopp::read<blopp::compact_default_options, test_object_1>(*write_result_2);
        ASSERT_TRUE(read_result_2);
    }

    TEST(type_object, fail_object_offset_overflow) {
        const auto input = test_object_1{
           .string_value = std::string(size_t{ 240 }, 'A'),
        };
        auto write_result = blopp::write<blopp_test::minimal_offset_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::object_offset_overflow);
    }

    TEST(type_object, fail_write_object_property_count_overflow) {
        const auto input = test_mapped_256_properties{
           .value = 1
        };

        auto write_result = blopp::write<blopp_test::minimal_count_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::object_property_count_overflow);
    }

    TEST(type_object, fail_read_mismatching_object_property_count_single_mapped) {
        const auto input = test_missmatch_property_count_2_properties{
           .value_1 = 1,
           .value_2 = 2,
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_single_mapped_missmatch_property_count_3_properties>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::mismatching_object_property_count);
    }

    TEST(type_object, fail_read_mismatching_object_property_count_multi_mapped) {
        const auto input = test_missmatch_property_count_2_properties{
           .value_1 = 1,
           .value_2 = 2,
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_multi_mapped_missmatch_property_count_3_properties>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::mismatching_object_property_count);
    }

    TEST(type_object, fail_test_object_1_erase_inputs) {
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
            auto write_result = blopp::write(input);
            ASSERT_TRUE(write_result);

            auto& input_bytes = *write_result;
            const auto input_size = input_bytes.size();

            for (size_t i = 0; i < input_size; i++) {
                input_bytes.erase(input_bytes.begin());
                auto output_result = blopp::read<test_object_1>(input_bytes);
                ASSERT_FALSE(output_result);
            }
        }
        {
            auto write_result = blopp::write(input);
            ASSERT_TRUE(write_result);

            auto& input_bytes = *write_result;
            const auto input_size = input_bytes.size();

            for (size_t i = 0; i < input_size; i++) {
                input_bytes.erase(input_bytes.cbegin());
                auto output_result = blopp::read<test_object_1>(input_bytes);
                ASSERT_FALSE(output_result);
            }
        }
    }

    TEST(type_object, fail_read_user_defined_failure) {
        auto write_result = blopp::write(test_user_defined_failure_ok{ 100 });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_user_defined_failure_fail>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::user_defined_failure);
    }

    TEST(type_object, fail_write_user_defined_failure) {
        auto write_result = blopp::write(test_user_defined_failure_fail{ 200 });
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::user_defined_failure);
    }

    TEST(type_object, ok_read_user_defined_failure) {
        auto write_result = blopp::write(test_user_defined_failure_ok{ 300 });
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<test_user_defined_failure_ok>(*write_result);
        ASSERT_TRUE(read_result);
        EXPECT_EQ(read_result->value.value_1, int32_t{ 300 });
    }

    TEST(type_object, fail_object_excess_bytes) {
        auto write_result = blopp::write(test_object_nested_1{ });
        ASSERT_TRUE(write_result);

        auto modified_input = *write_result;

        ASSERT_EQ(modified_input.size(), 25);
        modified_input.at(1) += 1; // Increment object block size.
        modified_input.emplace_back(uint8_t{ 0 }); // Add excess byte.

        auto read_result = blopp::read<disallow_object_excess_bytes_options, test_object_nested_1>(modified_input);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::bad_object_excess_bytes);
    }

    TEST(type_object, ok_more_members) {
        auto object_members_3 = test_member_count_3_members{
            .value_1 = 1,
            .value_2 = 2,
            .value_3 = 3
        };

        auto write_result = blopp::write(object_members_3);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<allow_more_members_options, test_member_count_2_members>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = *read_result;
        EXPECT_EQ(output.remaining.size(), size_t{ 0 });
        EXPECT_EQ(output.value.value_1, int32_t{ 1 });
        EXPECT_EQ(output.value.value_2, int32_t{ 2 });
    }

    TEST(type_object, fail_more_members) {
        auto object_members_3 = test_member_count_3_members{
            .value_1 = 1,
            .value_2 = 2,
            .value_3 = 3
        };

        auto write_result = blopp::write(object_members_3);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<disallow_more_members_options, test_member_count_2_members>(*write_result);
        ASSERT_FALSE(read_result);
        EXPECT_EQ(read_result.error(), blopp::read_error_code::mismatching_object_property_count);
    }
}
