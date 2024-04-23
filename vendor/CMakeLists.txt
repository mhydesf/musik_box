include(ExternalProject)

ExternalProject_Add(
    fftw
    URL http://www.fftw.org/fftw-3.3.8.tar.gz
    PREFIX ${CMAKE_BINARY_DIR}/fftw
    CONFIGURE_COMMAND ./configure --prefix=${CMAKE_BINARY_DIR}/fftw --enable-shared
    BUILD_COMMAND make -j22
    BUILD_IN_SOURCE 1
)

ExternalProject_Add(
    gnuplot-iostream
    GIT_REPOSITORY https://github.com/dstahlke/gnuplot-iostream.git
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX ${CMAKE_BINARY_DIR}/gnuplot-iostream
)
