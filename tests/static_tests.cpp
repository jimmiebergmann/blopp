#include "structs.hpp"

int main()
{
    { // blop_1
        static_assert(blopp::size<blob_1>::value == 13);
        static_assert(blopp::size_v<blob_1> == 13);

        static_assert(blopp::members<blob_1>::value.size() == 3);
        static_assert(blopp::members<blob_1>::value[0].size == 4);
        static_assert(blopp::members<blob_1>::value[1].size == 1);
        static_assert(blopp::members<blob_1>::value[2].size == 8);
        static_assert(blopp::members<blob_1>::value[0].offset == 0);
        static_assert(blopp::members<blob_1>::value[1].offset == 4);
        static_assert(blopp::members<blob_1>::value[2].offset == 8);

        static_assert(blopp::members_v<blob_1>.size() == 3);
        static_assert(blopp::members_v<blob_1>[0].size == 4);
        static_assert(blopp::members_v<blob_1>[1].size == 1);
        static_assert(blopp::members_v<blob_1>[2].size == 8);
        static_assert(blopp::members_v<blob_1>[0].offset == 0);
        static_assert(blopp::members_v<blob_1>[1].offset == 4);
        static_assert(blopp::members_v<blob_1>[2].offset == 8);

        static_assert(blopp::members<blob_1>::value[0].raw_offset == 0);
        static_assert(blopp::members<blob_1>::value[1].raw_offset == 4);
        static_assert(blopp::members<blob_1>::value[2].raw_offset == 5);
        static_assert(blopp::members_v<blob_1>[0].raw_offset == 0);
        static_assert(blopp::members_v<blob_1>[1].raw_offset == 4);
        static_assert(blopp::members_v<blob_1>[2].raw_offset == 5);
    }
    { // blob_2
        static_assert(blopp::size<blob_2>::value == 13);
        static_assert(blopp::size_v<blob_2> == 13);

        static_assert(blopp::members<blob_2>::value.size() == 3);
        static_assert(blopp::members<blob_2>::value[0].size == 8);
        static_assert(blopp::members<blob_2>::value[1].size == 1);
        static_assert(blopp::members<blob_2>::value[2].size == 4);
        static_assert(blopp::members<blob_2>::value[0].offset == 8);
        static_assert(blopp::members<blob_2>::value[1].offset == 4);
        static_assert(blopp::members<blob_2>::value[2].offset == 0);

        static_assert(blopp::members_v<blob_2>.size() == 3);
        static_assert(blopp::members_v<blob_2>[0].size == 8);
        static_assert(blopp::members_v<blob_2>[1].size == 1);
        static_assert(blopp::members_v<blob_2>[2].size == 4);
        static_assert(blopp::members_v<blob_2>[0].offset == 8);
        static_assert(blopp::members_v<blob_2>[1].offset == 4);
        static_assert(blopp::members_v<blob_2>[2].offset == 0);

        static_assert(blopp::members<blob_2>::value[0].raw_offset == 0);
        static_assert(blopp::members<blob_2>::value[1].raw_offset == 8);
        static_assert(blopp::members<blob_2>::value[2].raw_offset == 9);
        static_assert(blopp::members_v<blob_2>[0].raw_offset == 0);
        static_assert(blopp::members_v<blob_2>[1].raw_offset == 8);
        static_assert(blopp::members_v<blob_2>[2].raw_offset == 9);
    }

    return 0;
}