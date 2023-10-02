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

#include <type_traits>
#include <expected>
#include <optional>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <span>
#include <stdint.h>
#include <stddef.h>
#include <cstring>

// Declarations
namespace blopp {

    template<typename T>
    struct object;

    enum class context_direction : uint8_t {
        read,
        write
    };

    template<typename T>
    [[nodiscard]] auto write(const T& value) -> std::vector<uint8_t>;

    enum class read_error_code {
        mismatching_type,
        insufficient_data
    };

    template<typename T>
    [[nodiscard]] auto read(const std::vector<uint8_t>& input) -> std::expected<T, read_error_code>;

    template<typename T>
    [[nodiscard]] auto read(std::span<const uint8_t>& input) -> std::expected<T, read_error_code>;
    
}


// Implementations.
namespace blopp::impl {

    enum class data_types : uint8_t {
        null = 0,
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
        list = 15
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

    template<typename T>
    constexpr bool always_false = false;

    template<typename T, template<typename...> class TRef>
    struct is_specialization : std::false_type {};

    template<template<typename...> class TRef, typename... TArgs>
    struct is_specialization<TRef<TArgs...>, TRef> : std::true_type {};

    template<typename T, template<typename...> class TRef>
    static constexpr bool is_specialization_v = is_specialization<T, TRef>::value;

    struct dummy_context {
        auto& map(auto&) { 
            return *this;
        }
    };

    template<typename T>
    constexpr auto object_is_mapped() -> bool {
        return std::is_class_v<T> == true && requires(T& value, dummy_context dummy) { { blopp::object<T>::map(dummy, value) }; };
    }

    template<typename T>
    constexpr auto container_has_reserve() -> bool {
        return requires(T & container, size_t size) { { container.reserve(size) }; };
    }

