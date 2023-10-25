#include "blopp_test.hpp"
#include "blopp_test_binary_format_types.hpp"

namespace {
    TEST(type_vector, ok_vector_bool) {
        const auto input = std::vector<bool>{ true, false, true, false, false, true };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::vector<bool>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        ASSERT_EQ(output.size(), size_t{ 6 });
        EXPECT_TRUE(output.at(0));
        EXPECT_FALSE(output.at(1));
        EXPECT_TRUE(output.at(2));
        EXPECT_FALSE(output.at(3));
        EXPECT_FALSE(output.at(4));
        EXPECT_TRUE(output.at(5));
    }

    TEST(type_vector, ok_vector_int32) {
        const auto input = std::vector<int32_t>{ 1337, -1337, 0, 123 };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::vector<int32_t>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        ASSERT_EQ(output.size(), size_t{ 4 });
        EXPECT_EQ(output.at(0), int32_t{ 1337 });
        EXPECT_EQ(output.at(1), int32_t{ -1337 });
        EXPECT_EQ(output.at(2), int32_t{ 0 });
        EXPECT_EQ(output.at(3), int32_t{ 123 });
    }

    TEST(type_vector, ok_vector_unique_ptr) {
        auto input = std::vector<std::unique_ptr<int32_t>>{};
        input.emplace_back(nullptr);
        input.emplace_back(std::make_unique<int32_t>(123));
        input.emplace_back(nullptr);

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);

        auto read_result = blopp::read<std::vector<std::unique_ptr<int32_t>>>(*write_result);
        ASSERT_TRUE(read_result);

        auto& output = read_result->value;
        ASSERT_EQ(output.size(), size_t{ 3 });

        EXPECT_EQ(output.at(0), nullptr);

        ASSERT_NE(output.at(1), nullptr);
        EXPECT_EQ((*output.at(1)), int32_t{ 123 });

        EXPECT_EQ(output.at(2), nullptr);
    }

    TEST(type_vector, fail_list_offset_overflow) {
        auto input = std::vector<std::string>{ 
            std::string(size_t{ 70 }, 'A'),
            std::string(size_t{ 100 }, 'B'),
            std::string(size_t{ 130 }, 'C'),
        };

        auto write_result = blopp::write<blopp_test::minimal_options>(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::list_offset_overflow);
    }
}