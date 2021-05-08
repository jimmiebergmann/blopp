/*
* MIT License
*
* Copyright (c) 2020 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef BLOPP_BLOPP_HPP
#define BLOPP_BLOPP_HPP

#include <cstring>
#include <type_traits>
#include <array>

namespace blopp
{

    /** Map wrapper struct, passed to map function of any context. */
    template<typename t_struct, typename t_value>
    struct map_wrapper
    {
        constexpr map_wrapper(
            t_value t_struct::* pointer,
            size_t offset
        ) :
            pointer(pointer),
            offset(offset),
            size(0)
        {}

        t_value t_struct::* pointer;
        size_t offset;
        size_t size;
    };

    template<typename t_struct>
    struct member_type
    {
        constexpr member_type() = default;

        constexpr member_type(
            size_t size,
            size_t offset,
            size_t raw_offset
        ) :
            size(size),
            offset(offset),
            raw_offset(raw_offset)
        {}

        size_t size;
        size_t offset;
        size_t raw_offset;

    };

    namespace impl
    {

        template<typename ... t_value, typename t_callback>
        constexpr void for_each_template_argument_value(t_callback&& callback, t_value ... value)
        {
            (callback(std::forward<t_value>(value)), ...);
        }

        /** Struct member validation. */
        template<typename t_value>
        static constexpr void validate_struct_member()
        {
            constexpr bool valid = std::is_arithmetic_v<t_value>;
            static_assert(valid, "Blopp: Data member is not of arithmetic type.");
        }


        /** Helper function for size of underlying member pointer data type. */
        template<typename t_struct, typename t_value>
        constexpr size_t get_member_pointer_type_size(t_value t_struct::* pointer)
        {
            return sizeof(t_value);
        }


        /** Context for struct size determination. */
        template<typename t_struct>
        struct struct_size_context
        {
            constexpr struct_size_context() = default;

            template<typename ... t_wrapper_value>
            constexpr size_t map(map_wrapper<t_struct, t_wrapper_value> ... args)
            {
                size_t total_size = 0;
                for_each_template_argument_value([&](const auto& wrapper)
                {
                    total_size += get_member_pointer_type_size(wrapper.pointer);
                }, args...);
                return total_size;
            }
        };

        template<typename t_struct>
        static constexpr size_t get_struct_size()
        {
            struct_size_context<t_struct> context = {};
            return t_struct::blopp_map(context);
        }


        /** Context for getting number of struct members. */
        template<typename t_struct>
        struct struct_member_count_context
        {
            constexpr struct_member_count_context() = default;

            template<typename ... t_wrapper_value>
            constexpr size_t map(t_wrapper_value ... args)
            {
                return sizeof...(args);
            }
        };

        template<typename t_struct>
        static constexpr size_t get_struct_member_count()
        {
            struct_member_count_context<t_struct> context = {};
            return t_struct::blopp_map(context);
        }


        /** Context for members size and offset. */
        template<typename t_struct>
        using members_array = std::array<member_type<t_struct>, get_struct_member_count<t_struct>()>;

        template<typename t_struct>
        struct struct_member_types_context
        {
            constexpr struct_member_types_context() = default;

            template<typename ... t_wrapper_value>
            constexpr members_array<t_struct> map(t_wrapper_value ... args)
            {
                members_array<t_struct> output = {};

                size_t index = 0;
                size_t raw_offset = 0;
                for_each_template_argument_value([&](const auto& wrapper)
                {
                    const size_t member_size = get_member_pointer_type_size(wrapper.pointer);
                    output[index] = member_type<t_struct>{ member_size, wrapper.offset, raw_offset };
                    ++index;
                    raw_offset += member_size;
                }, args...);

                return output;
            }
        };

        template<typename t_struct>
        static constexpr members_array<t_struct> get_member_types()
        {
            struct_member_types_context<t_struct> context = {};
            return t_struct::blopp_map(context);
        }

    }


    /** Struct size template. */
    template<typename t_struct>
    struct size
    {
        inline static constexpr size_t value = impl::get_struct_size<t_struct>();
    };

    template<typename t_struct>
    inline static constexpr size_t size_v = size<t_struct>::value;


    /** Members information template. */
    template<typename t_struct>
    struct members
    {
        inline static constexpr impl::members_array<t_struct> value = impl::get_member_types<t_struct>();
    };

    template<typename t_struct>
    inline static constexpr impl::members_array<t_struct> members_v = members<t_struct>::value;


    /** Deserialization result array type. */
    template<typename t_struct>
    using deserialize_array_result = std::array<uint8_t, size_v<t_struct>>;


    namespace impl
    {

        /** Context for serialization. */
        template<typename t_struct>
        struct serialize_context
        {
            constexpr serialize_context(const t_struct& input) :
                input(input)
            {}

            template<typename ... t_wrapper_value>
            constexpr deserialize_array_result<t_struct> map(t_wrapper_value ... args)
            {
                deserialize_array_result<t_struct> output = {};

                auto* output_ptr = reinterpret_cast<uint8_t*>(output.data());
                const auto* input_ptr = reinterpret_cast<const uint8_t*>(&input);

                const auto& struct_members = members_v<t_struct>;
                for (const auto& member : struct_members)
                {
                    auto* output_pos = output_ptr + member.raw_offset;
                    const auto* input_pos = input_ptr + member.offset;

                    std::memcpy(output_pos, input_pos, member.size);
                }

                return output;
            }

            const t_struct& input;
        };

        /** Context for deserialization. */
        template<typename t_struct>
        struct deserialize_context
        {
            constexpr deserialize_context(
                const void* data,
                const size_t size
            ) :
                m_data(data),
                m_size(size)
            {}

            template<typename ... t_wrapper_value>
            constexpr t_struct map(t_wrapper_value ... args)
            {
                t_struct output = {};

                const auto* input_ptr = static_cast<const uint8_t*>(m_data);
                auto* output_ptr = reinterpret_cast<uint8_t*>(&output);

                size_t data_offset = 0;
                const auto& struct_members = members_v<t_struct>;
                for (const auto& member : struct_members)
                {
                    if (data_offset + member.size > m_size)
                    {
                        break;
                    }

                    auto* output_pos = output_ptr + member.offset;
                    const auto* data_pos = input_ptr + data_offset;

                    std::memcpy(output_pos, data_pos, member.size);
                    data_offset += member.size;
                }

                return output;
            }

            const void* m_data;
            const size_t m_size;
        };


    }


    /** Serialization function. Turns data structure into bytes. */
    template<typename t_struct>
    deserialize_array_result<t_struct> serialize(const t_struct& input)
    {
        impl::serialize_context<t_struct> context = { input };
        return t_struct::blopp_map(context);
    }


    /** Deserialization function. Turns bytes into data structure. */
    template<typename t_struct>
    t_struct deserialize(const void* data, const size_t size)
    {
        impl::deserialize_context<t_struct> context = { data, size };
        return t_struct::blopp_map(context);
    }

    template<typename t_struct, typename t_array_type, size_t v_array_size>
    t_struct deserialize(const std::array<t_array_type, v_array_size>& data_array)
    {
        return deserialize<t_struct>(data_array.data(), data_array.size());
    }

}

#define blopp_map_member(struct_type, member_type) blopp::map_wrapper{ &struct_type::member_type, offsetof(struct_type, member_type) }

#endif