cmake_minimum_required(VERSION 3.18)
project(musik_box)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MB_LIB ${PROJECT_NAME}_lib)

add_subdirectory(lib/)
# add_subdirectory(test/)
add_subdirectory(vendor/)

ExternalProject_Get_Property(gnuplot-iostream INSTALL_DIR)
include_directories(${INSTALL_DIR}/src/gnuplot-iostream)

ExternalProject_Get_Property(fmt INSTALL_DIR)
include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/lib)

ExternalProject_Get_Property(fftw INSTALL_DIR)
include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/lib)

ExternalProject_Get_Property(libboost INSTALL_DIR)
include_directories(${INSTALL_DIR}/include)
link_directories(${INSTALL_DIR}/lib)

add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME}
    fmt
    fftw
    libboost
    gnuplot-iostream
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${MB_LIB}
    fftw3
    boost_iostreams
    boost_system
    boost_filesystem
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    fmt
    gnuplot-iostream
)
