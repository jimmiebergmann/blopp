cmake_minimum_required(VERSION 3.16)

project (blopp_tests)

find_package(GTest REQUIRED)

set(BloppIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(BloppTestDir "${CMAKE_CURRENT_SOURCE_DIR}")
set(BloppTestIncludeDir "${BloppTestDir}/include")
set(BloppTestSourceDir "${BloppTestDir}/src")

file(GLOB_RECURSE BloppTestHeaders "${BloppTestIncludeDir}/*.hpp")
file(GLOB_RECURSE BloppTestSources "${BloppTestSourceDir}/*.cpp")

add_executable(blopp_tests
  "${BloppIncludeDir}/blopp.hpp"
	"${BloppTestHeaders}"
  "${BloppTestSources}"
)

target_include_directories(blopp_tests PUBLIC
	"${BloppIncludeDir}"
	"${BloppTestIncludeDir}"
)

set_target_properties(blopp_tests PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_link_libraries(blopp_tests gtest::gtest)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang|GNU")  
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      target_compile_options(blopp_tests PRIVATE -Wmove -std=c++2b -stdlib=libc++)
    endif()   
    target_compile_options(blopp_tests PRIVATE -Werror -Wall -Wextra -Wno-long-long)    
    if("${CMAKE_BUILD_TYPE}" MATCHES Debug)
      target_compile_options(blopp_tests PRIVATE -g)
    else()
      target_compile_options(blopp_tests PRIVATE -O3)
    endif()

  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(blopp_tests PRIVATE /W4 /WX /w44062)
  endif()

set_target_properties(blopp_tests PROPERTIES
  OUTPUT_NAME_DEBUG "blopp_tests_d"
  OUTPUT_NAME_RELEASE "blopp_tests"
  RUNTIME_OUTPUT_DIRECTORY "${BloppTestDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BloppTestDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BloppTestDir}/bin"
)