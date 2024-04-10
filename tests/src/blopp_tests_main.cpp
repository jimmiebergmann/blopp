#include "blopp_test.hpp"

int main(int argc, char** argv)
{
    // Command line arguments for running a single test:
    // --gtest_filter=Test.Case*

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}