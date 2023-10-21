#include "blopp_test.hpp"

namespace {
    struct test_struct_fundamentals {
        int32_t value_1;
        bool value_2;
        float value_3;
    };

    struct test_struct_unique_ptrs {
        std::shared_ptr<test_struct_fundamentals> fundamentals_1;
        std::shared_ptr<test_struct_fundamentals> fundamentals_2;
        std::shared_ptr<test_struct_fundamentals> fundamentals_3;
        std::shared_ptr<test_struct_fundamentals> fundamentals_4;
        std::shared_ptr<test_struct_fundamentals> fundamentals_5;
    };
}

template<>
struct blopp::object<test_struct_fundamentals> {
    static void map(auto& context, auto& value) {
        context.map(
            value.value_1,
            value.value_2,
            value.value_3);
    }
};

template<>
struct blopp::object<test_struct_unique_ptrs> {
    static void map(auto& context, auto& value) {
        context.map(
            value.fundamentals_1,
            value.fundamentals_2,
            value.fundamentals_3,
            value.fundamentals_4,
            value.fundamentals_5);
    }
};

namespace {
    TEST(type_shared_ptr, ok_empty) {
        const auto input = std::shared_ptr<int32_t>{};
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::shared_ptr<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_FALSE(output_result->value);
    }

    TEST(type_shared_ptr, ok_int32) {
        const auto input = std::make_shared<int32_t>(123);
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::shared_ptr<int32_t>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_TRUE(output_result->value);
        EXPECT_EQ((*output_result->value), int32_t{ 123 });
    }

    TEST(type_shared_ptr, ok_struct_fundamentals) {
        const auto input = std::make_shared<test_struct_fundamentals>(123, false, 1.0f);
        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::shared_ptr<test_struct_fundamentals>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_TRUE(output_result->value);
        EXPECT_EQ(output_result->value->value_1, int32_t{ 123 });
        EXPECT_EQ(output_result->value->value_2, int32_t{ false });
        EXPECT_EQ(output_result->value->value_3, float{ 1.0f });
    }

    TEST(type_shared_ptr, ok_struct_unique_ptrs) {

        const auto input = std::make_shared<test_struct_unique_ptrs>();
        input->fundamentals_1 = std::make_shared<test_struct_fundamentals>(1, false, 1.0f);
        input->fundamentals_2 = nullptr;
        input->fundamentals_3 = std::make_shared<test_struct_fundamentals>(3, true, 3.0f);
        input->fundamentals_4 = input->fundamentals_3;
        input->fundamentals_5 = input->fundamentals_1;

        auto input_bytes = blopp::write(input);
        auto output_result = blopp::read<std::shared_ptr<test_struct_unique_ptrs>>(input_bytes);
        ASSERT_TRUE(output_result);

        ASSERT_TRUE(output_result->value);

        auto& unique_ptrs = *output_result->value;

        ASSERT_TRUE(unique_ptrs.fundamentals_1);
        EXPECT_NE(unique_ptrs.fundamentals_1, unique_ptrs.fundamentals_2);
        EXPECT_NE(unique_ptrs.fundamentals_1, unique_ptrs.fundamentals_3);
        EXPECT_NE(unique_ptrs.fundamentals_1, unique_ptrs.fundamentals_4);
        EXPECT_EQ(unique_ptrs.fundamentals_1, unique_ptrs.fundamentals_5);
        EXPECT_EQ(unique_ptrs.fundamentals_1->value_1, int32_t{ 1 });
        EXPECT_EQ(unique_ptrs.fundamentals_1->value_2, int32_t{ false });
        EXPECT_EQ(unique_ptrs.fundamentals_1->value_3, float{ 1.0f });

        ASSERT_FALSE(unique_ptrs.fundamentals_2);

        ASSERT_TRUE(unique_ptrs.fundamentals_3);
        EXPECT_EQ(unique_ptrs.fundamentals_3, unique_ptrs.fundamentals_4);
        EXPECT_NE(unique_ptrs.fundamentals_3, unique_ptrs.fundamentals_5);
        EXPECT_EQ(unique_ptrs.fundamentals_3->value_1, int32_t{ 3 });
        EXPECT_EQ(unique_ptrs.fundamentals_3->value_2, int32_t{ true });
        EXPECT_EQ(unique_ptrs.fundamentals_3->value_3, float{ 3.0f });
    }

    TEST(type_shared_ptr, ok_vector_unique_ptrs) {

        auto input = std::vector<std::shared_ptr<test_struct_fundamentals>>{};

        auto fundamentals_1 = std::make_shared<test_struct_fundamentals>(1, false, 1.0f);
        auto fundamentals_2 = std::make_shared<test_struct_fundamentals>(2, true, 2.0f);

        input.emplace_back(fundamentals_1);
        input.emplace_back();
        input.emplace_back(fundamentals_2);
        input.emplace_back(fundamentals_2);
        input.emplace_back(fundamentals_1);

        auto input_bytes = blopp::write(input);

        auto output_result = blopp::read<std::vector<std::shared_ptr<test_struct_fundamentals>>>(input_bytes);
        ASSERT_TRUE(output_result);

        auto& output = output_result->value;

        ASSERT_EQ(output.size(), size_t{ 5 });

        auto& output_1 = output.at(0);
        auto& output_2 = output.at(1);
        auto& output_3 = output.at(2);
        auto& output_4 = output.at(3);
        auto& output_5 = output.at(4);

        EXPECT_NE(output_1, output_2);
        EXPECT_NE(output_1, output_3);
        EXPECT_NE(output_1, output_4);
        EXPECT_EQ(output_1, output_5);
        ASSERT_TRUE(output_1);
        EXPECT_EQ(output_1->value_1, int32_t{ 1 });
        EXPECT_EQ(output_1->value_2, int32_t{ false });
        EXPECT_EQ(output_1->value_3, float{ 1.0f });

        EXPECT_FALSE(output_2);

        EXPECT_EQ(output_3, output_4);
        EXPECT_NE(output_3, output_5);
        ASSERT_TRUE(output_3);
        EXPECT_EQ(output_3->value_1, int32_t{ 2 });
        EXPECT_EQ(output_3->value_2, int32_t{ true });
        EXPECT_EQ(output_3->value_3, float{ 2.0f });
    }
}