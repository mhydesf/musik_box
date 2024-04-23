enable_testing()
file(GLOB TEST_SOURCES "*.cpp")

foreach(testSrc ${TEST_SOURCES})
    include_directories(${CMAKE_BINARY_DIR}/fftw/include)
    include_directories(${CMAKE_BINARY_DIR}/gnuplot-iostream/src/gnuplot-iostream)
    link_directories(${CMAKE_BINARY_DIR}/fftw/lib)
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName} PRIVATE
        ${MB_LIB}
        fftw3
        ${Boost_LIBRARIES}
    )
    add_test(NAME ${testName} COMMAND ${testName})
endforeach(testSrc)
