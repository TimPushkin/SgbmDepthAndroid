cmake_minimum_required(VERSION 3.16)

project("sgbm_android")

option(BUILD_EXAMPLES "Build examples" OFF)

find_program(CCACHE_EXECUTABLE ccache)
if (CCACHE_EXECUTABLE)
    foreach (LANG C CXX)
        if (NOT DEFINED CMAKE_${LANG}_COMPILER_LAUNCHER)
            message(STATUS "Using ccache for ${LANG}")
            set(CMAKE_${LANG}_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
        endif ()
    endforeach ()
endif ()

add_subdirectory(src)
if (BUILD_EXAMPLES)
    message(STATUS "Will build examples")
    add_subdirectory(examples)
endif ()
