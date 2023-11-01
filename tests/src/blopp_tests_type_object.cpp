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
        bool b_0 = {}; bool b_1 = {}; bool b_2 = {}; bool b_3 = {}; bool b_4 = {}; bool b_5 = {}; bool b_6 = {}; bool b_7 = {}; bool b_8 = {}; bool b_9 = {};
        bool b_10 = {}; bool b_11 = {}; bool b_12 = {}; bool b_13 = {}; bool b_14 = {}; bool b_15 = {}; bool b_16 = {}; bool b_17 = {}; bool b_18 = {}; bool b_19 = {};
        bool b_20 = {}; bool b_21 = {}; bool b_22 = {}; bool b_23 = {}; bool b_24 = {}; bool b_25 = {}; bool b_26 = {}; bool b_27 = {}; bool b_28 = {}; bool b_29 = {};
        bool b_30 = {}; bool b_31 = {}; bool b_32 = {}; bool b_33 = {}; bool b_34 = {}; bool b_35 = {}; bool b_36 = {}; bool b_37 = {}; bool b_38 = {}; bool b_39 = {};
        bool b_40 = {}; bool b_41 = {}; bool b_42 = {}; bool b_43 = {}; bool b_44 = {}; bool b_45 = {}; bool b_46 = {}; bool b_47 = {}; bool b_48 = {}; bool b_49 = {};
        bool b_50 = {}; bool b_51 = {}; bool b_52 = {}; bool b_53 = {}; bool b_54 = {}; bool b_55 = {}; bool b_56 = {}; bool b_57 = {}; bool b_58 = {}; bool b_59 = {};
        bool b_60 = {}; bool b_61 = {}; bool b_62 = {}; bool b_63 = {}; bool b_64 = {}; bool b_65 = {}; bool b_66 = {}; bool b_67 = {}; bool b_68 = {}; bool b_69 = {};
        bool b_70 = {}; bool b_71 = {}; bool b_72 = {}; bool b_73 = {}; bool b_74 = {}; bool b_75 = {}; bool b_76 = {}; bool b_77 = {}; bool b_78 = {}; bool b_79 = {};
        bool b_80 = {}; bool b_81 = {}; bool b_82 = {}; bool b_83 = {}; bool b_84 = {}; bool b_85 = {}; bool b_86 = {}; bool b_87 = {}; bool b_88 = {}; bool b_89 = {};
        bool b_90 = {}; bool b_91 = {}; bool b_92 = {}; bool b_93 = {}; bool b_94 = {}; bool b_95 = {}; bool b_96 = {}; bool b_97 = {}; bool b_98 = {}; bool b_99 = {};
        bool b_100 = {}; bool b_101 = {}; bool b_102 = {}; bool b_103 = {}; bool b_104 = {}; bool b_105 = {}; bool b_106 = {}; bool b_107 = {}; bool b_108 = {}; bool b_109 = {};
        bool b_110 = {}; bool b_111 = {}; bool b_112 = {}; bool b_113 = {}; bool b_114 = {}; bool b_115 = {}; bool b_116 = {}; bool b_117 = {}; bool b_118 = {}; bool b_119 = {};
        bool b_120 = {}; bool b_121 = {}; bool b_122 = {}; bool b_123 = {}; bool b_124 = {}; bool b_125 = {}; bool b_126 = {}; bool b_127 = {}; bool b_128 = {}; bool b_129 = {};
        bool b_130 = {}; bool b_131 = {}; bool b_132 = {}; bool b_133 = {}; bool b_134 = {}; bool b_135 = {}; bool b_136 = {}; bool b_137 = {}; bool b_138 = {}; bool b_139 = {};
        bool b_140 = {}; bool b_141 = {}; bool b_142 = {}; bool b_143 = {}; bool b_144 = {}; bool b_145 = {}; bool b_146 = {}; bool b_147 = {}; bool b_148 = {}; bool b_149 = {};
        bool b_150 = {}; bool b_151 = {}; bool b_152 = {}; bool b_153 = {}; bool b_154 = {}; bool b_155 = {}; bool b_156 = {}; bool b_157 = {}; bool b_158 = {}; bool b_159 = {};
        bool b_160 = {}; bool b_161 = {}; bool b_162 = {}; bool b_163 = {}; bool b_164 = {}; bool b_165 = {}; bool b_166 = {}; bool b_167 = {}; bool b_168 = {}; bool b_169 = {};
        bool b_170 = {}; bool b_171 = {}; bool b_172 = {}; bool b_173 = {}; bool b_174 = {}; bool b_175 = {}; bool b_176 = {}; bool b_177 = {}; bool b_178 = {}; bool b_179 = {};
        bool b_180 = {}; bool b_181 = {}; bool b_182 = {}; bool b_183 = {}; bool b_184 = {}; bool b_185 = {}; bool b_186 = {}; bool b_187 = {}; bool b_188 = {}; bool b_189 = {};
        bool b_190 = {}; bool b_191 = {}; bool b_192 = {}; bool b_193 = {}; bool b_194 = {}; bool b_195 = {}; bool b_196 = {}; bool b_197 = {}; bool b_198 = {}; bool b_199 = {};
        bool b_200 = {}; bool b_201 = {}; bool b_202 = {}; bool b_203 = {}; bool b_204 = {}; bool b_205 = {}; bool b_206 = {}; bool b_207 = {}; bool b_208 = {}; bool b_209 = {};
        bool b_210 = {}; bool b_211 = {}; bool b_212 = {}; bool b_213 = {}; bool b_214 = {}; bool b_215 = {}; bool b_216 = {}; bool b_217 = {}; bool b_218 = {}; bool b_219 = {};
        bool b_220 = {}; bool b_221 = {}; bool b_222 = {}; bool b_223 = {}; bool b_224 = {}; bool b_225 = {}; bool b_226 = {}; bool b_227 = {}; bool b_228 = {}; bool b_229 = {};
        bool b_230 = {}; bool b_231 = {}; bool b_232 = {}; bool b_233 = {}; bool b_234 = {}; bool b_235 = {}; bool b_236 = {}; bool b_237 = {}; bool b_238 = {}; bool b_239 = {};
        bool b_240 = {}; bool b_241 = {}; bool b_242 = {}; bool b_243 = {}; bool b_244 = {}; bool b_245 = {}; bool b_246 = {}; bool b_247 = {}; bool b_248 = {}; bool b_249 = {};
        bool b_250 = {}; bool b_251 = {}; bool b_252 = {}; bool b_253 = {}; bool b_254 = {}; bool b_255 = {};
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

    struct test_map_as {
        uint32_t value_ui32_as_ui32 = {};
        uint32_t value_ui32_as_ui16 = {};
        uint32_t value_ui32_as_f32 = {};
        double value_f64_as_i32 = {};
        double value_f64_as_f32 = {};
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
        context.map(
            value.b_0, value.b_1, value.b_2, value.b_3, value.b_4, value.b_5, value.b_6, value.b_7, value.b_8, value.b_9,
            value.b_10, value.b_11, value.b_12, value.b_13, value.b_14, value.b_15, value.b_16, value.b_17, value.b_18, value.b_19,
            value.b_20, value.b_21, value.b_22, value.b_23, value.b_24, value.b_25, value.b_26, value.b_27, value.b_28, value.b_29,
            value.b_30, value.b_31, value.b_32, value.b_33, value.b_34, value.b_35, value.b_36, value.b_37, value.b_38, value.b_39,
            value.b_40, value.b_41, value.b_42, value.b_43, value.b_44, value.b_45, value.b_46, value.b_47, value.b_48, value.b_49,
            value.b_50, value.b_51, value.b_52, value.b_53, value.b_54, value.b_55, value.b_56, value.b_57, value.b_58, value.b_59,
            value.b_60, value.b_61, value.b_62, value.b_63, value.b_64, value.b_65, value.b_66, value.b_67, value.b_68, value.b_69,
            value.b_70, value.b_71, value.b_72, value.b_73, value.b_74, value.b_75, value.b_76, value.b_77, value.b_78, value.b_79,
            value.b_80, value.b_81, value.b_82, value.b_83, value.b_84, value.b_85, value.b_86, value.b_87, value.b_88, value.b_89,
            value.b_90, value.b_91, value.b_92, value.b_93, value.b_94, value.b_95, value.b_96, value.b_97, value.b_98, value.b_99,
            value.b_100, value.b_101, value.b_102, value.b_103, value.b_104, value.b_105, value.b_106, value.b_107, value.b_108, value.b_109,
            value.b_110, value.b_111, value.b_112, value.b_113, value.b_114, value.b_115, value.b_116, value.b_117, value.b_118, value.b_119,
            value.b_120, value.b_121, value.b_122, value.b_123, value.b_124, value.b_125, value.b_126, value.b_127, value.b_128, value.b_129,
            value.b_130, value.b_131, value.b_132, value.b_133, value.b_134, value.b_135, value.b_136, value.b_137, value.b_138, value.b_139,
            value.b_140, value.b_141, value.b_142, value.b_143, value.b_144, value.b_145, value.b_146, value.b_147, value.b_148, value.b_149,
            value.b_150, value.b_151, value.b_152, value.b_153, value.b_154, value.b_155, value.b_156, value.b_157, value.b_158, value.b_159,
            value.b_160, value.b_161, value.b_162, value.b_163, value.b_164, value.b_165, value.b_166, value.b_167, value.b_168, value.b_169,
            value.b_170, value.b_171, value.b_172, value.b_173, value.b_174, value.b_175, value.b_176, value.b_177, value.b_178, value.b_179,
            value.b_180, value.b_181, value.b_182, value.b_183, value.b_184, value.b_185, value.b_186, value.b_187, value.b_188, value.b_189,
            value.b_190, value.b_191, value.b_192, value.b_193, value.b_194, value.b_195, value.b_196, value.b_197, value.b_198, value.b_199,
            value.b_200, value.b_201, value.b_202, value.b_203, value.b_204, value.b_205, value.b_206, value.b_207, value.b_208, value.b_209,
            value.b_210, value.b_211, value.b_212, value.b_213, value.b_214, value.b_215, value.b_216, value.b_217, value.b_218, value.b_219,
            value.b_220, value.b_221, value.b_222, value.b_223, value.b_224, value.b_225, value.b_226, value.b_227, value.b_228, value.b_229,
            value.b_230, value.b_231, value.b_232, value.b_233, value.b_234, value.b_235, value.b_236, value.b_237, value.b_238, value.b_239,
            value.b_240, value.b_241, value.b_242, value.b_243, value.b_244, value.b_245, value.b_246, value.b_247, value.b_248, value.b_249,
            value.b_250, value.b_251, value.b_252, value.b_253, value.b_254, value.b_255
        );
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

template<>
struct blopp::object<test_map_as> {
    static auto map(auto& context, auto& value) {
        context.map_as<uint32_t>(value.value_ui32_as_ui32);
        context.map_as<uint16_t>(value.value_ui32_as_ui16);
        context.map_as<float>(value.value_ui32_as_f32);
        context.map_as<int32_t>(value.value_f64_as_i32);
        context.map_as<float>(value.value_f64_as_f32);
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
        const auto input = test_mapped_256_properties{};

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

    TEST(type_object, ok_map_as) {
        auto input = test_map_as{
            .value_ui32_as_ui32  = 123,
            .value_ui32_as_ui16 = 2000,
            .value_ui32_as_f32 = 64,
            .value_f64_as_i32 = 16.0,
            .value_f64_as_f32 = 8.0,
        };

        auto write_result = blopp::write(input);
        ASSERT_TRUE(write_result);
        EXPECT_EQ(write_result->size(), size_t{ 34 });

        auto read_result = blopp::read<test_map_as>(*write_result);
        ASSERT_TRUE(read_result);

        const auto& output = read_result->value;
        EXPECT_EQ(output.value_ui32_as_ui32, input.value_ui32_as_ui32);
        EXPECT_EQ(output.value_ui32_as_ui16, input.value_ui32_as_ui16);
        EXPECT_EQ(output.value_ui32_as_f32, input.value_ui32_as_f32);
        EXPECT_EQ(output.value_f64_as_i32, input.value_f64_as_i32);
        EXPECT_EQ(output.value_f64_as_f32, input.value_f64_as_f32);
    }

    TEST(type_object, fail_map_as_1) {
        auto input = test_map_as{
            .value_ui32_as_ui16 = uint32_t{ 70000 }
        };

        auto write_result = blopp::write(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::conversion_overflow);
    }

    TEST(type_object, fail_map_as_2) {
        auto input = test_map_as{
            .value_f64_as_i32 = 1234567891234.0
        };

        auto write_result = blopp::write(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::conversion_overflow);
    }

    TEST(type_object, fail_map_as_overflow) {
        auto input = test_map_as{
            .value_ui32_as_ui16 = uint32_t{ 123456 }
        };

        auto write_result = blopp::write(input);
        ASSERT_FALSE(write_result);
        EXPECT_EQ(write_result.error(), blopp::write_error_code::conversion_overflow);

    }    

}
