cmake_minimum_required(VERSION 3.18)

project(sgbmdepth)

option(BUILD_EXAMPLES "Build examples" OFF)

# Use ccache if available
find_program(CCACHE_EXECUTABLE ccache)
if (CCACHE_EXECUTABLE)
    foreach (LANG C CXX)
        if (NOT DEFINED CMAKE_${LANG}_COMPILER_LAUNCHER)
            message(STATUS "Will use ccache for ${LANG}")
            set(CMAKE_${LANG}_COMPILER_LAUNCHER ${CCACHE_EXECUTABLE})
        endif ()
    endforeach ()
endif ()

add_subdirectory(src)
add_subdirectory(external)

if (BUILD_EXAMPLES)
    message(STATUS "Will build examples")
    add_subdirectory(examples)
endif ()
