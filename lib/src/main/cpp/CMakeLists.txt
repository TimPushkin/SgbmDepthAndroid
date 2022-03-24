cmake_minimum_required(VERSION 3.16)

project("sgbm_android_core")

option(BUILD_EXAMPLES "Build examples" OFF)

add_subdirectory(core)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
