#include "blopp_test.hpp"

namespace {
    TEST(conversion_overflows, asserts) {
        // Small to small
        static_assert(!blopp::impl::conversion_overflows<int8_t, int8_t>(int8_t{ -127 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, int8_t>(int8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, int8_t>(int8_t{ 127 }), "");     

        static_assert(blopp::impl::conversion_overflows<int8_t, uint8_t>(int8_t{ -1 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, uint8_t>(int8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, uint8_t>(int8_t{ 127 }), "");

        static_assert(!blopp::impl::conversion_overflows<uint8_t, int8_t>(uint8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint8_t, int8_t>(uint8_t{ 127 }), "");
        static_assert(blopp::impl::conversion_overflows<uint8_t, int8_t>(uint8_t{ 200 }), "");

        static_assert(!blopp::impl::conversion_overflows<uint8_t, uint8_t>(uint8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint8_t, uint8_t>(uint8_t{ 127 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint8_t, uint8_t>(uint8_t{ 200 }), "");


        // Small to big
        static_assert(!blopp::impl::conversion_overflows<int8_t, int16_t>(int8_t{ -127 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, int16_t>(int8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, int16_t>(int8_t{ 127 }), "");
        
        static_assert(blopp::impl::conversion_overflows<int8_t, uint16_t>(int8_t{ -127 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, uint16_t>(int8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<int8_t, uint16_t>(int8_t{ 127 }), "");
        
        static_assert(!blopp::impl::conversion_overflows<uint8_t, int16_t>(uint8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint8_t, int16_t>(uint8_t{ 127 }), "");

        static_assert(!blopp::impl::conversion_overflows<uint8_t, uint16_t>(uint8_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint8_t, uint16_t>(uint8_t{ 127 }), "");


        // Big to small
        static_assert(blopp::impl::conversion_overflows<int16_t, int8_t>(int16_t{ -2000 }), "");
        static_assert(!blopp::impl::conversion_overflows<int16_t, int8_t>(int16_t{ -127 }), "");
        static_assert(!blopp::impl::conversion_overflows<int16_t, int8_t>(int16_t{ 127 }), "");
        static_assert(blopp::impl::conversion_overflows<int16_t, int8_t>(int16_t{ 2000 }), "");

        static_assert(blopp::impl::conversion_overflows<int16_t, uint8_t>(int16_t{ -2000 }), "");
        static_assert(blopp::impl::conversion_overflows<int16_t, uint8_t>(int16_t{ -127 }), "");
        static_assert(!blopp::impl::conversion_overflows<int16_t, uint8_t>(int16_t{ 127 }), "");
        static_assert(blopp::impl::conversion_overflows<int16_t, uint8_t>(int16_t{ 2000 }), "");

        static_assert(!blopp::impl::conversion_overflows<uint16_t, int8_t>(uint16_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint16_t, int8_t>(uint16_t{ 127 }), "");
        static_assert(blopp::impl::conversion_overflows<uint16_t, int8_t>(uint16_t{ 2000 }), "");

        static_assert(!blopp::impl::conversion_overflows<uint16_t, uint8_t>(uint16_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<uint16_t, uint8_t>(uint16_t{ 200 }), "");
        static_assert(blopp::impl::conversion_overflows<uint16_t, uint8_t>(uint16_t{ 2000 }), "");


        // To float
        static_assert(!blopp::impl::conversion_overflows<int16_t, double>(int16_t{ -2000 }), "");
        static_assert(!blopp::impl::conversion_overflows<int16_t, double>(int16_t{ 0 }), "");
        static_assert(!blopp::impl::conversion_overflows<int16_t, double>(int16_t{ 2000 }), "");


        // From float
        static_assert(blopp::impl::conversion_overflows<double, int16_t>(double{ -200000.0 }), "");
        static_assert(!blopp::impl::conversion_overflows<double, int16_t>(double{ -2000.0 }), "");
        static_assert(!blopp::impl::conversion_overflows<double, int16_t>(double{ 0.0 }), "");
        static_assert(!blopp::impl::conversion_overflows<double, int16_t>(double{ 2000.0 }), "");
        static_assert(blopp::impl::conversion_overflows<double, int16_t>(double{ 200000.0 }), "");

    }

}