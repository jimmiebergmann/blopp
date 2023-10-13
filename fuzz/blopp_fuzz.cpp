#include "blopp.hpp"

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

static volatile auto dummy_result = 0;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    auto input = std::span{ data, size };
    auto result = blopp::read<fuzz_object>(input);
    dummy_result = result.has_value() ? 1 : 0;
    return 0;
}
