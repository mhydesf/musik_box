execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

add_subdirectory(matplotlib-cpp/)
