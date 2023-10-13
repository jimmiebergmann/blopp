/*
* MIT License
*
* Copyright (c) 2023 Jimmie Bergmann
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

#ifndef BLOPP_INCLUDE_BLOPP_HPP
#define BLOPP_INCLUDE_BLOPP_HPP

//#define BLOPP_USE_RESULT_WRAPPER

#if __has_include(<expected>) && !defined(BLOPP_USE_RESULT_WRAPPER)
#include <expected>
#endif

#include <type_traits>
#include <algorithm>
#include <iterator>
#include <limits>
#include <variant>
#include <optional>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <span>
#include <stdint.h>
#include <stddef.h>
#include <cstring>

#if !defined(__cpp_lib_expected) || defined(BLOPP_USE_RESULT_WRAPPER)
#define BLOPP_EXPECTED_IS_RESULT_WRAPPER
#endif

namespace blopp {

#if defined(BLOPP_EXPECTED_IS_RESULT_WRAPPER)
    template<typename TValue, typename TError>
    class result_wrapper {
    public:

        template<typename T = TValue>
        result_wrapper(const T& value) : m_variant(value) {}

        template<typename T = TValue>
        result_wrapper(T&& value) : m_variant(std::move(value)) {}

        TValue& value() { return std::get<0>(m_variant); }
        const TValue& value() const { return std::get<0>(m_variant); }

        TValue& operator * () { return std::get<0>(m_variant); }
        const TValue& operator * () const { return std::get<0>(m_variant); }
        TValue* operator -> () { return &std::get<0>(m_variant); }
        const TValue* operator -> () const { return &std::get<0>(m_variant); }

        TError& error() { return std::get<1>(m_variant); }
        const TError& error() const { return std::get<1>(m_variant); }

        bool has_value() const noexcept { return m_variant.index() == 0; }
        operator bool() const noexcept { return has_value(); }

    private:

        std::variant<TValue, TError> m_variant;
    };

    template<typename TValue, typename TError>
    using expected = result_wrapper<TValue, TError>;
#else
    template<typename TValue, typename TError>
    using expected = std::expected<TValue, TError>;
#endif

    struct default_binary_format_types {
        using string_size_type = uint64_t;
        using object_size_type = uint64_t;
        using object_property_count_type = uint16_t;
        using list_size_type = uint64_t;
        using list_element_count_type = uint64_t;
        using format_size_type = uint16_t;
    };

    struct compact_binary_format_types {
        using string_size_type = uint32_t;
        using object_size_type = uint32_t;
        using object_property_count_type = uint16_t;
        using list_size_type = uint32_t;
        using list_element_count_type = uint32_t;
        using format_size_type = uint8_t;
    };

    struct default_write_options {
        using binary_format_types = default_binary_format_types;
    };

    struct default_read_options {
        using binary_format_types = default_binary_format_types;
    };

    struct compact_read_options {
        using binary_format_types = compact_binary_format_types;
    };

    struct compact_write_options {
        using binary_format_types = compact_binary_format_types;
    };


    enum class write_error_code {
        string_size_overflow,
        object_size_overflow,
        object_property_count_overflow,
        list_size_overflow,
        list_element_count_overflow,
        format_failure,
        format_size_overflow
    };

    enum class read_error_code {
        insufficient_data,
        mismatching_type,
        mismatching_array_size,
        format_insufficient_data
    };


    using write_result_type = std::vector<uint8_t>;

    template<typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type;

    template<typename TOptions, typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type;


    using read_input_type = std::vector<uint8_t>;

    template<typename T>
    [[nodiscard]] auto read(const read_input_type& input) -> expected<T, read_error_code>;

    template<typename TOptions, typename T>
    [[nodiscard]] auto read(const read_input_type& input) -> expected<T, read_error_code>;


    using read_span_input_type = std::span<const uint8_t>;

    template<typename T>
    [[nodiscard]] auto read(read_span_input_type& input) -> expected<T, read_error_code>;

    template<typename TOptions, typename T>
    [[nodiscard]] auto read(read_span_input_type& input) -> expected<T, read_error_code>;
    

    template<typename T>
    struct object;

    enum class context_direction : uint8_t {
        read,
        write
    };
}


// Implementations.
namespace blopp::impl {

    template<typename T>
    constexpr bool always_false = false;

    enum class data_types : uint8_t {
        unspecified = 0,
        null = 1,
        boolean = 2,
        character = 3,
        int8 = 4,
        int16 = 5,
        int32 = 6,
        int64 = 7,
        uint8 = 8,
        uint16 = 9,
        uint32 = 10,
        uint64 = 11,
        float32 = 12,
        float64 = 13,
        string = 14,
        object = 15,
        list = 16
    };

    static constexpr uint8_t data_types_count = 16;

    constexpr bool validate_data_type(const data_types data_type)
    {
        return static_cast<uint8_t>(data_type) < data_types_count;
    }


    template<data_types Vdata_type>
    struct fundamental_traits_base {
        static constexpr auto is_fundamental = true;
        static constexpr auto data_type = Vdata_type;
    };

    template<typename T>
    struct fundamental_traits {
        static constexpr auto is_fundamental = false;
    };

    template<>
    struct fundamental_traits<bool> : fundamental_traits_base< data_types::boolean> {};
    template<>
    struct fundamental_traits<int8_t> : fundamental_traits_base< data_types::int8> {};
    template<>
    struct fundamental_traits<int16_t> : fundamental_traits_base< data_types::int16> {};
    template<>
    struct fundamental_traits<int32_t> : fundamental_traits_base< data_types::int32> {};
    template<>
    struct fundamental_traits<int64_t> : fundamental_traits_base< data_types::int64> {};
    template<>
    struct fundamental_traits<uint8_t> : fundamental_traits_base< data_types::uint8> {};
    template<>
    struct fundamental_traits<uint16_t> : fundamental_traits_base< data_types::uint16> {};
    template<>
    struct fundamental_traits<uint32_t> : fundamental_traits_base< data_types::uint32> {};
    template<>
    struct fundamental_traits<uint64_t> : fundamental_traits_base< data_types::uint64> {};
    template<>
    struct fundamental_traits<char> : fundamental_traits_base< data_types::character> {};
    template<>
    struct fundamental_traits<char8_t> : fundamental_traits_base< data_types::character> {};
    template<>
    struct fundamental_traits<float> : fundamental_traits_base< data_types::float32> {};
    template<>
    struct fundamental_traits<double> : fundamental_traits_base< data_types::float64> {};


    template<typename T, template<typename...> class TRef>
    struct is_specialization : std::false_type {};

    template<template<typename...> class TRef, typename... TArgs>
    struct is_specialization<TRef<TArgs...>, TRef> : std::true_type {};

    template<typename T, template<typename...> class TRef>
    static constexpr bool is_specialization_v = is_specialization<T, TRef>::value;

    template<typename T>
    struct is_std_array : std::false_type {};

    template<typename T, size_t N>
    struct is_std_array<std::array<T, N>> : std::true_type {};

    template<typename T>
    static constexpr bool is_std_array_v = is_std_array<T>::value;

    template<typename T>
    static constexpr bool is_std_vector_v = is_specialization_v<T, std::vector>;

    struct dummy_read_write_context {
        auto& map(auto&) { return *this; }
    };

    template<typename T>
    constexpr auto object_is_mapped() -> bool {
        return
            std::is_class_v<T> == true &&
            requires(T & value, dummy_read_write_context dummy) { { blopp::object<T>::map(dummy, value) }; };
    }

    struct dummy_format_read_write_context {
        auto format(auto&) { return true; }
    };

    template<typename T>
    constexpr auto object_is_formatted() -> bool {
        return
            std::is_class_v<T> == true &&
            requires(T & value, dummy_format_read_write_context dummy) { { blopp::object<T>::format(dummy, value) }; };
    }


    template<typename T>
    constexpr data_types get_data_type() {
        using value_fundamental_traits = fundamental_traits<T>;
        if constexpr (value_fundamental_traits::is_fundamental == true) {
            return value_fundamental_traits::data_type;
        }
        else if constexpr (std::is_same_v<T, std::string> == true) {
            return data_types::string;
        }
        else if constexpr (is_specialization_v<T, std::unique_ptr> == true) {
            using element_t = typename T::element_type;
            return get_data_type<element_t>();
        }
        else if constexpr (
            is_specialization_v<T, std::vector> == true ||
            is_specialization_v<T, std::list> == true)
        {
            return data_types::list;
        }
        else if constexpr (std::is_class_v<T> == true && object_is_mapped<T>() == true) {
            return data_types::object;
        }
        else {
            static_assert(always_false<T>, "Unmapped blopp data type.");
        }
    }


    enum class element_flags : uint8_t {
        data_type_per_element = 1
    };

    constexpr data_types clean_element_flags(const data_types data_type) {
       return static_cast<data_types>(static_cast<uint8_t>(data_type) & 0b00011111);
    }

    constexpr element_flags get_element_flags(const data_types data_type) {
        return static_cast<element_flags>((static_cast<uint8_t>(data_type) & 0b11100000) >> 5);
    }

    constexpr void set_element_flags(data_types& data_type, const element_flags flags) {
        data_type = static_cast<data_types>(static_cast<uint8_t>(data_type) | static_cast<uint8_t>(flags) << 5);
    }


    template<typename TValue>
    inline size_t write_value(write_result_type& output, const TValue& value) {
        const auto* value_ptr = reinterpret_cast<const uint8_t*>(&value);
        std::copy(value_ptr, value_ptr + sizeof(value), std::back_inserter(output));
        return sizeof(value);
    }

    template<typename TContainer>
    inline size_t write_contiguous_container(write_result_type& output, const TContainer& container) {
        using element_t = typename TContainer::value_type;
        const auto container_ptr = reinterpret_cast<const uint8_t*>(container.data());
        const auto container_byte_count = sizeof(element_t) * container.size();
        std::copy(container_ptr, container_ptr + container_byte_count, std::back_inserter(output));
        return container_byte_count;
    }

    template<typename T>
    inline T read_value(read_span_input_type& input) {
        auto value = T{};
        std::memcpy(&value, input.data(), sizeof(T));
        input = input.subspan(sizeof(T));
        return value;
    }

    template<typename T>
    inline void read_value(read_span_input_type& input, T& value) {
        std::memcpy(&value, input.data(), sizeof(T));
        input = input.subspan(sizeof(T));
    }

    template<typename TContainer>
    inline void read_contiguous_container(read_span_input_type& input, TContainer& container, const size_t count) {
        using container_t = std::remove_cvref_t<decltype(container)>;
        using element_t = typename container_t::value_type;

        const auto* src_element_begin_ptr = reinterpret_cast<const element_t*>(input.data());
        
        if constexpr (is_std_array_v<container_t> == true) {
            auto* dest_element_ptr = reinterpret_cast<element_t*>(container.data());
            std::memcpy(dest_element_ptr, src_element_begin_ptr, count * sizeof(element_t));
        }
        else {
            const auto* src_element_end_ptr = src_element_begin_ptr + count;
            std::copy(src_element_begin_ptr, src_element_end_ptr, std::back_inserter(container));
        }        
        
        input = input.subspan(count * sizeof(element_t));
    }

    template<typename TContainer>
    inline void clear_container([[maybe_unused]] TContainer& container) {
        using container_t = std::remove_cvref_t<decltype(container)>;
        if constexpr (is_std_array_v<container_t> == false) {
            container.clear();
        }
    }

    template<typename TContainer>
    inline auto& emplace_container(TContainer& container, [[maybe_unused]] const size_t index) {
        using container_t = std::remove_cvref_t<decltype(container)>;
        if constexpr (is_std_array_v<container_t> == true) {
            return container.at(index);
        }
        else {
            return container.emplace_back();
        }
    }


    template<typename T>
    class post_output_writer
    {

    public:

        post_output_writer(write_result_type& output) :
            m_output{ output },
            m_position{ m_output.size() }
        {
            m_output.insert(m_output.end(), sizeof(T), uint8_t{});
        }

        void update(const T value) {
            std::memcpy(m_output.data() + m_position, &value, sizeof(T));
        }

    private:

        write_result_type& m_output;
        size_t m_position;

    };


    template<typename TOptions>
    class write_format_context {

    public:

        static constexpr auto direction = context_direction::write;

        explicit write_format_context(
            write_result_type& output
        ) :
            m_output{ output }
        {}

        write_format_context(const write_format_context&) = delete;
        write_format_context(write_format_context&&) = delete;
        write_format_context& operator = (const write_format_context&) = delete;
        write_format_context& operator = (write_format_context&&) = delete;

        [[nodiscard]] bool format(auto& value) {
            return format_impl(value);
        }

    private:

        using options = TOptions;
        using options_format_size_type = typename options::binary_format_types::format_size_type;
        
        static constexpr auto max_format_size = static_cast<size_t>(std::numeric_limits<options_format_size_type>::max());

        [[nodiscard]] inline bool format_impl(auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (
                value_fundamental_traits::is_fundamental == true ||
                std::is_enum_v<value_t> == true)
            {
                write_value(m_output, value);
            }
            else if constexpr (
                is_std_array_v<value_t> == true)
            {
                using element_t = typename value_t::value_type;
                using element_fundamental_traits = fundamental_traits<element_t>;

                if constexpr (
                    element_fundamental_traits::is_fundamental == true ||
                    std::is_enum_v<element_t> == true) 
                {
                    write_contiguous_container(m_output, value);
                }
                else {
                    static_assert(always_false<value_t>, "Unmapped blopp format container element data type.");
                }
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp format data type.");
            }

            return m_format_size <= max_format_size;
        };

        write_result_type& m_output;
        size_t m_format_size = 0;

    };


    template<typename TOptions>
    class write_context {

    public:

        static constexpr auto direction = context_direction::write;

        explicit write_context(
            write_result_type& output
        ) :
            m_output{ output }
        {}

        write_context(const write_context&) = delete;
        write_context(write_context&&) = delete;
        write_context& operator = (const write_context&) = delete;
        write_context& operator = (write_context&&) = delete;

        auto& map(auto& value) {
            ++m_property_count;
            return map_impl<false>(value);
        }

    private:

        using options = TOptions;
        using options_string_size_type = typename options::binary_format_types::string_size_type;
        using options_object_size_type = typename options::binary_format_types::object_size_type;
        using options_object_property_count_type = typename options::binary_format_types::object_property_count_type;
        using options_list_size_type = typename options::binary_format_types::list_size_type;
        using options_list_element_count_type = typename options::binary_format_types::list_element_count_type;
        using options_format_size_type = typename options::binary_format_types::format_size_type;

        inline void write_null() {
            m_byte_count += write_value(m_output, static_cast<uint8_t>(data_types::null));
        }

        template<bool VSkipDataType, typename T>
        inline void write_fundamental_value(const T& value) {
            using value_t = std::remove_cvref_t<T>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (VSkipDataType == false) {
                m_byte_count += write_value(m_output, static_cast<uint8_t>(value_fundamental_traits::data_type));
            }

            m_byte_count += write_value(m_output, value);
        }

        template<bool VSkipDataType>
        inline void write_fundamental(const auto& value) {
            write_fundamental_value<VSkipDataType>(value);
        }

        template<bool VSkipDataType>
        inline void write_enum(const auto& value) {
            using value_t = std::underlying_type_t<std::remove_cvref_t<decltype(value)>>;
            write_fundamental_value<VSkipDataType, value_t>(static_cast<value_t>(value));
        }

        template<bool VSkipDataType>
        inline void write_string(const auto& value) {
            if constexpr (VSkipDataType == false) {
                m_byte_count += write_value(m_output, static_cast<uint8_t>(data_types::string));
            }

            m_byte_count += write_value(m_output, static_cast<options_string_size_type>(value.size()));
            m_byte_count += write_contiguous_container(m_output, value);
        }
     
        template<bool VSkipDataType>
        inline void write_object(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if constexpr (VSkipDataType == false) {
                m_byte_count += write_value(m_output, static_cast<uint8_t>(data_types::object));
            }

            const auto block_start_position = m_byte_count;         

            auto block_size_writer = post_output_writer<options_object_size_type>{ m_output };
            auto property_count_writer = post_output_writer<options_object_property_count_type>{ m_output };
            m_byte_count += sizeof(options_object_size_type) + sizeof(options_object_property_count_type);

            auto object_write_context = write_context{ m_output };
            object<value_t>::map(object_write_context, value);
            m_byte_count += object_write_context.m_byte_count;

            const auto block_size = static_cast<options_object_size_type>(m_byte_count - block_start_position);
            block_size_writer.update(block_size);
            property_count_writer.update(object_write_context.m_property_count);
        }

        template<bool VSkipDataType>
        inline void write_list(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            constexpr auto element_is_unique_ptr = is_specialization_v<element_t, std::unique_ptr>;

            if constexpr (VSkipDataType == false) {
                m_byte_count += write_value(m_output, static_cast<uint8_t>(data_types::list));
            }

            const auto block_start_position = m_byte_count;

            auto block_size_writer = post_output_writer<options_list_size_type>{ m_output };
            m_byte_count += sizeof(options_list_size_type);

            auto element_data_type = get_data_type<element_t>();
            if constexpr (element_is_unique_ptr == true) {
                set_element_flags(element_data_type, element_flags::data_type_per_element);
            }

            m_byte_count += write_value(m_output, element_data_type);
            m_byte_count += write_value(m_output, static_cast<options_list_element_count_type>(value.size()));

            if constexpr (element_fundamental_traits::is_fundamental == true) {
                if constexpr (
                    std::contiguous_iterator<typename value_t::iterator> &&
                    (is_std_vector_v<value_t> == false || std::is_same_v<element_t, bool> == false))
                {
                    m_byte_count += write_contiguous_container(m_output, value);
                }
                else {
                    for (const auto element_value : value) {
                        m_byte_count += write_value(m_output, static_cast<const element_t>(element_value));
                    }
                }
            }
            else {
                for (const auto& element_value : value) {
                    map_impl<element_is_unique_ptr == false>(element_value);
                }
            }

            const auto block_size = static_cast<options_list_size_type>(m_byte_count - block_start_position);
            block_size_writer.update(block_size);
        }

        template<bool VSkipDataType>
        inline void write_formatted(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if constexpr (VSkipDataType == false) {
                m_byte_count += write_value(m_output, static_cast<uint8_t>(data_types::unspecified));
            }

            auto block_size_writer = post_output_writer<options_format_size_type>{ m_output };
            m_byte_count += sizeof(options_format_size_type);
            
            const auto block_start_position = m_output.size();

            auto format_write_context = write_format_context<TOptions>{ m_output };
            if (!object<value_t>::format(format_write_context, value)) {
                // return std::unexpected(write_error_code::format_failure); // TODO
            }
            const auto format_size = m_output.size() - block_start_position;
            /*if (format_size > static_cast<size_t>(std::numeric_limits< options_format_size_type>::max()))
            {
                // return std::unexpected(write_error_code::format_size_overflow); // TODO
            }*/

            m_byte_count += format_size;

            const auto block_size = static_cast<options_format_size_type>(format_size);
            block_size_writer.update(block_size);
        }

        template<bool VSkipDataType>
        inline auto& map_impl(auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (value_fundamental_traits::is_fundamental == true) {
                write_fundamental<VSkipDataType>(value);
            }
            else if constexpr (std::is_enum_v<value_t> == true) {
                write_enum<VSkipDataType>(value);
            }
            else if constexpr (std::is_same_v<value_t, std::string> == true) {
                write_string<VSkipDataType>(value);
            }
            else if constexpr (is_specialization_v<value_t, std::unique_ptr> == true) {
                if (value == nullptr) {
                    write_null();
                }
                else {
                    map_impl<VSkipDataType>(*value);
                }
            }
            else if constexpr (
                is_std_array_v<value_t> == true ||
                is_std_vector_v<value_t> == true ||
                is_specialization_v<value_t, std::list> == true)
            {
                write_list<VSkipDataType>(value);
            }
            else if constexpr (object_is_mapped<value_t>() == true) {
                write_object<VSkipDataType>(value);
            }
            else if constexpr (object_is_formatted<value_t>() == true) {
                write_formatted<VSkipDataType>(value);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp data type.");
            }

            return *this;
        }

        write_result_type& m_output;
        size_t m_byte_count = 0;
        options_object_property_count_type m_property_count = 0;

    };


    template<typename TOptions>
    class read_format_context {

    public:

        static constexpr auto direction = context_direction::write;

        explicit read_format_context(
            read_span_input_type& input,
            std::optional<read_error_code>& error
        ) :
            m_input{ input },
            m_error{ error }
        {}

        read_format_context(const read_format_context&) = delete;
        read_format_context(read_format_context&&) = delete;
        read_format_context& operator = (const read_format_context&) = delete;
        read_format_context& operator = (read_format_context&&) = delete;

        [[nodiscard]] bool format(auto& value) {
            return format_impl(value);
        }

    private:

        using options = TOptions;
        using options_format_size_type = typename options::binary_format_types::format_size_type;

        static constexpr auto max_format_size = static_cast<size_t>(std::numeric_limits<options_format_size_type>::max());

        inline bool has_bytes_left(const size_t count)
        {
            return m_input.size() >= count;
        }

        [[nodiscard]] bool format_impl(auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (
                value_fundamental_traits::is_fundamental == true ||
                std::is_enum_v<value_t> == true)
            {
                if (!has_bytes_left(sizeof(value_t)))
                {
                    m_error = read_error_code::format_insufficient_data;
                    return false;
                }

                read_value(m_input, value);

                return true;
            }
            else if constexpr (is_std_array_v<value_t> == true) {
                using element_t = typename value_t::value_type;
                using element_fundamental_traits = fundamental_traits<element_t>;

                if constexpr (
                    element_fundamental_traits::is_fundamental == true ||
                    std::is_enum_v<element_t> == true)
                {
                    if (!has_bytes_left(value.size() * sizeof(element_t)))
                    {
                        m_error = read_error_code::format_insufficient_data;
                        return false;
                    }

                    read_contiguous_container(m_input, value, value.size());
                    return true;
                }
                else {
                    static_assert(always_false<value_t>, "Unmapped blopp format container element data type.");
                }
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp format data type.");
            }
        }

        read_span_input_type& m_input;
        std::optional<read_error_code>& m_error;

    };


    template<typename TOptions>
    class read_context {

    public:

        static constexpr auto direction = context_direction::read;

        explicit read_context(
            read_span_input_type& input
        ) :
            m_input{ input }
        {} 

        read_context(const read_context&) = delete;
        read_context(read_context&&) = delete;
        read_context& operator = (const read_context&) = delete;
        read_context& operator = (read_context&&) = delete;

        auto& map(auto& value) {
            if (m_error.has_value()) {
                return *this;
            }
            ++m_property_count;
            return map_impl<false>(value);
        }

        inline auto error() const {
            return m_error;
        }

    private:

        using options = TOptions;
        using options_string_size_type = typename options::binary_format_types::string_size_type;
        using options_object_size_type = typename options::binary_format_types::object_size_type;
        using options_object_property_count_type = typename options::binary_format_types::object_property_count_type;
        using options_list_size_type = typename options::binary_format_types::list_size_type;
        using options_list_element_count_type = typename options::binary_format_types::list_element_count_type;
        using options_format_size_type = typename options::binary_format_types::format_size_type;

        inline bool has_bytes_left(const size_t count)
        {
            return m_input.size() >= count;
        }

        inline void skip_input_bytes(const size_t byte_count) {
            m_input = m_input.subspan(byte_count);
        }

        template<bool VSkipDataType, typename T>
        inline auto read_fundamental_value(T& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<T>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return read_error_code::insufficient_data;
                }

                const auto data_type = read_value<data_types>(m_input);
                if (data_type != value_fundamental_traits::data_type) {
                    return read_error_code::mismatching_type;
                }
            }

            if (!has_bytes_left(sizeof(value_t))) {
                return read_error_code::insufficient_data;
            }

            read_value(m_input, value);

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_fundamental(auto& value) -> std::optional<read_error_code> {
            return read_fundamental_value<VSkipDataType>(value);
        }

        template<bool VSkipDataType>
        inline auto read_enum(auto& value) -> std::optional<read_error_code> {
            using value_t = std::underlying_type_t<std::remove_cvref_t<decltype(value)>>;
            return read_fundamental_value<VSkipDataType>(reinterpret_cast<value_t&>(value));
        }

        template<bool VSkipDataType>
        inline auto read_string(std::string& value) -> std::optional<read_error_code> {
            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return read_error_code::insufficient_data;
                }

                const auto data_type = read_value<data_types>(m_input);
                if (data_type != data_types::string) {
                    return read_error_code::mismatching_type;
                }
            }
            
            if (!has_bytes_left(sizeof(options_string_size_type))) {
                return read_error_code::insufficient_data;
            }

            const auto string_size = static_cast<size_t>(read_value<options_string_size_type>(m_input));

            if (!has_bytes_left(string_size)) {
                return read_error_code::insufficient_data;
            }

            value.clear();
            read_contiguous_container(m_input, value, string_size);

            return {};
        }

        inline auto read_unique_ptr(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::element_type;
            
            if (!has_bytes_left(sizeof(data_types))) {
                return read_error_code::insufficient_data;
            }

            const auto data_type = read_value<data_types>(m_input);
            if (data_type == data_types::null) {
                value = nullptr;
                return {};
            }

            value = std::make_unique<element_t>();
            map_impl<true>(*value);
        
            if (m_error.has_value()) {
                return m_error.value();
            }

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_object(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return read_error_code::insufficient_data;
                }

                const auto data_type = read_value<data_types>(m_input);
                if (data_type != data_types::object) {
                    return read_error_code::mismatching_type;
                }
            }

            if (!has_bytes_left(sizeof(options_object_size_type) + sizeof(options_object_property_count_type))) {
                return read_error_code::insufficient_data;
            }

            const auto max_object_size = m_input.size();
            const auto object_size = static_cast<size_t>(read_value<options_object_size_type>(m_input));
            if (object_size > max_object_size) {
                return read_error_code::insufficient_data;
            }

            skip_input_bytes(sizeof(options_object_property_count_type));

            auto object_read_context = read_context{ m_input };
            object<value_t>::map(object_read_context, value);

            if (object_read_context.m_error) {
                return object_read_context.m_error.value();
            }

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_list(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return read_error_code::insufficient_data;
                }

                const auto data_type = read_value<data_types>(m_input);
                if (data_type != data_types::list) {
                    return read_error_code::mismatching_type;
                }
            }

            if (!has_bytes_left(sizeof(options_list_size_type) + sizeof(data_types) + sizeof(options_list_element_count_type))) {
                return read_error_code::insufficient_data;
            }

            skip_input_bytes(sizeof(options_list_size_type));

            const auto element_data_type_raw = read_value<data_types>(m_input);
            const auto element_data_type = clean_element_flags(element_data_type_raw);
            if (element_data_type != get_data_type<element_t>()) {
                return read_error_code::mismatching_type;
            }

            const auto element_count = static_cast<size_t>(read_value<options_list_element_count_type>(m_input));
            if constexpr (is_std_array_v<value_t> == true) {
                if (element_count != value.size()) {
                    return read_error_code::mismatching_array_size;
                }
            }

            clear_container(value);

            if constexpr (element_fundamental_traits::is_fundamental == true) {
                if (!has_bytes_left(sizeof(element_t) * element_count)) {
                    return read_error_code::insufficient_data;
                }

                read_contiguous_container(m_input, value, element_count);
            }
            else {
                for (size_t i = 0; i < element_count; ++i) {
                    auto& element_value = emplace_container(value, i);
                    map_impl<true>(element_value);
                    if (m_error.has_value()) {
                        return m_error;
                    }
                }
            }

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_formatted(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return read_error_code::insufficient_data;
                }

                const auto data_type = read_value<data_types>(m_input);
                if (data_type != data_types::unspecified) {
                    return read_error_code::mismatching_type;
                }
            }

            if (!has_bytes_left(sizeof(options_format_size_type))) {
                return read_error_code::insufficient_data;
            }

            const auto object_size = static_cast<size_t>(read_value<options_format_size_type>(m_input));
            if (!has_bytes_left(object_size)) {
                return read_error_code::insufficient_data;
            }

            auto format_result = std::optional<read_error_code>{};
            auto format_read_context = read_format_context<TOptions>{ m_input, format_result };
            object<value_t>::format(format_read_context, value);
            
            return format_result;
        }

        template<bool VSkipDataType>
        inline auto& map_impl(auto& value)
        {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (value_fundamental_traits::is_fundamental == true) {
                m_error = read_fundamental<VSkipDataType>(value);
            }
            else if constexpr (std::is_enum_v<value_t> == true) {
                m_error = read_enum<VSkipDataType>(value);
            }
            else if constexpr (std::is_same_v<value_t, std::string> == true) {
                m_error = read_string<VSkipDataType>(value);
            }
            else if constexpr (is_specialization_v<value_t, std::unique_ptr> == true) {
                m_error = read_unique_ptr(value);
            }
            else if constexpr (
                is_std_array_v<value_t> == true ||
                is_specialization_v<value_t, std::vector> == true ||
                is_specialization_v<value_t, std::list> == true)
            {
                m_error = read_list<VSkipDataType>(value);
            }
            else if constexpr (object_is_mapped<value_t>() == true)
            {
                m_error = read_object<VSkipDataType>(value);
            }
            else if constexpr (object_is_formatted<value_t>() == true)
            {
                m_error = read_formatted<VSkipDataType>(value);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp data type.");
            }

            return *this;
        }

        read_span_input_type& m_input;
        uint16_t m_property_count = 0;
        std::optional<read_error_code> m_error;

    };

}


