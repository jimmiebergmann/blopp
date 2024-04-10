#include "blopp_test.hpp"

namespace {
    struct my_test_object {
        uint64_t id = 0;
        std::string name = {};      
    };
}

template<>
struct blopp::object<my_test_object> {
    static auto map(auto& context, auto& value) {
        context.map(
            value.id,
            value.name
        );
    }
};

namespace {
    TEST(filesystem, fail_cannot_open_file) {
        const auto path = std::filesystem::path{ "this_path/does/not/exist/or/does/it" };

        auto write_result = blopp::write(int32_t{ 123 }, path);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::cannot_open_file);
    }

    TEST(filesystem, ok_open_file) {
        const auto path = std::filesystem::path{ "filesystem_blopp_test_file_1.bin" };

        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
        }

        auto write_result = blopp::write(int32_t{ 123 }, path);
        ASSERT_TRUE(write_result);
    }

    TEST(filesystem, ok_read_write) {
        const auto input = my_test_object{
            .id = 123,
            .name = "test"
        };

        const auto path = std::filesystem::path{ "filesystem_blopp_test_file_2.bin" };

        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
        }

        auto write_result = blopp::write(input, path);
        ASSERT_TRUE(write_result);
        
        auto read_result = blopp::read<my_test_object>(path);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        EXPECT_EQ(output.id, input.id);
        EXPECT_STREQ(output.name.c_str(), input.name.c_str());
    }

}