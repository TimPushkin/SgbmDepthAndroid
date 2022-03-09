cmake_minimum_required(VERSION 3.18.1)

project("sgbm_android")

set(SOURCE_FILES sgbm_android_lib.cpp depth_estimator.cpp)

add_library(sgbm_android SHARED ${SOURCE_FILES})

find_library(log-lib log)
find_package(OpenCV 4.5 REQUIRED java)

target_link_libraries(sgbm_android ${log-lib} ${OpenCV_LIBRARIES})
