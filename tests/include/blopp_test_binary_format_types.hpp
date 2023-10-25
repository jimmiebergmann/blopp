#ifndef BLOPP_TEST_BINARY_FORMAT_TYPES_HPP
#define BLOPP_TEST_BINARY_FORMAT_TYPES_HPP

#include <stdint.h>
#include <stddef.h>

namespace blopp_test {

    struct minimal_binary_format_types {
        using offset_type = uint16_t;
        using string_offset_type = uint8_t;
        using string_char_size_type = uint8_t;
        using object_property_count_type = uint8_t;
        using list_element_count_type = uint8_t;
        using map_element_count_type = uint8_t;
        using variant_index_type = uint8_t;
        using format_size_type = uint8_t;
    };

    struct minimal_options {
        using binary_format_types = minimal_binary_format_types;
        static constexpr auto allow_more_object_members = false;
        static constexpr auto allow_less_object_members = false;
    };

}

#endif