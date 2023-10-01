#include "blopp_test.hpp"

int main(int argc, char** argv)
{
    // Command line arguments for running a single test:
    // --gtest_filter=Test.Case*

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// #include "blopp.hpp"
// #include <gtest/gtest.h>

// // Your data structures.
// struct product
// {
//     uint32_t id;
//     std::string name;
//     double price;
// };

// struct store
// {
//     std::string name;
//     std::vector<product> products;
// };

// template<>
// struct blopp::object<product>
// {
//     static void map(auto& context, auto& value)
//     {
//         context
//             .map(value.id)
//             .map(value.name)
//             .map(value.price);
//     }
// };

// template<>
// struct blopp::object<store>
// {
//     static void map(auto& context, auto& value)
//     {
//         context
//             .map(value.name)
//             .map(value.products);
//     }
// };


// int main()
// {
//     auto input = store{
//         .name = "cheap meme store",
//         .products = {
//             { 
//                 .id = 1,
//                 .name = "doge",
//                 .price = 4.20
//             },
//             {
//                 .id = 2,
//                 .name = "y u no",
//                 .price = 13.37
//             }
//         }
//     };

//     /*auto input = store{
//         .name = "cheap meme store",
//         .products = {
//             .id = 1,
//             .name = "doge",
//             .prices = { 4.20, 13.37 }
//         }
//     };*/

//     /*auto input = product{
//         .id = 1,
//         .name = "doge",
//         .price = 4.20
//     };*/


//     //auto input = int32_t{ 127 };

//     auto input_bytes = blopp::write(input);
//     auto output = blopp::read<store>(input_bytes);
//     if (!output) {
//         return 1;
//     }

//     return 0;
// }