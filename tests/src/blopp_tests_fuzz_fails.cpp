#include "blopp_test.hpp"

namespace {
    struct fuzz_object
    {
        std::string value_1;
        uint32_t value_2;
        double value_3;
        std::vector<int32_t> value_4;
    };
}

template<>
struct blopp::object<fuzz_object> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.value_1,
            value.value_2,
            value.value_3,
            value.value_4);
    }
};

namespace {
    TEST(fuzz_fails, buffer_overflow_1)
    {
        // FIXED:
        // read_context -> map -> map_impl -> read_list -> read_contiguous_container:
        // Buffer overflow due to has_bytes_left returning false when element_count * sizeof(element_t) overflows.

        const auto input = std::array<uint8_t, 52>{
            0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x0a, 0x0d, 0x0f, 0x0c, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x00, 0xd8, 0x0c, 0x0d, 0x0d,
                0x10, 0xf2, 0xf2, 0xf5, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 };

        auto input_span = std::span{ input.data(), input.size() };
        auto result = blopp::read<fuzz_object>(input_span);
        ASSERT_FALSE(result);
    }

    TEST(fuzz_fails, bad_boolean_value_1)
    {
        // FIXED:
        // Reading a byte with value != 0 or 1 as boolean is UB

        const auto input = std::array<uint8_t, 2>{ 0x01, 0xFF };

        auto input_span = std::span{ input.data(), input.size() };
        auto result = blopp::read<bool>(input_span);
        ASSERT_FALSE(result);
        EXPECT_EQ(result.error(), blopp::read_error_code::bad_boolean_value);
    }

    TEST(fuzz_fails, bad_boolean_value_2)
    {
        // FIXED:
        // Reading a byte with value != 0 or 1 as boolean is UB

        const auto input = std::array<uint8_t, 19>{ 
            0x0F, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF };

        auto input_span = std::span{ input.data(), input.size() };
        auto result1 = blopp::read<std::vector<bool>>(input_span);
        ASSERT_FALSE(result1);
        EXPECT_EQ(result1.error(), blopp::read_error_code::bad_boolean_value);

        auto result2 = blopp::read<std::list<bool>>(input_span);
        ASSERT_FALSE(result2);
        EXPECT_EQ(result2.error(), blopp::read_error_code::bad_boolean_value);

        auto result3 = blopp::read<std::array<bool, 1>>(input_span);
        ASSERT_FALSE(result3);
        EXPECT_EQ(result3.error(), blopp::read_error_code::bad_boolean_value);
    }
}