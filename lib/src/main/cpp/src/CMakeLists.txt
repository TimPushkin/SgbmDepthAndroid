project("sgbm_android_core")

set(SOURCE_FILES DepthEstimator.cpp)
set(OPENCV_INSTALL_DIR ${CMAKE_BINARY_DIR}/opencv-install)
set(OPENCV_PUBLIC_LIBRARIES opencv_core)
set(OPENCV_PRIVATE_LIBRARIES ${OPENCV_PUBLIC_LIBRARIES} opencv_calib3d opencv_imgproc opencv_imgcodecs opencv_flann)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

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
        -DBUILD_LIST=calib3d,imgproc,imgcodecs,flann
        # -DCPU_BASELINE=NEON TODO: check the current platform and act accordingly
        -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
        )
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INSTALL_DIR}/include/opencv4)
target_link_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INSTALL_DIR}/lib)
add_dependencies(${PROJECT_NAME} opencv)

if (ANDROID)
    find_library(log-lib log)
else ()
    set(log-lib "")
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENCV_PUBLIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${log-lib} ${OPENCV_PRIVATE_LIBRARIES})
