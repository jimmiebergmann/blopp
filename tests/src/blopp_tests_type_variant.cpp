#include "blopp_test.hpp"

struct test_struct {
    int32_t value_1;
    bool value_2;
    float value_3;
};

template<>
struct blopp::object<test_struct> {
    static void map(auto& context, auto& value) {
        context
            .map(value.value_1)
            .map(value.value_2)
            .map(value.value_3);
    }
};

using variant_test_type = std::variant<int32_t, float, test_struct>;

TEST(type_variant, ok_variant_index_int) {
    

    auto input = variant_test_type{ int32_t{ 123 } };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<variant_test_type>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;

    ASSERT_EQ(output.index(), size_t{ 0 });
    EXPECT_EQ(std::get<0>(output), int32_t{ 123 });
}

TEST(type_variant, ok_variant_index_float) {
    using variant_type = std::variant<int32_t, float>;

    auto input = variant_test_type{ 16.0f };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<variant_test_type>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;

    ASSERT_EQ(output.index(), size_t{ 1 });
    EXPECT_EQ(std::get<1>(output), 16.0f);
}


TEST(type_variant, ok_variant_index_struct) {
    using variant_type = std::variant<int32_t, float>;

    auto input = variant_test_type{ 
        test_struct{
            .value_1 = 123,
            .value_2 = true,
            .value_3 = 4.0f
        }
    };

    auto input_bytes = blopp::write(input);
    auto output_result = blopp::read<variant_test_type>(input_bytes);
    ASSERT_TRUE(output_result);

    auto& output = output_result->value;

    ASSERT_EQ(output.index(), size_t{ 2 });

    auto& output_struct = std::get<2>(output);

    EXPECT_EQ(output_struct.value_1, 123);
    EXPECT_EQ(output_struct.value_2, true);
    EXPECT_EQ(output_struct.value_3, 4.0f);
}
