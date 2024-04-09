cmake_minimum_required(VERSION 3.18)
project(musik_box)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
execute_process(
    COMMAND pipenv --venv
    OUTPUT_VARIABLE PIPENV_VENV_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND "${PIPENV_VENV_PATH}/bin/python" -c "import sys; print(f'python{sys.version_info.major}.{sys.version_info.minor}')"
    OUTPUT_VARIABLE PYTHON_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(pybind11_DIR "${PIPENV_VENV_PATH}/lib/${PYTHON_VERSION}/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)
pybind11_add_module(musik_box SHARED musik_box_wrapper.cpp)
set_target_properties(musik_box PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

set(INIT_PY "${CMAKE_CURRENT_BINARY_DIR}/__init__.py")
if(NOT EXISTS ${INIT_PY})
    file(WRITE ${INIT_PY} "")
endif()

add_custom_command(TARGET musik_box POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:musik_box>/$<TARGET_FILE_NAME:musik_box>
            ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:musik_box>
)

add_library(musik_box_lib INTERFACE)
target_include_directories(musik_box_lib INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
)
target_link_libraries(musik_box PRIVATE musik_box_lib)

enable_testing()
add_subdirectory(test)
