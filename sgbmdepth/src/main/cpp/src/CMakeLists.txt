set(SOURCE_FILES DepthEstimator.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# TODO: move Scapix configuration into `external` folder
if (DEFINED SCAPIX_BRIDGE)
    message(STATUS "Configuring Scapix for ${SCAPIX_BRIDGE}")

    set(HEADER_FILES ${CMAKE_SOURCE_DIR}/include/DepthEstimator.h)

    include(FetchContent)
    FetchContent_Declare(
            cmodule
            URL "https://github.com/scapix-com/cmodule/archive/refs/tags/v1.0.35.tar.gz"  # Build fails with v1.0.33
            URL_HASH SHA256=3eeabad538e9e06bc21f4ccafccfa97808b8458044ee32f28a37b81e7ce8d174
    )
    FetchContent_MakeAvailable(cmodule)

    find_package(Scapix REQUIRED)

    scapix_bridge_headers(${PROJECT_NAME} me.timpushkin.sgbmdepth ${HEADER_FILES})

    message(STATUS "Configuring Scapix - done")
endif ()
