cmake_minimum_required(VERSION 3.0)
project(stlb_testing)
enable_testing()

# set variables
file(MAKE_DIRECTORY build)
set(LIBRARY_SOURCES_DS ../src/data_structures)
set(LIBRARY_SOURCES_A  ../src/algorithms)
set(COMPILE_FLAGS      "-O2 -std=c++14")
set(OUTPUT             build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT})
add_definitions(${COMPILE_FLAGS})

# include stlb::library
include_directories(${LIBRARY_SOURCES_DS})
include_directories(${LIBRARY_SOURCES_A})
include_directories(${PROJECT_SOURCE_DIR})

# fixed_uint & bitset tests
set(TEST_SOURCE_COMBINATIONS fixed_uint/combinations.cpp)
set(TEST_SOURCE_FIBONACCI    fixed_uint/fibonacci.cpp)

# algorithm tests
set(TEST_SOURCE_RADIX_SORT_UINT8_T     algorithm/radix_sort_uint8_t.cpp)
set(TEST_SOURCE_RADIX_SORT_UINT16_T    algorithm/radix_sort_uint16_t.cpp)
set(TEST_SOURCE_RADIX_SORT_UINT32_T    algorithm/radix_sort_uint32_t.cpp)
set(TEST_SOURCE_RADIX_SORT_UINT64_T    algorithm/radix_sort_uint64_t.cpp)
set(TEST_SOURCE_RADIX_SORT_UINT128_T   algorithm/radix_sort_uint128_t.cpp)

# compilation
add_executable(combinations ${TEST_SOURCE_COMBINATIONS})
add_executable(fibonacci    ${TEST_SOURCE_FIBONACCI})
add_executable(radix_sort_uint8_t     ${TEST_SOURCE_RADIX_SORT_UINT8_T})
add_executable(radix_sort_uint16_t    ${TEST_SOURCE_RADIX_SORT_UINT16_T})
add_executable(radix_sort_uint32_t    ${TEST_SOURCE_RADIX_SORT_UINT32_T})
add_executable(radix_sort_uint64_t    ${TEST_SOURCE_RADIX_SORT_UINT64_T})
add_executable(radix_sort_uint128_t   ${TEST_SOURCE_RADIX_SORT_UINT128_T})

# add tests
add_test(fixed_uint_combinations ./${OUTPUT}/combinations)
add_test(fixed_uint_fibonacci    ./${OUTPUT}/fibonacci)
add_test(radix_sort_uint8_t      ./${OUTPUT}/radix_sort_uint8_t)
add_test(radix_sort_uint16_t     ./${OUTPUT}/radix_sort_uint16_t)
add_test(radix_sort_uint32_t     ./${OUTPUT}/radix_sort_uint32_t)
add_test(radix_sort_uint64_t     ./${OUTPUT}/radix_sort_uint64_t)
add_test(radix_sort_uint128_t    ./${OUTPUT}/radix_sort_uint128_t)

