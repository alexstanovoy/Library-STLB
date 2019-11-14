cmake_minimum_required(VERSION 3.9)
set(PROJECT_REVISION "0.0.1")
project(stlb_global_cmake VERSION ${PROJECT_REVISION})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

include("cmake/build_settings.cmake")
include("cmake/codecov_settings.cmake")
include("cmake/doxygen_settings.cmake")

# finalize
set(CMAKE_CXX_FLAGS             ${CXX_COMPILER_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS      ${CXX_LINKER_FLAGS})

include("testing/CMakeLists.txt")
include("docs/CMakeLists.txt")
