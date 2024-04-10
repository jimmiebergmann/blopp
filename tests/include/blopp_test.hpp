#ifndef BLOPP_TEST_HPP
#define BLOPP_TEST_HPP

#include "blopp.hpp"
#include <gtest/gtest.h>
#include <iostream>

namespace blopp_test {

    inline void print_info(const std::string& message) {
        std::cout << "\033[0;32m" << "[          ] " << "\033[0;0m";
        std::cout << "\033[0;36m" << message << "\033[0;0m" << std::endl;
    }

}

#endif