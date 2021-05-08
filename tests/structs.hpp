#ifndef BLOPP_TESTS_STRUCTS_HPP
#define BLOPP_TESTS_STRUCTS_HPP

#include "../blopp.hpp"

struct blob_1
{

    int32_t a;
    bool b;
    double c;

    template<typename t_context>
    static constexpr auto blopp_map(t_context context)
    {
        return context.map(
            blopp_map_member(blob_1, a),
            blopp_map_member(blob_1, b),
            blopp_map_member(blob_1, c));
    }
};

struct blob_2
{

    int32_t a;
    bool b;
    double c;

    template<typename t_context>
    static constexpr auto blopp_map(t_context context)
    {
        return context.map(
            blopp_map_member(blob_2, c),
            blopp_map_member(blob_2, b),
            blopp_map_member(blob_2, a));
    }
};

#endif