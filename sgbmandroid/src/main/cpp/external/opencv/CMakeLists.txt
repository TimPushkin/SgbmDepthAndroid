message(STATUS "Configuring OpenCV")

set(OPENCV_VERSION 4.5.5)
set(OPENCV_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install)
set(OPENCV_PUBLIC_LIBRARIES opencv_core)
set(OPENCV_PRIVATE_LIBRARIES opencv_calib3d opencv_imgproc opencv_imgcodecs opencv_flann)

if (ANDROID)
    message(STATUS "Will use OpenCV Android SDK")

    if (NOT EXISTS ${OPENCV_INSTALL_DIR}/OpenCV-android-sdk/sdk/native/jni/OpenCVConfig.cmake)
        message(STATUS "OpenCV Android SDK not extracted in ${OPENCV_INSTALL_DIR}")

        set(OPENCV_ARCHIVE opencv-${OPENCV_VERSION}-android-sdk.zip)

        if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${OPENCV_ARCHIVE})
            message(STATUS "OpenCV Android SDK archive not found in ${CMAKE_CURRENT_BINARY_DIR}")

            message(STATUS "Downloading OpenCV Android SDK")
            file(DOWNLOAD "https://github.com/opencv/opencv/releases/download/${OPENCV_VERSION}/${OPENCV_ARCHIVE}" ${CMAKE_CURRENT_BINARY_DIR}/${OPENCV_ARCHIVE})
            message(STATUS "Downloading OpenCV Android SDK - done")
        endif ()

        message(STATUS "Extracting OpenCV Android SDK")
        file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/${OPENCV_ARCHIVE} DESTINATION ${OPENCV_INSTALL_DIR} PATTERNS OpenCV-android-sdk/sdk/native/*)
        message(STATUS "Extracting OpenCV Android SDK - done")
    endif ()

    set(OpenCV_DIR ${OPENCV_INSTALL_DIR}/OpenCV-android-sdk/sdk/native/jni)

    find_package(OpenCV REQUIRED ${OPENCV_PUBLIC_LIBRARIES} ${OPENCV_PRIVATE_LIBRARIES})
else ()
    message(STATUS "Will build OpenCV from sources")

    message(STATUS "Adding OpenCV as an external project")
    include(ExternalProject)
    ExternalProject_Add(opencv
            GIT_REPOSITORY https://github.com/opencv/opencv.git
            GIT_TAG ${OPENCV_VERSION}
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
            -DWITH_OPENVX=ON  # TODO: use OPENVX_ROOT
            -DWITH_EIGEN=ON  # TODO: use EIGEN_INCLUDE_PATH
            -DWITH_IPP=ON
            -DBUILD_LIST=calib3d,imgproc,imgcodecs,flann
            # -DCPU_BASELINE=NEON TODO: act accordingly to the current platform
            -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
            )
    message(STATUS "Adding OpenCV as an external project - done")

    target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INSTALL_DIR}/include/opencv4)
    target_link_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INSTALL_DIR}/lib)
    add_dependencies(${PROJECT_NAME} opencv)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENCV_PUBLIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENCV_PRIVATE_LIBRARIES})

message(STATUS "Configuring OpenCV - done")