namespace blopp
{

    template<typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type {
        return write<default_write_options, T>(value);
    }

    template<typename TOptions, typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type {
        auto result = write_result_type{};
        auto context = impl::write_context<TOptions>{ result };
        context.map(value);
        return result;
    }

    template<typename T>
    [[nodiscard]] auto read(const read_input_type& input) -> expected<T, read_error_code> {
        auto span = std::span{ input };
        return read<default_read_options, T>(span);
    }

    template<typename TOptions, typename T>
    [[nodiscard]] auto read(const read_input_type& input) -> expected<T, read_error_code> {
        auto span = std::span{ input };
        return read<TOptions, T>(span);
    }

    template<typename T>
    [[nodiscard]] auto read(read_span_input_type& span) -> expected<T, read_error_code> {
        return read<default_read_options, T>(span);
    }

    template<typename TOptions, typename T>
    [[nodiscard]] auto read(read_span_input_type& span) -> expected<T, read_error_code> {
        auto output = T{};
        auto context = impl::read_context<TOptions>{ span };

        context.map(output);

        if (auto error = context.error(); error) {
#if defined(BLOPP_EXPECTED_IS_RESULT_WRAPPER)
            return expected<T, read_error_code>{error.value()};   
#else
            return std::unexpected(error.value());
#endif
        }

        return output;
    }

}

#endif