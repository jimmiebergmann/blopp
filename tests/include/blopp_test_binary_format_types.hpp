#ifndef BLOPP_TEST_BINARY_FORMAT_TYPES_HPP
#define BLOPP_TEST_BINARY_FORMAT_TYPES_HPP

#include <stdint.h>
#include <stddef.h>

namespace blopp_test {

    struct minimal_offset_binary_format_types {
        using string_offset_type = uint8_t;
        using string_char_size_type = uint8_t;
        using object_offset_type = uint8_t;
        using object_property_count_type = uint64_t;
        using list_offset_type = uint8_t;
        using list_element_count_type = uint64_t;
        using map_offset_type = uint8_t;
        using map_element_count_type = uint64_t;
        using variant_index_type = uint64_t;
        using format_size_type = uint8_t;
    };

    struct minimal_count_binary_format_types {
        using string_offset_type = uint64_t;
        using string_char_size_type = uint8_t;
        using object_offset_type = uint64_t;
        using object_property_count_type = uint8_t;
        using list_offset_type = uint64_t;
        using list_element_count_type = uint8_t;
        using map_offset_type = uint64_t;
        using map_element_count_type = uint8_t;
        using variant_index_type = uint8_t;
        using format_size_type = uint64_t;
    };

    struct minimal_offset_options : minimal_offset_binary_format_types {
        static constexpr auto allow_object_excess_bytes = true;
        static constexpr auto allow_more_object_members = true;
    };

    struct minimal_count_options : minimal_count_binary_format_types {
        static constexpr auto allow_object_excess_bytes = true;
        static constexpr auto allow_more_object_members = true;
    };

}

#endif