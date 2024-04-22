cmake_minimum_required(VERSION 3.18)
project(musik_box)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_REQUIRED True)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_subdirectory(lib/)
add_subdirectory(test/)
add_subdirectory(vendor/)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    musik_box_lib
    Python3::Python
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${NUMPY_INCLUDE_DIR}
    vendor/matplotlib-cpp/
)
