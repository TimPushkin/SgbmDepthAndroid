cmake_minimum_required(VERSION 3.16)

project("sgbm_android")

option(BUILD_EXAMPLES "Build examples" OFF)

add_subdirectory(src)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
