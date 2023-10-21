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
#include <map>
#include <tuple>
#include <variant>
#include <typeindex>
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

    template<typename T, typename TError>
    inline result_wrapper<T, TError> make_unexpected(const TError& error) {
        return result_wrapper<T, TError>{ error };
    }

#else
    template<typename TValue, typename TError>
    using expected = std::expected<TValue, TError>;

    template<typename T, typename TError>
    inline std::unexpected<TError> make_unexpected(const TError& error) {
        return std::unexpected(error);
    }
#endif

    struct default_binary_format_types {
        using offset_type = uint64_t;
        using string_size_type = uint64_t;
        using object_property_count_type = uint16_t;
        using list_element_count_type = uint64_t;
        using map_element_count_type = uint64_t;
        using variant_index_type = uint16_t;
        using format_size_type = uint16_t;
    };

    struct compact_binary_format_types {
        using offset_type = uint32_t;
        using string_size_type = uint32_t;
        using object_property_count_type = uint16_t;
        using list_element_count_type = uint32_t;
        using map_element_count_type = uint32_t;
        using variant_index_type = uint8_t;
        using format_size_type = uint8_t;
    };

    struct default_options {
        using binary_format_types = default_binary_format_types;
        static constexpr auto allow_more_object_members = false;
        static constexpr auto allow_less_object_members = false;
    };

    struct compact_default_options {
        using binary_format_types = compact_binary_format_types;
        static constexpr auto allow_more_object_members = false;
        static constexpr auto allow_less_object_members = false;
    };


    /*enum class write_error_code {
        string_size_overflow,
        object_size_overflow,
        object_property_count_overflow,
        list_size_overflow,
        list_element_count_overflow,
        format_failure,
        format_size_overflow
    };*/

    enum class read_error_code {
        insufficient_data,
        mismatching_type,
        mismatching_nullable,
        mismatching_reference,
        mismatching_array_size,
        mismatching_object_property_count,
        bad_reference_position,
        bad_reference_type,
        bad_variant_index
    };


    using write_result_type = std::vector<uint8_t>;

    template<typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type;

    template<typename TOptions, typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type;


    using read_input_type = std::span<const uint8_t>;

    template<typename T>
    struct read_result
    {
        using value_type = T;

        T value = {};
        read_input_type remaining = {};
    };

    template<typename T>
    using read_result_type = expected<read_result<T>, read_error_code>;

    template<typename T>
    [[nodiscard]] auto read(read_input_type input) -> read_result_type<T>;

    template<typename TOptions, typename T>
    [[nodiscard]] auto read(read_input_type input) -> read_result_type<T>;


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
        boolean = 1,
        character = 2,
        int8 = 3,
        int16 = 4,
        int32 = 5,
        int64 = 6,
        uint8 = 7,
        uint16 = 8,
        uint32 = 9,
        uint64 = 10,
        float32 = 11,
        float64 = 12,
        string = 13,
        object = 14,
        list = 15,
        map = 16,
        variant = 17
    };

    enum class nullable_value_flags : uint8_t {
        is_null = 0,
        has_value = 1,
        is_reference = 2,
        has_value_and_is_reference = 3
    };


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


    template<typename T, template<typename...> typename TRef>
    struct is_specialization : std::false_type {};

    template<template<typename...> typename TRef, typename... TArgs>
    struct is_specialization<TRef<TArgs...>, TRef> : std::true_type {};

    template<typename T, template<typename...> typename TRef>
    static constexpr bool is_specialization_v = is_specialization<T, TRef>::value;

    template<typename T>
    struct is_std_array : std::false_type {};

    template<typename T, size_t Vn>
    struct is_std_array<std::array<T, Vn>> : std::true_type {};

    template<typename T>
    struct is_std_span : std::false_type {};

    template<typename T, size_t Vextent>
    struct is_std_span<std::span<T, Vextent>> : std::true_type {};

    template<typename T>
    static constexpr bool is_std_array_v = is_std_array<T>::value;

    template<typename T>
    static constexpr bool is_std_span_v = is_std_span<T>::value;

    template<typename T>
    static constexpr bool is_std_vector_v = is_specialization_v<T, std::vector>;

    template<typename T>
    static constexpr bool is_std_list_v = is_specialization_v<T, std::list>;

    template<typename T>
    static constexpr bool is_std_string_v = std::is_same_v<T, std::string>;

    template<typename T>
    static constexpr bool is_std_unique_ptr_v = is_specialization_v<T, std::unique_ptr>;

    template<typename T>
    static constexpr bool is_std_shared_ptr_v = is_specialization_v<T, std::shared_ptr>;

    template<typename T>
    static constexpr bool is_std_optional_v = is_specialization_v<T, std::optional>;

    template<typename T>
    static constexpr bool is_std_variant_v = is_specialization_v<T, std::variant>;

    template<typename T>
    static constexpr bool is_std_map_v = 
        is_specialization_v<T, std::map> ||
        is_specialization_v<T, std::multimap>;

    template<typename T>
    static constexpr bool is_nullable_v =
        is_std_unique_ptr_v<T> ||
        is_std_shared_ptr_v<T> ||
        is_std_optional_v<T>;

    struct dummy_read_write_context {
        auto& map(auto& ...) { return *this; }
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
        else if constexpr (std::is_enum_v<T> == true) {
            using underlying_t = std::underlying_type_t<T>;
            using underlying_fundamental_traits = fundamental_traits<underlying_t>;
            return underlying_fundamental_traits::data_type;
        }
        else if constexpr (is_std_string_v<T> == true) {
            return data_types::string;
        }
        else if constexpr (
            is_std_unique_ptr_v<T> == true ||
            is_std_shared_ptr_v<T> == true)
        {
            using element_t = typename T::element_type;
            return get_data_type<element_t>();
        }
        else if constexpr (is_std_optional_v<T> == true) {
            using element_t = typename T::value_type;
            return get_data_type<element_t>();
        }
        else if constexpr (
            is_std_array_v<T> == true ||
            is_std_vector_v<T> == true ||
            is_std_list_v<T> == true ||
            std::is_array_v<T> == true)
        {
            return data_types::list;
        }
        else if constexpr (is_std_map_v<T> == true) {
            return data_types::map;
        }
        else if constexpr (is_std_variant_v<T> == true) {
            return data_types::variant;
        }
        else if constexpr (std::is_class_v<T> == true && object_is_mapped<T>() == true) {
            return data_types::object;
        }
        else if constexpr (std::is_class_v<T> == true && object_is_formatted<T>() == true) {
            return data_types::unspecified;
        }
        else {
            static_assert(always_false<T>, "Unmapped blopp data type.");
        }
    }

    template<typename TValue>
    inline bool nullable_has_value([[maybe_unused]] const TValue& value) {
        if constexpr (
            is_std_unique_ptr_v<TValue> == true ||
            is_std_shared_ptr_v<TValue> == true)
        {
            return value != nullptr;
        }
        else if constexpr (is_std_optional_v<TValue> == true) {
            return value.has_value();
        }
        else {
            static_assert(always_false<TValue>, "Unmapped blopp nullable value.");
        }
    }

    template<typename T>
    inline void clear_nullable_value(T& value) {
        value.reset();
    }

    template<typename TContainer>
    inline void clear_container([[maybe_unused]] TContainer& container) {
        using container_t = std::remove_cvref_t<decltype(container)>;
        if constexpr (
            is_std_vector_v<container_t> == true ||
            is_std_list_v<container_t> == true ||
            is_std_map_v<container_t> == true)
        {
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
            auto default_value = T{};
            const auto* value_ptr = reinterpret_cast<const uint8_t*>(&default_value);
            std::copy(value_ptr, value_ptr + sizeof(T), std::back_inserter(m_output));
        }

        post_output_writer(const post_output_writer&) = delete;
        post_output_writer(post_output_writer&&) = delete;
        post_output_writer& operator = (const post_output_writer&) = delete;
        post_output_writer& operator = (post_output_writer&&) = delete;

        void update(const T value) {
            std::memcpy(m_output.data() + m_position, &value, sizeof(T));
        }

    private:

        write_result_type& m_output;
        size_t m_position;

    };

    class write_context_base {

    protected:

        explicit write_context_base(write_result_type& output) :
            m_output{ output }
        {}

        template<bool Vis_nullable>
        inline void write_data_type(data_types data_type) {
            auto value = static_cast<uint8_t>(data_type);
            if constexpr (Vis_nullable == true) {
                value = static_cast<uint8_t>(value | 0b10000000);
            }
            m_output.push_back(value);
        }

        template<typename TValue>
        inline void write_value(const TValue& value) {
            const auto* value_ptr = reinterpret_cast<const uint8_t*>(&value);
            std::copy(value_ptr, value_ptr + sizeof(value), std::back_inserter(m_output));
        }

        template<typename TContainer>
        inline void write_contiguous_container(const TContainer& container) {
            using element_t = typename TContainer::value_type;
            const auto container_ptr = reinterpret_cast<const uint8_t*>(container.data());
            const auto container_byte_count = sizeof(element_t) * container.size();
            std::copy(container_ptr, container_ptr + container_byte_count, std::back_inserter(m_output));
        }

        write_result_type& m_output;

    };

    template<typename TOptions>
    class write_format_context : private write_context_base {

    public:

        static constexpr auto direction = context_direction::write;

        explicit write_format_context(
            write_result_type& output
        ) :
            write_context_base{ output }
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

        [[nodiscard]] inline auto write_array(auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            static_assert(
                element_fundamental_traits::is_fundamental == true ||
                std::is_enum_v<element_t> == true,
                "Only fundamentals and enums are supported for blopp formatting of arrays.");

            write_contiguous_container(value);
        }

        [[nodiscard]] inline auto format_impl(auto& value) -> bool {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (
                value_fundamental_traits::is_fundamental == true ||
                std::is_enum_v<value_t> == true)
            {
                write_value(value);
            }
            else if constexpr (is_std_array_v<value_t> == true) {
                write_array(value);
            }
            else if constexpr (std::is_array_v<value_t> == true) {
                auto array_span = std::span(value);
                write_array(array_span);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp format data type.");
            }

            return m_format_size <= max_format_size;
        };

        size_t m_format_size = 0;

    };

    struct write_reference {
        uint64_t position;
        std::type_index type_index;
    };

    using write_reference_map = std::map<const void*, write_reference>;

    template<typename TOptions>
    class write_context : private write_context_base {

    public:

        static constexpr auto direction = context_direction::write;

        explicit write_context(
            write_result_type& output,
            write_reference_map& reference_map
        ) :
            write_context_base{ output },
            m_reference_map{ reference_map }
        {}

        write_context(const write_context&) = delete;
        write_context(write_context&&) = delete;
        write_context& operator = (const write_context&) = delete;
        write_context& operator = (write_context&&) = delete;

        template<typename ... T>
        auto& map(T& ... value) {
            static_assert(sizeof...(value) > 0, "Cannot pass 0 parameters to map function of blopp context.");

            (map_impl<false>(value), ...);
            m_property_count += sizeof...(value);
            return *this;
        }

    private:

        using options = TOptions;
        using options_offset_type = typename options::binary_format_types::offset_type;
        using options_string_size_type = typename options::binary_format_types::string_size_type;
        using options_object_property_count_type = typename options::binary_format_types::object_property_count_type;
        using options_list_element_count_type = typename options::binary_format_types::list_element_count_type;
        using options_map_element_count_type = typename options::binary_format_types::map_element_count_type;
        using options_variant_index_type = typename options::binary_format_types::variant_index_type;
        using options_format_size_type = typename options::binary_format_types::format_size_type;

        template<typename TValue>
        inline bool write_nullable_value_flags(const TValue& value) {           
            using value_t = std::remove_cvref_t<decltype(value)>;

            const auto has_value = nullable_has_value(value);

            if constexpr (is_std_shared_ptr_v<value_t> == true) {
                if (!has_value) {
                    m_output.push_back(static_cast<uint8_t>(nullable_value_flags::is_null));
                    return false;
                }

                auto it = m_reference_map.find(value.get());
                if (it != m_reference_map.end()) {
                    m_output.push_back(static_cast<uint8_t>(nullable_value_flags::has_value_and_is_reference));
                    write_value(it->second.position);
                    return false;
                }

                m_output.push_back(static_cast<uint8_t>(nullable_value_flags::has_value));
                return true;
            }
            else {
                static_assert(static_cast<uint8_t>(false) == static_cast<uint8_t>(nullable_value_flags::is_null),
                    "Blopp failed to cast boolean to correct nullable_value_flags. false != is_null");

                static_assert(static_cast<uint8_t>(true) == static_cast<uint8_t>(nullable_value_flags::has_value),
                    "Blopp failed to cast boolean to correct nullable_value_flags. true != has_value");

                m_output.push_back(static_cast<uint8_t>(has_value));
                return has_value;
            }
        }

        inline void write_fundamental(const auto& value) {
            write_value(value);
        }

        inline void write_enum(const auto& value) {
            using underlying_t = std::underlying_type_t<std::remove_cvref_t<decltype(value)>>;
            write_value(static_cast<underlying_t>(value));
        }

        inline void write_string(const auto& value) {
            write_value(static_cast<options_string_size_type>(value.size()));
            write_contiguous_container(value);
        }

        inline void write_shared_ptr(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::element_type;

            m_reference_map.emplace(value.get(), write_reference{
                .position = static_cast<uint64_t>(m_output.size()),
                .type_index = std::type_index(typeid(element_t))
            });

            map_impl<true>(*value);
        }

        inline void write_object(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
       
            auto block_offset_writer = post_output_writer<options_offset_type>{ m_output };
            auto property_count_writer = post_output_writer<options_object_property_count_type>{ m_output };      
            
            const auto block_start_position = m_output.size();

            auto object_write_context = write_context{ m_output, m_reference_map };
            object<value_t>::map(object_write_context, value);

            const auto block_offset = static_cast<options_offset_type>(m_output.size() - block_start_position);
            block_offset_writer.update(block_offset);

            property_count_writer.update(object_write_context.m_property_count);
        }

        inline void write_list(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            auto block_offset_writer = post_output_writer<options_offset_type>{ m_output };
            const auto block_start_position = m_output.size();

            constexpr auto element_is_nullable = is_nullable_v<element_t>;
            write_data_type<element_is_nullable>(get_data_type<element_t>());

            write_value(static_cast<options_list_element_count_type>(value.size()));

            if constexpr (element_fundamental_traits::is_fundamental == true) {
                if constexpr (std::contiguous_iterator<typename value_t::iterator> == true)
                {
                    write_contiguous_container(value);
                }
                else {
                    for (const auto element_value : value) {
                        write_value(static_cast<const element_t>(element_value));
                    }
                }
            }
            else {
                for (const auto& element_value : value) {
                    map_impl<true>(element_value);
                }
            }

            const auto block_offset = static_cast<options_offset_type>(m_output.size() - block_start_position);
            block_offset_writer.update(block_offset);
        }

        inline void write_map(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using key_t = typename value_t::key_type;
            using mapped_t = typename value_t::mapped_type;

            auto block_offset_writer = post_output_writer<options_offset_type>{ m_output };
            const auto block_start_position = m_output.size();

            constexpr auto key_is_nullable = is_nullable_v<key_t>;
            write_data_type<key_is_nullable>(get_data_type<key_t>());

            constexpr auto mapped_is_nullable = is_nullable_v<mapped_t>;
            write_data_type<mapped_is_nullable>(get_data_type<mapped_t>());

            write_value(static_cast<options_map_element_count_type>(value.size()));

            for (const auto& [key, mapped] : value)
            {
                map_impl<true>(key);
                map_impl<true>(mapped);
            }

            const auto block_offset = static_cast<options_offset_type>(m_output.size() - block_start_position);
            block_offset_writer.update(block_offset);
        }

        inline void write_variant(const auto& value) {
            
            const auto index = value.index();    
            write_value(static_cast<options_variant_index_type>(index));

            std::visit([&](const auto& variant_value) {
                map_impl<false>(variant_value);
            }, value);
        }

        inline void write_formatted(const auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;

            auto block_size_writer = post_output_writer<options_format_size_type>{ m_output };            
            const auto block_start_position = m_output.size();

            auto format_write_context = write_format_context<TOptions>{ m_output };
            if (!object<value_t>::format(format_write_context, value)) {
                // return std::unexpected(write_error_code::format_failure); // TODO
            }

            /*if (format_size > static_cast<size_t>(std::numeric_limits< options_format_size_type>::max()))
            {
                // return std::unexpected(write_error_code::format_size_overflow); // TODO
            }*/

            const auto block_size = static_cast<options_format_size_type>(m_output.size() - block_start_position);
            block_size_writer.update(block_size);
        }

        template<bool Vskip_data_type>
        inline void map_impl(auto& value) {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            constexpr auto value_is_nullable = is_nullable_v<value_t>;

            if constexpr (Vskip_data_type == false) {
                write_data_type<value_is_nullable>(get_data_type<value_t>());
            }

            if constexpr (value_is_nullable == true) {
                if (!write_nullable_value_flags(value)) {
                    return;
                }
            }

            if constexpr (value_fundamental_traits::is_fundamental == true) {
                write_fundamental(value);
            }
            else if constexpr (std::is_enum_v<value_t> == true) {
                write_enum(value);
            }
            else if constexpr (is_std_string_v<value_t> == true) {
                write_string(value);
            }
            else if constexpr (
                is_std_unique_ptr_v<value_t> == true ||
                is_std_optional_v<value_t> == true)
            {
                map_impl<true>(*value);
            }
            else if constexpr (is_std_shared_ptr_v<value_t> == true) {
                write_shared_ptr(value);
            }
            else if constexpr (
                is_std_array_v<value_t> == true ||
                is_std_vector_v<value_t> == true ||
                is_std_list_v<value_t> == true)
            {
                write_list(value);
            }
            else if constexpr (std::is_array_v<value_t> == true) {
                write_list(std::span(value));
            }
            else if constexpr (is_std_map_v<value_t> == true) {
                write_map(value);
            }
            else if constexpr (is_std_variant_v<value_t> == true) {
                write_variant(value);
            }
            else if constexpr (object_is_mapped<value_t>() == true) {
                write_object(value);
            }
            else if constexpr (object_is_formatted<value_t>() == true) {
                write_formatted(value);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp data type.");
            }
        }

        options_object_property_count_type m_property_count = 0;
        write_reference_map& m_reference_map;

    };

    class read_context_base {

    protected:

        explicit read_context_base(read_input_type& input) :
            m_input{ input }
        {}

        inline bool has_bytes_left(const size_t count)
        {
            return m_input.size() >= count;
        }

        template<typename T>
        inline bool has_bytes_left(const size_t count)
        {
            constexpr auto max_count = std::numeric_limits<size_t>::max() / sizeof(T);

            if (count > max_count) {
                return false;
            }

            return m_input.size() >= (count * sizeof(T));
        }

        inline data_types read_data_type() {
            auto next_byte_without_flag = static_cast<uint8_t>(*m_input.data() & 0b01111111);
            m_input = m_input.subspan(sizeof(uint8_t));
            return static_cast<data_types>(next_byte_without_flag);
        }

        inline uint8_t read_nullable_value_flags() {
            auto value = static_cast<uint8_t>(*m_input.data() & 0b00000011);
            m_input = m_input.subspan(sizeof(uint8_t));
            return value;
        }

        inline std::pair<data_types, bool> read_data_type_with_nullable_flag() {
            auto next_byte = *m_input.data();
            auto next_byte_without_flag = static_cast<uint8_t>(next_byte & 0b01111111);
            auto flag = static_cast<bool>(next_byte & 0b10000000);
            m_input = m_input.subspan(sizeof(uint8_t));
            return { static_cast<data_types>(next_byte_without_flag), flag };
        }

        template<typename T>
        inline T read_value() {
            auto value = T{};
            std::memcpy(&value, m_input.data(), sizeof(T));
            m_input = m_input.subspan(sizeof(T));
            return value;
        }

        template<typename T>
        inline void read_value(T& value) {
            std::memcpy(&value, m_input.data(), sizeof(T));
            m_input = m_input.subspan(sizeof(T));
        }

        template<typename TContainer>
        inline void read_container(TContainer& container, const size_t count) {
            using container_t = std::remove_cvref_t<decltype(container)>;
            using element_t = typename container_t::value_type;

            if (count == 0) {
                return;
            }

            if constexpr (
                is_std_array_v<container_t> == true ||
                is_std_span_v<container_t> == true)
            {
                const auto* src_element_ptr = m_input.data();
                auto* dest_element_ptr = container.data();
                std::memcpy(dest_element_ptr, src_element_ptr, count * sizeof(element_t));
            }
            else if constexpr (std::contiguous_iterator<typename TContainer::iterator> == true) {
                size_t old_container_size = container.size();
                container.resize(old_container_size + count);

                const auto* src_element_ptr = m_input.data();
                auto* dest_element_ptr = container.data() + old_container_size;
                std::memcpy(dest_element_ptr, src_element_ptr, count * sizeof(element_t));
            }
            else {
                auto* src_element_ptr = m_input.data();
                for (size_t i = 0; i < count; ++i) {
                    auto element_value = element_t{};
                    std::memcpy(&element_value, src_element_ptr, sizeof(element_t));
                    container.push_back(element_value);
                    src_element_ptr += sizeof(element_t);
                }
            }

            m_input = m_input.subspan(count * sizeof(element_t));
        }
        
        read_input_type& m_input;

    };


    template<typename TOptions>
    class read_format_context: private read_context_base {

    public:

        static constexpr auto direction = context_direction::write;

        explicit read_format_context(
            read_input_type& input,
            std::optional<read_error_code>& error
        ) :
            read_context_base{ input },
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

        [[nodiscard]] inline auto read_fundamental(auto& value) -> bool {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if (!has_bytes_left(sizeof(value_t)))
            {
                m_error = read_error_code::insufficient_data;
                return false;
            }

            read_value(value);
            return true;
        }

        [[nodiscard]] inline auto read_array(auto& value) -> bool {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            static_assert(
                element_fundamental_traits::is_fundamental == true ||
                std::is_enum_v<element_t> == true,
                "Only fundamentals and enums are supported for blopp formatting of arrays.");

            if (!has_bytes_left<element_t>(value.size()))
            {
                m_error = read_error_code::insufficient_data;
                return false;
            }

            read_container(value, value.size());
            return true;
        }

        [[nodiscard]] inline auto format_impl(auto& value) -> bool {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;     

            if constexpr (
                value_fundamental_traits::is_fundamental == true ||
                std::is_enum_v<value_t> == true)
            {
                return read_fundamental(value);
            }
            else if constexpr (is_std_array_v<value_t> == true) {
                return read_array(value);
            }
            else if constexpr (std::is_array_v<value_t> == true) {
                auto array_span = std::span(value);
                return read_array(array_span);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp format data type.");
            }
        }

        std::optional<read_error_code>& m_error;

    };

    struct read_reference {
        std::shared_ptr<void> pointer;
        std::type_index type_index;
    };

    using read_reference_map = std::map<uint64_t, read_reference>;

    template<typename TOptions>
    class read_context : private read_context_base {

    public:

        static constexpr auto direction = context_direction::read;

        explicit read_context(
            read_input_type& input,
            read_input_type original_input,
            read_reference_map& reference_map
        ) :
            read_context_base{ input },
            m_original_input{ original_input },
            m_reference_map{ reference_map }
        {} 

        read_context(const read_context&) = delete;
        read_context(read_context&&) = delete;
        read_context& operator = (const read_context&) = delete;
        read_context& operator = (read_context&&) = delete;

        template<typename ... T>
        auto& map(T& ... value) {
            static_assert(sizeof...(value) > 0, "Cannot pass 0 parameters to map function of blopp context.");
            
            if constexpr (sizeof...(value) == 1) {
                if (m_error.has_value()) {
                    return *this;
                }

                ((m_error = map_impl<false>(value)), ...);
                ++m_property_count;
            }
            else {
                
                (void)((!m_error.has_value() && ((m_error = map_impl<false>(value)), 1)) && ...);
                m_property_count += sizeof...(value);
            }           

            return *this;
        }


        inline auto error() const {
            return m_error;
        }

    private:

        using options = TOptions;
        using options_offset_type = typename options::binary_format_types::offset_type;
        using options_string_size_type = typename options::binary_format_types::string_size_type;
        using options_object_property_count_type = typename options::binary_format_types::object_property_count_type;
        using options_list_element_count_type = typename options::binary_format_types::list_element_count_type;
        using options_map_element_count_type = typename options::binary_format_types::map_element_count_type;
        using options_variant_index_type = typename options::binary_format_types::variant_index_type;
        using options_format_size_type = typename options::binary_format_types::format_size_type;

        static constexpr auto options_allow_more_object_members = options::allow_more_object_members;
        static constexpr auto options_allow_less_object_members = options::allow_less_object_members;

        inline void skip_input_bytes(const size_t byte_count) {
            m_input = m_input.subspan(byte_count);
        }

        template<typename T>
        inline auto read_fundamental_value(T& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<T>;

            if (!has_bytes_left(sizeof(value_t))) {
                return read_error_code::insufficient_data;
            }

            read_value(value);
            return {};
        }

        inline auto read_fundamental(auto& value) -> std::optional<read_error_code> {
            return read_fundamental_value(value);
        }

        inline auto read_enum(auto& value) -> std::optional<read_error_code> {
            using value_t = std::underlying_type_t<std::remove_cvref_t<decltype(value)>>;
            return read_fundamental_value(reinterpret_cast<value_t&>(value));
        }

        inline auto read_string(auto& value) -> std::optional<read_error_code> {
            if (!has_bytes_left(sizeof(options_string_size_type))) {
                return read_error_code::insufficient_data;
            }

            const auto string_size = static_cast<size_t>(read_value<options_string_size_type>());

            if (!has_bytes_left(string_size)) {
                return read_error_code::insufficient_data;
            }

            value.clear();
            read_container(value, string_size);

            return {};
        }

        inline auto read_unique_ptr(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::element_type;

            if (!has_bytes_left(sizeof(data_types))) {
                return read_error_code::insufficient_data;
            }

            value = std::make_unique<element_t>();
            return map_impl<true>(*value);
        }

        inline auto read_shared_ptr(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::element_type;

            if (!has_bytes_left(sizeof(uint8_t))) {
                return read_error_code::insufficient_data;
            }

            const auto nullable_value_flags = read_nullable_value_flags();

            if (!(nullable_value_flags & static_cast<uint8_t>(nullable_value_flags::has_value))) {
                clear_nullable_value(value);
                return std::nullopt;
            }

            if (nullable_value_flags & static_cast<uint8_t>(nullable_value_flags::is_reference)) {
                const auto position = read_value<uint64_t>();

                auto it = m_reference_map.find(position);
                if (it == m_reference_map.end()) {
                    return read_error_code::bad_reference_position;
                }

                if (it->second.type_index != std::type_index(typeid(element_t))) {
                    return read_error_code::bad_reference_type;
                }

                value = std::static_pointer_cast<element_t>(it->second.pointer);
                return std::nullopt;
            }

            value = std::make_shared<element_t>();

            const auto position = static_cast<uint64_t>(m_input.data() - m_original_input.data());

            m_reference_map.emplace(position, read_reference{
                .pointer = value,
                .type_index = std::type_index(typeid(element_t))
            });

            return map_impl<true>(*value);
        }

        inline auto read_optional(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;

            if (!has_bytes_left(sizeof(data_types))) {
                return read_error_code::insufficient_data;
            }

            value = std::make_optional<element_t>();
            return map_impl<true>(*value);
        }

        template<typename T, typename ... Ts>
        inline auto read_variant_value(std::variant<Ts...>& value) -> std::optional<read_error_code>
        {
            value = T{};
            return map_impl<false>(std::get<T>(value));
        }

        template<typename ... Ts>
        inline auto read_variant(std::variant<Ts...>& value) -> std::optional<read_error_code> {
            if (!has_bytes_left(sizeof(options_variant_index_type))) {
                return read_error_code::insufficient_data;
            }

            const auto index = static_cast<size_t>(read_value<options_variant_index_type>());
            if (index >= sizeof...(Ts)) {
                return read_error_code::bad_variant_index;
            }

            size_t loop_index = 0;
            auto read_variant_error = std::optional<read_error_code>{};

            (void)((index == loop_index++ && ((read_variant_error = read_variant_value<Ts>(value)), 1)) || ...);

            return read_variant_error;
        }

        inline auto read_object(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if (!has_bytes_left(sizeof(options_offset_type) + sizeof(options_object_property_count_type))) {
                return read_error_code::insufficient_data;
            }

            const auto block_offset = static_cast<size_t>(read_value<options_offset_type>());
            if (!has_bytes_left(block_offset)) {
                return read_error_code::insufficient_data;
            }

            skip_input_bytes(sizeof(options_object_property_count_type));

            auto object_read_context = read_context{ m_input, m_original_input, m_reference_map };
            object<value_t>::map(object_read_context, value);

            if (object_read_context.m_error) {
                return object_read_context.m_error.value();
            }

            return {};
        }

        inline auto read_list(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            if (!has_bytes_left(sizeof(options_offset_type) + sizeof(data_types) + sizeof(options_list_element_count_type))) {
                return read_error_code::insufficient_data;
            }

            const auto block_offset = static_cast<size_t>(read_value<options_offset_type>());
            if (!has_bytes_left(block_offset)) {
                return read_error_code::insufficient_data;
            }

            const auto [element_data_type, element_nullable_flag] = read_data_type_with_nullable_flag();
            constexpr auto element_is_nullable = is_nullable_v<element_t>;

            if (element_nullable_flag != element_is_nullable) {
                return read_error_code::mismatching_nullable;
            }

            if (element_data_type != get_data_type<element_t>()) {
                return read_error_code::mismatching_type;
            }

            const auto element_count = static_cast<size_t>(read_value<options_list_element_count_type>());
            if constexpr (is_std_array_v<value_t> == true) {
                if (element_count != value.size()) {
                    return read_error_code::mismatching_array_size;
                }
            }

            clear_container(value);

            if constexpr (element_fundamental_traits::is_fundamental == true) {
                if (!has_bytes_left<element_t>(element_count)) {
                    return read_error_code::insufficient_data;
                }

                read_container(value, element_count);
            }
            else {
                for (size_t i = 0; i < element_count; ++i) {
                    auto& element_value = emplace_container(value, i);
                    if (auto map_error = map_impl<true>(element_value); map_error) {
                        return map_error;
                    }
                }
            }

            return {};
        }

        inline auto read_map(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using key_t = typename value_t::key_type;
            using mapped_t = typename value_t::mapped_type;

            if (!has_bytes_left(
                    sizeof(options_offset_type) + sizeof(data_types) + 
                    sizeof(data_types) + sizeof(options_map_element_count_type)))
            {
                return read_error_code::insufficient_data;
            }

            const auto block_offset = static_cast<size_t>(read_value<options_offset_type>());
            if (!has_bytes_left(block_offset)) {
                return read_error_code::insufficient_data;
            }

            const auto [key_data_type, key_nullable_flag] = read_data_type_with_nullable_flag();
            constexpr auto key_is_nullable = is_nullable_v<key_t>;

            if (key_is_nullable != key_nullable_flag) {
                return read_error_code::mismatching_nullable;
            }

            if (key_data_type != get_data_type<key_t>()) {
                return read_error_code::mismatching_type;
            }

            const auto [mapped_data_type, mapped_nullable_flag] = read_data_type_with_nullable_flag();
            constexpr auto mapped_is_nullable = is_nullable_v<mapped_t>;

            if (mapped_is_nullable != mapped_nullable_flag) {
                return read_error_code::mismatching_nullable;
            }

            if (mapped_data_type != get_data_type<mapped_t>()) {
                return read_error_code::mismatching_type;
            }

            const auto element_count = static_cast<size_t>(read_value<options_map_element_count_type>());
            
            clear_container(value);

            for (size_t i = 0; i < element_count; ++i) {

                auto key_value = key_t{};

                if (auto map_error = map_impl<true>(key_value); map_error) {
                    return map_error;
                }

                auto mapped_value = mapped_t{};

                if (auto map_error = map_impl<true>(mapped_value); map_error) {
                    return map_error;
                }

                value.insert({ std::move(key_value), std::move(mapped_value) });
            }

            return {};
        }

        inline auto read_formatted(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;

            if (!has_bytes_left(sizeof(options_format_size_type))) {
                return read_error_code::insufficient_data;
            }

            const auto object_size = static_cast<size_t>(read_value<options_format_size_type>());
            if (!has_bytes_left(object_size)) {
                return read_error_code::insufficient_data;
            }

            auto format_result = std::optional<read_error_code>{};
            auto format_read_context = read_format_context<TOptions>{ m_input, format_result };
            object<value_t>::format(format_read_context, value);

            return format_result;
        }

        template<bool Vskip_data_type>
        inline auto map_impl(auto& value) -> std::optional<read_error_code> {
            using value_t = std::remove_cvref_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            constexpr auto value_is_nullable = is_nullable_v<value_t>;

            if constexpr (Vskip_data_type == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return read_error_code::insufficient_data;
                }

                const auto [data_type, nullable_flag] = read_data_type_with_nullable_flag();
            
                if (nullable_flag != value_is_nullable) {
                    return read_error_code::mismatching_nullable;
                }

                const auto value_data_type = get_data_type<value_t>();
                if (data_type != value_data_type) {
                    return read_error_code::mismatching_type;
                }
            }

            if constexpr (value_is_nullable == true && is_std_shared_ptr_v<value_t> == false) {
                if (!has_bytes_left(sizeof(uint8_t))) {
                    return read_error_code::insufficient_data;
                }

                const auto nullable_value_flags = read_nullable_value_flags();

                if (nullable_value_flags & static_cast<uint8_t>(nullable_value_flags::is_reference)) {
                    return read_error_code::mismatching_reference;
                }

                if (!nullable_value_flags)
                {
                    clear_nullable_value(value);
                    return std::nullopt;
                }
            }

            if constexpr (value_fundamental_traits::is_fundamental == true) {
                return read_fundamental(value);
            }
            else if constexpr (std::is_enum_v<value_t> == true) {
                return read_enum(value);
            }
            else if constexpr (is_std_string_v<value_t> == true) {
                return read_string(value);
            }
            else if constexpr (is_std_unique_ptr_v<value_t> == true) {
                return read_unique_ptr(value);
            }
            else if constexpr (is_std_shared_ptr_v<value_t> == true) {
                return read_shared_ptr(value);
            }
            else if constexpr (is_std_optional_v<value_t> == true) {
                return read_optional(value);
            }
            else if constexpr (
                is_std_array_v<value_t> == true ||
                is_std_vector_v<value_t> == true ||
                is_std_list_v<value_t> == true)
            {
                return read_list(value);
            }
            else if constexpr (std::is_array_v<value_t> == true) {
                auto array_span = std::span(value);
                return read_list(array_span);
            }
            else if constexpr (is_std_variant_v<value_t> == true) {
                return read_variant(value);
            }
            else if constexpr (is_std_map_v<value_t> == true) {
                return read_map(value);
            }
            else if constexpr (object_is_mapped<value_t>() == true)
            {
                return read_object(value);
            }
            else if constexpr (object_is_formatted<value_t>() == true)
            {
                return read_formatted(value);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp data type.");
            }
        }

        std::optional<read_error_code> m_error = {};
        uint16_t m_property_count = 0;   
        read_input_type m_original_input;
        read_reference_map& m_reference_map;

    };

}


namespace blopp
{

    template<typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type {
        return write<default_options, T>(value);
    }

    template<typename TOptions, typename T>
    [[nodiscard]] auto write(const T& value) -> write_result_type {
        auto result = write_result_type{};

        impl::write_reference_map reference_map = {};

        auto context = impl::write_context<TOptions>{
            result,
            reference_map
        };

        context.map(value);
        return result;
    }


    template<typename T>
    [[nodiscard]] auto read(read_input_type input) -> read_result_type<T> {
        return read<default_options, T>(input);
    }

    template<typename TOptions, typename T>
    [[nodiscard]] auto read(read_input_type input) -> read_result_type<T> {
        auto result = read_result<T>{};

        read_input_type input_remaining = input;
        impl::read_reference_map reference_map = {};
        
        auto context = impl::read_context<TOptions>{ 
            input_remaining,
            input_remaining,
            reference_map
        };

        context.map(result.value);

        if (auto error = context.error(); error) {
            return make_unexpected<read_result<T>, read_error_code>(error.value());
        }

        result.remaining = input_remaining;
        return result;
    }

}

#endif