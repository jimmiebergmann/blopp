# Blopp binary format specification
Blopp is a little-endian binary format for serializing and deserializing C++ data structures.
Each supported data type is described as a `node`, which consist of fundamental data or other nodes.
For example, a struct consists of other nodes, representing its struct members. The same goes for containers.

All nodes start with a `data_type` byte and has optional data attached to it.

## data_type

Type: `unit8_t`

| name        | value | type                                                      |
| ----------- | ----- | --------------------------------------------------------- |
| unspecified | 0     |                                                           |
| boolean     | 1     | `bool`                                                    |
| character   | 2     | `char`                                                    |
| int8        | 3     | `int8_t`                                                  |
| int16       | 4     | `int16_t`                                                 |
| int32       | 5     | `int32_t`                                                 |
| int64       | 6     | `int64_t`                                                 |
| uint8       | 7     | `uint8_t`                                                 |
| uint16      | 8     | `uint16_t`                                                |
| uint32      | 9     | `uint32_t`                                                |
| uint64      | 10    | `uint64_t`                                                |
| float32     | 11    | `float`                                                   |
| float64     | 12    | `double`                                                  |
| string      | 13    | `std::string`                                             |
| object      | 14    | `struct`, `class`                                         |
| list        | 15    | `std::vector`, `std::list`, `std::array`, `c-style array` |
| map         | 16    | `std::map`, `std::multimap`                               |
| variant     | 17    | `std::variant`                                            |

Enum values are stored as the underlying type.

## data_type flag
Highest bit of `data_type` is set to 1 if the value is nullable(if value's type is `std::unique_ptr` or `std::optional`).

## nullable_value flag

Type: `unit8_t`

| bit | description                                                 |
| --- | ----------------------------------------------------------- |
| 1   | Set if node has value.                                      |
| 2   | Set if node references to another node. (`shared_ptr` only) |

## Size and count types
Some nodes, such as `string` consists of data for describing the string length.
Data types for these types are compile time configurable via traits, but the follow types are used by default:

| name                       | type       |
| -------------------------- | ---------- |
| offset_type                | `uint64_t` |
| string_offset_type         | `uint64_t` |
| string_char_size_type      | `uint8_t`  |
| object_property_count_type | `uint16_t` |
| list_element_count_type    | `uint64_t` |
| map_element_count_type     | `uint64_t` |
| variant_index_type         | `uint16_t` |
| format_size_type           | `uint16_t` |

## Node layout

| size | type             | Description                                                                                      |
| ---- | ---------------- | ------------------------------------------------------------------------------------------------ |
| 1    | `data_type`      | Data type of node                                                                                |
| 1    | `nullable_value` | Describes if nullable node has a value and is refers to another node                             |
|      |                  | Node data, present if `nullable_value` isnt't present, or nullable_value` has value flag is set. |

## Node data layout

### `unspecified` layout
| size                     | type               | Description |
| ------------------------ | ------------------ | ----------- |
| sizeof(format_size_type) | `format_size_type` | Always `0`  |

### `boolean` layout
| size | type   | Description |
| ---- | ------ | ----------- |
| 1    | `bool` | Value       |

### `character` layout
| size | type   | Description |
| ---- | ------ | ----------- |
| 1    | `char` | Value       |

### `int8` layout
| size | type     | Description |
| ---- | -------- | ----------- |
| 1    | `int8_t` | Value       |

### `int16` layout
| size | type      | Description |
| ---- | --------- | ----------- |
| 2    | `int16_t` | Value       |

### `int32` layout
| size | type      | Description |
| ---- | --------- | ----------- |
| 4    | `int32_t` | Value       |

### `int64` layout
| size | type      | Description |
| ---- | --------- | ----------- |
| 8    | `int64_t` | Value       |

### `uint8` layout
| size | type      | Description |
| ---- | --------- | ----------- |
| 1    | `uint8_t` | Value       |

### `uint16` layout
| size | type       | Description |
| ---- | ---------- | ----------- |
| 2    | `uint16_t` | Value       |

### `uint32` layout
| size | type       | Description |
| ---- | ---------- | ----------- |
| 4    | `uint32_t` | Value       |

### `uint64` layout
| size | type       | Description |
| ---- | ---------- | ----------- |
| 8    | `uint64_t` | Value       |

### `float32` layout
| size | type    | Description |
| ---- | ------- | ----------- |
| 4    | `float` | Value       |

### `float64` layout
| size | type     | Description |
| ---- | -------- | ----------- |
| 8    | `double` | Value       |

### `string` layout
| size                                       | type                                       | Description                                           |
| ------------------------------------------ | ------------------------------------------ | ----------------------------------------------------- |
| sizeof(string_offset_type)                 | string_offset_type                         | Offset to end of `string value`, excluding this field |
| sizeof(string_char_size_type)              | string_char_size_type                      | Size of each `character`                              |
| Size of each `character` * (Offset - `1`)  | `character`[(Offset - `1`) /  `character`] | `string value`                                        |

Strings are not null-terminated.

### `object` layout
| size                               | type                       | Description                                                |
| ---------------------------------- | -------------------------- | ---------------------------------------------------------- |
| offset_type                        | offset_type                | Offset to end of `object properties`, excluding this field |
| sizeof(object_property_count_type) | object_property_count_type | Property count                                             |
| Size of `object properties`        | `node`[Property count]     | `object properties`                                        |

### `list` layout
| size                            | type                    | Description                                            |
| ------------------------------- | ----------------------- | ------------------------------------------------------ |
| sizeof(offset_type)             | offset_type             | Offset to end of `list elements`, excluding this field |
| 1                               | `data_type`             | Element `data_type`                                    |
| sizeof(list_element_count_type) | list_element_count_type | Element count                                          |
| Size of `list elements`         | `node`[Element count]   | `list elements`                                        |

`data_type` of each `node` in `list elements` is omitted, which makes a list of integers the same size of a c-style array.

### `map` layout
| size                           | type                              | Description                                                |
| ------------------------------ | --------------------------------- | ---------------------------------------------------------- |
| sizeof(offset_type)            | offset_type                       | Offset to end of `map element pairs`, excluding this field |
| 1                              | `data_type`                       | Key `data_type`                                            |
| 1                              | `data_type`                       | Value `data_type`                                          |
| sizeof(map_element_count_type) | map_element_count_type            | Element count                                              |
| Size of `map element pairs`    | { `node`, `node` }[Element count] | `map element pairs`                                        |

`data_type` of each `node` in `map element pairs` is omitted.

### `variant` layout
| size                       | type               | Description     |
| -------------------------- | ------------------ | --------------- |
| sizeof(variant_index_type) | variant_index_type | index           |
| Size of `variant value`    | `node`             | `variant value` |

