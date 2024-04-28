enable_testing()
file(GLOB TEST_SOURCES "*.cpp")

foreach(testSrc ${TEST_SOURCES})
    get_filename_component(testName ${testSrc} NAME_WE)

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
    
    add_executable(${testName} ${testSrc})
    add_dependencies(${testName}
        fmt
        fftw
        libboost
        gnuplot-iostream
    )
    target_link_libraries(${testName} PRIVATE
        ${MB_LIB}
        fftw3
        boost_iostreams
        boost_system
        boost_filesystem
    )
    target_link_libraries(${testName} INTERFACE
        fmt
        gnuplot-iostream
    )
    add_test(NAME ${testName} COMMAND ${testName})
endforeach(testSrc)
