cmake_minimum_required(VERSION 3.19)
project(Polyfit_master)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)

include_directories(.)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(Polyfit_master
        main.cpp
        PolyfitBoost.hpp
        PolyfitEigen.hpp)
