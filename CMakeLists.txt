cmake_minimum_required(VERSION 3.18)
project(musik_box)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
find_package(fmt REQUIRED)

add_subdirectory(lib/)
add_subdirectory(test/)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(vendor/gnuplot-iostream)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    musik_box_lib
    fftw3
    ${Boost_LIBRARIES}
)
