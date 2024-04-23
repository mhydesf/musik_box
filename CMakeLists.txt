cmake_minimum_required(VERSION 3.18)
project(musik_box)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(MB_LIB ${PROJECT_NAME}_lib)

find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)

add_subdirectory(lib/)
add_subdirectory(test/)
add_subdirectory(vendor/)

include_directories(${CMAKE_BINARY_DIR}/fftw/include)
include_directories(${CMAKE_BINARY_DIR}/gnuplot-iostream/src/gnuplot-iostream)
link_directories(${CMAKE_BINARY_DIR}/fftw/lib)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME} fftw)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${MB_LIB}
    fftw3
    ${Boost_LIBRARIES}
)
