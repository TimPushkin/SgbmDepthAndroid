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
            URL "https://github.com/scapix-com/cmodule/archive/refs/tags/v1.0.33.tar.gz"
            URL_HASH SHA256=b56bf2f3b5acb979f1242757976eaf525caeea70714caa6e19571f65e0b60727
    )
    FetchContent_MakeAvailable(cmodule)

    find_package(Scapix REQUIRED)

    scapix_bridge_headers(${PROJECT_NAME} me.timpushkin.sgbmdepth ${HEADER_FILES})

    message(STATUS "Configuring Scapix - done")
endif ()
