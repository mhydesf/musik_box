include(ExternalProject)

ExternalProject_Add(
    libboost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.85.0/source/boost_1_85_0.tar.bz2
    PREFIX ${CMAKE_BINARY_DIR}/libboost
    CONFIGURE_COMMAND ./bootstrap.sh --prefix=${CMAKE_BINARY_DIR}/libboost
    BUILD_COMMAND ./b2 --prefix=${CMAKE_BINARY_DIR}/libboost --with-iostreams --with-system --with-filesystem link=shared
    INSTALL_COMMAND ./b2 --prefix=${CMAKE_BINARY_DIR}/libboost install
    BUILD_IN_SOURCE 1
)

ExternalProject_Add(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 9.1.0
    PREFIX ${CMAKE_BINARY_DIR}/fmt
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DFMT_DOC=OFF -DFMT_TEST=OFF
)

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
