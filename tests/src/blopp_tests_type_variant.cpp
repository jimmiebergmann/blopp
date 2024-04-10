#include "blopp_test.hpp"

namespace {
    struct test_struct {
        int32_t value_1;
        bool value_2;
        float value_3;
    };
}

template<>
struct blopp::object<test_struct> {
    static void map(auto& context, auto& value) {
        context.map(
            value.value_1,
            value.value_2,
            value.value_3);
    }
};


namespace {
    using variant_test_type = std::variant<int32_t, float, test_struct>;

    TEST(type_variant, ok_variant_index_int) {
        auto input = variant_test_type{ int32_t{ 123 } };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<variant_test_type>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;

        ASSERT_EQ(output.index(), size_t{ 0 });
        EXPECT_EQ(std::get<0>(output), int32_t{ 123 });
    }

    TEST(type_variant, ok_variant_index_float) {
        auto input = variant_test_type{ 16.0f };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<variant_test_type>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;

        ASSERT_EQ(output.index(), size_t{ 1 });
        EXPECT_EQ(std::get<1>(output), 16.0f);
    }


    TEST(type_variant, ok_variant_index_struct) {
        auto input = variant_test_type{
            test_struct{
                .value_1 = 123,
                .value_2 = true,
                .value_3 = 4.0f
            }
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<variant_test_type>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;

        ASSERT_EQ(output.index(), size_t{ 2 });

        auto& output_struct = std::get<2>(output);

        EXPECT_EQ(output_struct.value_1, 123);
        EXPECT_EQ(output_struct.value_2, true);
        EXPECT_EQ(output_struct.value_3, 4.0f);
    }
}