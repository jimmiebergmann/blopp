cmake_minimum_required(VERSION 3.16)

project (blopp_tests)

set(RootDir "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(TestDir "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(
	"${RootDir}"
	"${TestDir}"
)

add_executable(blob_static_tests
	"${TestDir}/static_tests.cpp"
	"${TestDir}/structs.hpp"
	"${RootDir}/blopp.hpp"
)


set_target_properties(blob_static_tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED OFF
  CXX_EXTENSIONS OFF
)
  
set_target_properties( blob_static_tests
  PROPERTIES
  OUTPUT_NAME_DEBUG "blob_static_tests_d"
  OUTPUT_NAME_RELEASE "blob_static_tests"
  RUNTIME_OUTPUT_DIRECTORY "${RootDir}/Bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${RootDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RootDir}/bin"
)