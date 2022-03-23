cmake_minimum_required(VERSION 3.16)

project("sgbm_android_core")

set(SOURCE_FILES depth_estimator.cpp)
set(OPENCV_INSTALL_DIR ${CMAKE_BINARY_DIR}/opencv-install)

include(ExternalProject)
ExternalProject_Add(opencv
        GIT_REPOSITORY https://github.com/opencv/opencv.git
        GIT_TAG 4.5.5
        UPDATE_DISCONNECTED TRUE
        CMAKE_ARGS # https://docs.opencv.org/4.x/db/d05/tutorial_config_reference.html
        -DBUILD_TESTS=OFF
        -DBUILD_PERF_TESTS=OFF
        -DBUILD_EXAMPLES=OFF
        -DBUILD_opencv_apps=OFF
        -DBUILD_JAVA=OFF
        -DBUILD_FAT_JAVA_LIB=OFF
        -DBUILD_opencv_python2=OFF
        -DBUILD_opencv_python3=OFF
        -DWITH_GTK=OFF
        -DWITH_WIN32UI=OFF
        -DWITH_FFMPEG=OFF
        -DWITH_V4L=OFF
        -DWITH_OPENCL=ON
        -DWITH_OPENVX=ON # TODO: use OPENVX_ROOT
        -DWITH_EIGEN=ON # TODO: use EIGEN_INCLUDE_PATH
        -DWITH_IPP=ON
        -DBUILD_LIST=calib3d,imgproc,flann
        # -DCPU_BASELINE=NEON TODO: check the current platform and act accordingly
        -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
        )
include_directories(${OPENCV_INSTALL_DIR}/include/opencv4)
link_directories(${OPENCV_INSTALL_DIR}/lib)
set(OPENCV_LIBRARIES opencv_calib3d opencv_imgproc)

add_library(sgbm_android_core SHARED ${SOURCE_FILES})

add_dependencies(sgbm_android_core opencv)

if (ANDROID)
    find_library(log-lib log)
else()
    set(log-lib "")
endif(ANDROID)

target_link_libraries(sgbm_android_core ${log-lib} ${OPENCV_LIBRARIES})
