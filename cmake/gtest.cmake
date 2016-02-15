
cmake_minimum_required(VERSION 2.8.8)
project(gtest_builder C CXX)
include(ExternalProject)

ExternalProject_Add(googletest
    #    GIT_REPOSITORY https://github.com/google/googletest.git
    URL https://github.com/google/googletest/archive/release-1.7.0.tar.gz
    CMAKE_ARGS -Dgtest_force_shared_crt=ON
               -DBUILD_GTEST=ON
     PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
# Disable install step
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIR ${source_dir}/include)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBS_DIR ${binary_dir})


set(GTEST_LIBRARY ${GTEST_LIBS_DIR}/libgtest.a)
set(GTEST_LIBRARY_WITH_MAIN ${GTEST_LIBS_DIR}/libgtest_main.a)
