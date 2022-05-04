set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/DepthEstimator.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

message(STATUS "Configuring Scapix")

include(FetchContent)
FetchContent_Declare(
        cmodule
        URL "https://github.com/scapix-com/cmodule/archive/refs/tags/v1.0.32.tar.gz"
        URL_HASH SHA256=e6937c95d73188a5add846b27cd00160fe4f6ea31af33be417d2ab7ac19bd8d2
)
FetchContent_MakeAvailable(cmodule)

set(SCAPIX_BRIDGE "java" CACHE STRING "cpp, java, objc, python, js, cs")
#set(SCAPIX_JAVA_API "android-28" CACHE STRING "one of the folders inside 'scapix/java_api': jdk-11.0.2, android-28, etc.")

find_package(Scapix REQUIRED)

set(HEADER_FILES ${CMAKE_SOURCE_DIR}/include/DepthEstimator.h)

scapix_bridge_headers(${PROJECT_NAME} me.timpushkin.sgbmandroid ${HEADER_FILES})

message(STATUS "Configuring Scapix - done")

#target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