    template<typename T>
    data_types get_data_type() {
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


    template<typename T>
    class post_output_writer
    {

    public:

        post_output_writer(std::vector<uint8_t>& output) :
            m_output{ output },
            m_position{ m_output.size() }
        {
            m_output.insert(m_output.end(), sizeof(T), uint8_t{});
        }

        void update(const T value) {
            std::memcpy(m_output.data() + m_position, &value, sizeof(T));
        }

    private:

        std::vector<uint8_t>& m_output;
        size_t m_position;

    };


    class write_context {

    public:

        static constexpr auto direction = context_direction::write;

        write_context(
            std::vector<uint8_t>& output
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

        inline void write_bytes(const auto& value) {
            const auto* value_ptr = reinterpret_cast<const uint8_t*>(&value);
            m_output.insert(m_output.end(), value_ptr, value_ptr + sizeof(value));
        }

        template<typename T>
        inline void write_bytes(std::span<const T> values) {

            const auto values_ptr = reinterpret_cast<const uint8_t*>(values.data());
            m_output.insert(m_output.end(), values_ptr, values_ptr + (sizeof(T) * values.size()));
        }

        inline void write_null() {
            m_output.push_back(static_cast<uint8_t>(data_types::null));
        }

        template<bool VSkipDataType>
        inline void write_fundamental(const auto& value) {
            using value_t = std::decay_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;
            
            if constexpr (VSkipDataType == false) {
                m_output.push_back(static_cast<uint8_t>(value_fundamental_traits::data_type));
                m_byte_count += sizeof(data_types);
            }
            
            write_bytes(value);
            m_byte_count += sizeof(value_t);
        }

        template<bool VSkipDataType>
        inline void write_string(const auto& value) {
            if constexpr (VSkipDataType == false) {
                m_output.push_back(static_cast<uint8_t>(data_types::string));
                m_byte_count += sizeof(data_types);
            }

            const auto value_size = value.size();
            write_bytes(value_size);
            m_output.insert(m_output.end(), value.begin(), value.end());

            m_byte_count += sizeof(size_t) + value.size();
        }

        template<bool VSkipDataType>
        inline void write_list(const auto& value) {
            using value_t = std::decay_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            if constexpr (VSkipDataType == false) {
                m_output.push_back(static_cast<uint8_t>(data_types::list));
                m_byte_count += sizeof(data_types);
            }

            const auto block_start_position = m_byte_count;

            auto block_size_writer = post_output_writer<size_t>{ m_output };

            const auto element_data_type = get_data_type<element_t>();
            write_bytes(element_data_type);

            const size_t element_count = value.size();
            write_bytes(element_count);

            m_byte_count += sizeof(size_t) + sizeof(data_types) + sizeof(size_t);

            if constexpr (element_fundamental_traits::is_fundamental == true) {
                if constexpr (
                    std::is_same_v<element_t, bool> == false &&
                    is_specialization_v<value_t, std::vector> == true)
                {
                    write_bytes(std::span{ value });
                }
                else {
                    for (const auto element_value : value) {
                        write_bytes(element_value);
                    }
                }
                m_byte_count += sizeof(element_t) * element_count;
            }
            else {
                for (const auto& element_value : value) {
                    map_impl<true>(element_value);
                }
            }

            const auto block_size = m_byte_count - block_start_position;
            block_size_writer.update(block_size);
        };

        template<bool VSkipDataType>
        inline void write_object(const auto& value) {
            using value_t = std::decay_t<decltype(value)>;

            if constexpr (VSkipDataType == false) {
                m_output.push_back(static_cast<uint8_t>(data_types::object));
                m_byte_count += sizeof(data_types);
            }

            const auto block_start_position = m_byte_count;

            auto block_size_writer = post_output_writer<size_t>{ m_output };
            auto property_count_writer = post_output_writer<uint16_t>{ m_output };
            m_byte_count += sizeof(size_t) + sizeof(uint16_t);

            auto object_write_context = write_context{ m_output };
            object<value_t>::map(object_write_context, value);
            m_byte_count += object_write_context.m_byte_count;

            const auto block_size = m_byte_count - block_start_position;
            block_size_writer.update(block_size);
            property_count_writer.update(object_write_context.m_property_count);
        }

        template<bool VSkipDataType>
        inline auto& map_impl(auto& value) {
            using value_t = std::decay_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (value_fundamental_traits::is_fundamental == true) {
                write_fundamental<VSkipDataType>(value);
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
                is_specialization_v<value_t, std::vector> == true ||
                is_specialization_v<value_t, std::list> == true)
            {
                write_list<VSkipDataType>(value);
            }
            else if constexpr (
                std::is_class_v<value_t> == true &&
                object_is_mapped<value_t>() == true)
            {
                write_object<VSkipDataType>(value);
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp data type.");
            }

            return *this;
        }

        std::vector<uint8_t>& m_output;
        size_t m_byte_count = 0;
        uint16_t m_property_count = 0;

    };


    class read_context {

    public:

        static constexpr auto direction = context_direction::read;

        read_context(
            std::span<const uint8_t>& input
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

        inline bool has_bytes_left(const size_t count)
        {
            return m_input.size() >= count;
        }

        template<typename T>
        inline T read_input_value() {
            auto result = T{};
            std::memcpy(&result, m_input.data(), sizeof(T));
            m_input = m_input.subspan(sizeof(T));
            return result;
        }

        inline void read_input_container_values(auto& container, const size_t count) {
            using container_t = std::decay_t<decltype(container)>;
            using element_t = typename container_t::value_type;

            const auto* input_element_begin = reinterpret_cast<const element_t*>(m_input.data());
            const auto* input_element_end = input_element_begin + count;
            container.insert(container.end(), input_element_begin, input_element_end);
            m_input = m_input.subspan(count * sizeof(element_t));
        }

        inline void skip_input_bytes(const size_t byte_count) {
            m_input = m_input.subspan(byte_count);
        }

        template<bool VSkipDataType>
        inline auto read_fundamental(auto& value) -> std::expected<void, read_error_code> {
            using value_t = std::decay_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;
            
            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return std::unexpected(read_error_code::insufficient_data);
                }

                const auto data_type = read_input_value<data_types>();
                if (data_type != value_fundamental_traits::data_type) {
                    return std::unexpected(read_error_code::mismatching_type);
                }
            }

            if (!has_bytes_left(sizeof(value_t))) {
                return std::unexpected(read_error_code::insufficient_data);
            }

            value = read_input_value<value_t>();

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_string(std::string& value) -> std::expected<void, read_error_code> {
            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return std::unexpected(read_error_code::insufficient_data);
                }

                const auto data_type = read_input_value<data_types>();
                if (data_type != data_types::string) {
                    return std::unexpected(read_error_code::mismatching_type);
                }
            }
            
            if (!has_bytes_left(sizeof(size_t))) {
                return std::unexpected(read_error_code::insufficient_data);
            }

            const auto string_size = read_input_value<size_t>();

            if (!has_bytes_left(string_size)) {
                return std::unexpected(read_error_code::insufficient_data);
            }

            value.clear();
            read_input_container_values(value, string_size);

            return {};
        }

        inline auto read_unique_ptr(auto& value) -> std::expected<void, read_error_code> {
            using value_t = std::decay_t<decltype(value)>;
            using element_t = typename value_t::element_type;
            
            if (!has_bytes_left(sizeof(data_types))) {
                return std::unexpected(read_error_code::insufficient_data);
            }

            const auto data_type = read_input_value<data_types>();
            if (data_type == data_types::null) {
                value = nullptr;
                return {};
            }

            value = std::make_unique<element_t>();
            map_impl<true>(*value);
        
            if (m_error.has_value()) {
                return std::unexpected(m_error.value());
            }

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_list(auto& value) -> std::expected<void, read_error_code> {
            using value_t = std::decay_t<decltype(value)>;
            using element_t = typename value_t::value_type;
            using element_fundamental_traits = fundamental_traits<element_t>;

            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return std::unexpected(read_error_code::insufficient_data);
                }

                const auto data_type = read_input_value<data_types>();
                if (data_type != data_types::list) {
                    return std::unexpected(read_error_code::mismatching_type);
                }
            }

            if (!has_bytes_left(sizeof(size_t) + sizeof(data_types) + sizeof(size_t))) {
                return std::unexpected(read_error_code::insufficient_data);
            }

            [[maybe_unused]] const auto object_size = read_input_value<size_t>();
            
            const auto element_data_type = read_input_value<data_types>();
            if (element_data_type != get_data_type< element_t>()) {
                return std::unexpected(read_error_code::mismatching_type);
            }

            const auto element_count = read_input_value<size_t>();
    
            if constexpr (element_fundamental_traits::is_fundamental == true) {
                if (!has_bytes_left(sizeof(element_t) * element_count)) {
                    return std::unexpected(read_error_code::insufficient_data);
                }

                value.clear();
                read_input_container_values(value, element_count);
            }
            else {
                for (size_t i = 0; i < element_count; ++i) {
                    auto& element_value = value.emplace_back();
                    map_impl<true>(element_value);
                    if (m_error.has_value()) {
                        return std::unexpected(m_error.value());
                    }
                }
            }

            return {};
        }

