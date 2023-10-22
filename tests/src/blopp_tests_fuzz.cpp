#include "blopp_test.hpp"

// This test file is only to test if fuzz builds with other compilers than clang.
namespace {
    using fuzz_variant = std::variant<
        bool,
        char,
        int8_t,
        int16_t,
        int32_t,
        int64_t,
        uint8_t,
        uint16_t,
        uint32_t,
        uint64_t,
        float,
        double,
        std::string,
        std::vector<int32_t>,
        std::list<int32_t>,
        std::array<int32_t, 4>,
        std::map<int32_t, int32_t>
    >;

    enum class fuzz_enum {
        a, b, c
    };

    struct fuzz_object_nested;

    struct fuzz_object_nested {
        bool value_fund_1;
        std::shared_ptr<fuzz_object_nested> value_shared_ptr_1;
    };

    struct fuzz_object {
        bool value_fund_1;
        char value_fund_2;
        int8_t value_fund_3;
        int16_t value_fund_4;
        int32_t value_fund_5;
        int64_t value_fund_6;
        uint8_t value_fund_7;
        uint16_t value_fund_8;
        uint32_t value_fund_9;
        uint64_t value_fund_10;
        float value_fund_11;
        double value_fund_12;

        fuzz_enum value_enum_1;

        std::string value_string_1;

        std::vector<int32_t> value_vector_1;
        std::vector<fuzz_variant> value_vector_2;
        std::vector<fuzz_object_nested> value_vector_3;
        std::vector<std::shared_ptr<fuzz_object_nested>> value_vector_4;

        std::list<int32_t> value_list_1;

        std::array<int32_t, 3> value_array_1;

        std::map<int32_t, int32_t> value_map_1;
        std::map<std::string, fuzz_object_nested> value_map_2;

        std::unique_ptr<int32_t> value_unique_ptr_1;
        std::unique_ptr<int32_t> value_unique_ptr_2;

        std::shared_ptr<int32_t> value_shared_ptr_1;
        std::shared_ptr<int32_t> value_shared_ptr_2;
        std::shared_ptr<fuzz_object_nested> value_shared_ptr_3;

        std::optional<int32_t> value_optional_1;

        fuzz_variant value_variant_1;
    };
}


template<>
struct blopp::object<fuzz_object> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.value_fund_1,
            value.value_fund_2,
            value.value_fund_3,
            value.value_fund_4,
            value.value_fund_5,
            value.value_fund_6,
            value.value_fund_7,
            value.value_fund_8,
            value.value_fund_9,
            value.value_fund_10,
            value.value_fund_11,
            value.value_fund_12,

            value.value_enum_1,

            value.value_string_1,

            value.value_vector_1,
            value.value_vector_2,
            value.value_vector_3,
            value.value_vector_4,

            value.value_list_1,

            value.value_array_1,

            value.value_map_1,
            value.value_map_2,

            value.value_unique_ptr_1,
            value.value_unique_ptr_2,

            value.value_shared_ptr_1,
            value.value_shared_ptr_2,

            value.value_optional_1,

            value.value_variant_1
        );
    }
};

template<>
struct blopp::object<fuzz_object_nested> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.value_fund_1,
            value.value_shared_ptr_1
        );
    }
};


namespace {
    TEST(fuzz, fuzz)
    {
        const auto input = fuzz_object{ };
        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);
        EXPECT_GT(write_result->size(), size_t{ 0 });
    }
}