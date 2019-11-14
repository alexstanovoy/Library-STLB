include(CTest)
enable_testing()

file(GLOB TESTS "${LIBRARY_TESTS}/*.cpp")
foreach(test ${TESTS})
    get_filename_component(bare_name ${test} NAME_WE)
    add_executable(${bare_name} "${test}")
    add_test(${bare_name} "./${bare_name}")
endforeach()
