set(DOXYGEN_CONFIG 	"cmake/doxygen.cfg")

find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen was not found. Before generating a documentation, install Doxygen.")
endif()

add_custom_target(
    doc_generation ALL
    COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_CONFIG}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
