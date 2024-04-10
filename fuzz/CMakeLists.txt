cmake_minimum_required(VERSION 3.16)

project (blopp_fuzz)

if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  message(SEND_ERROR "Requires Clang to build blopp fuzz.")
endif()

set(BloppIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(BloppFuzzDir "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(blopp_fuzz 
  "${BloppFuzzDir}/blopp_fuzz.cpp"
)

target_include_directories(blopp_fuzz PUBLIC
	"${BloppIncludeDir}"
)

set_target_properties(blopp_fuzz PROPERTIES
  CXX_STANDARD 23
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_link_libraries(blopp_fuzz PRIVATE -fsanitize=fuzzer,undefined,address)
target_compile_options(blopp_fuzz PRIVATE -fsanitize=fuzzer,undefined,address)

set_target_properties(blopp_fuzz PROPERTIES
  OUTPUT_NAME_DEBUG "blopp_fuzz_d"
  OUTPUT_NAME_RELEASE "blopp_fuzz"
  RUNTIME_OUTPUT_DIRECTORY "${BloppFuzzDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BloppFuzzDir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BloppFuzzDir}/bin"
)
