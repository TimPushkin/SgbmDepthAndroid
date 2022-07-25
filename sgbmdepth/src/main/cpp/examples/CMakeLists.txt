# Depth estimation example
add_executable(get_depth_example GetDepthExample.cpp)

target_link_libraries(get_depth_example ${PROJECT_NAME})

# Required for CMake version less than 3.20
if (DEFINED SCAPIX_BRIDGE AND CMAKE_VERSION VERSION_LESS 3.20)
    scapix_fix_sources(${PROJECT_NAME})
endif ()
