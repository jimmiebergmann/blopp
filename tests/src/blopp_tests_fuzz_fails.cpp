#include "blopp_test.hpp"

struct fuzz_object
{
    std::string value_1;
    uint32_t value_2;
    double value_3;
    std::vector<int32_t> value_4;
};

template<>
struct blopp::object<fuzz_object> {
    static auto map(auto& context, auto& value) {
        context
            .map(value.value_1)
            .map(value.value_2)
            .map(value.value_3)
            .map(value.value_4);
    }
};


TEST(fuzz_fails, fail_1)
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