        template<bool VSkipDataType>
        inline auto read_object(auto& value) -> std::expected<void, read_error_code> {
            using value_t = std::decay_t<decltype(value)>;

            if constexpr (VSkipDataType == false) {
                if (!has_bytes_left(sizeof(data_types))) {
                    return std::unexpected(read_error_code::insufficient_data);
                }

                const auto data_type = read_input_value<data_types>();
                if (data_type != data_types::object) {
                    return std::unexpected(read_error_code::mismatching_type);
                }
            }

            if (!has_bytes_left(sizeof(size_t) + sizeof(uint16_t))) {
                return std::unexpected(read_error_code::insufficient_data);
            }
            
            const auto max_object_size = m_input.size();
            const auto object_size = read_input_value<size_t>();
            if (object_size > max_object_size) {
                return std::unexpected(read_error_code::insufficient_data);
            }

            skip_input_bytes(sizeof(uint16_t));

            auto object_read_context = read_context{ m_input };
            object<value_t>::map(object_read_context, value);

            if (object_read_context.m_error) {
                return std::unexpected(object_read_context.m_error.value());
            }

            return {};
        }

        template<bool VSkipDataType>
        inline auto& map_impl(auto& value)
        {
            using value_t = std::decay_t<decltype(value)>;
            using value_fundamental_traits = fundamental_traits<value_t>;

            if constexpr (value_fundamental_traits::is_fundamental == true) {
                if (auto result = read_fundamental<VSkipDataType>(value); !result) {
                    m_error = result.error();
                }
            }
            else if constexpr (std::is_same_v<value_t, std::string> == true) {
                if (auto result = read_string<VSkipDataType>(value); !result) {
                    m_error = result.error();
                }
            }
            else if constexpr (is_specialization_v<value_t, std::unique_ptr> == true) {
                if (auto result = read_unique_ptr(value); !result) {
                    m_error = result.error();
                }
            }
            else if constexpr (
                is_specialization_v<value_t, std::vector> == true ||
                is_specialization_v<value_t, std::list> == true)
            {
                if (auto result = read_list<VSkipDataType>(value); !result) {
                    m_error = result.error();
                }
            }
            else if constexpr (object_is_mapped<value_t>() == true)
            {
                if (auto result = read_object<VSkipDataType>(value); !result) {
                    m_error = result.error();
                }
            }
            else {
                static_assert(always_false<value_t>, "Unmapped blopp data type.");
            }

            return *this;
        }

        std::span<const uint8_t>& m_input;
        uint16_t m_property_count = 0;
        std::optional<read_error_code> m_error;

    };

}


namespace blopp
{

    template<typename T>
    [[nodiscard]] auto write(const T& value) -> std::vector<uint8_t> {
        auto result = std::vector<uint8_t>{};
        auto context = impl::write_context{ result };
        context.map(value);
        return result;
    }

    template<typename T>
    [[nodiscard]] auto read(const std::vector<uint8_t>& input) -> std::expected<T, read_error_code> {
        auto span = std::span{ input };
        return read<T>(span);
    }

    template<typename T>
    [[nodiscard]] auto read(std::span<const uint8_t>& span) -> std::expected<T, read_error_code> {
        auto output = T{};
        auto context = impl::read_context{ span };

        context.map(output);

        if (auto error = context.error(); error) {
            return std::unexpected(error.value());
        }
        return output;
    }

}

#endif