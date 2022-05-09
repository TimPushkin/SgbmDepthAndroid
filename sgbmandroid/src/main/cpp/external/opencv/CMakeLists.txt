message(STATUS "Configuring OpenCV")

set(OPENCV_VERSION 4.5.5)
set(OPENCV_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install)
set(OPENCV_PUBLIC_LIBRARIES opencv_core)
set(OPENCV_PRIVATE_LIBRARIES opencv_calib3d opencv_imgproc opencv_imgcodecs opencv_flann)

if (CMAKE_CROSSCOMPILING)
    message(STATUS "Will forward toolchain and related to OpenCV for cross compiling")

    list(APPEND CROSSCOMPILE_ARGS -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME})
    list(APPEND CROSSCOMPILE_ARGS -DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION})

    if (ANDROID)
        list(APPEND CROSSCOMPILE_ARGS -DANDROID_PLATFORM=${ANDROID_PLATFORM})
        list(APPEND CROSSCOMPILE_ARGS -DANDROID_ABI=${ANDROID_ABI})
        list(APPEND CROSSCOMPILE_ARGS -DCMAKE_ANDROID_ARCH_ABI=${CMAKE_ANDROID_ARCH_ABI})
        list(APPEND CROSSCOMPILE_ARGS -DANDROID_NDK=${ANDROID_NDK})
        list(APPEND CROSSCOMPILE_ARGS -DCMAKE_ANDROID_NDK=${CMAKE_ANDROID_NDK})
        list(APPEND CROSSCOMPILE_ARGS -DANDROID_ARM_NEON=${ANDROID_ARM_NEON})
    endif ()

    list(APPEND CROSSCOMPILE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
    list(APPEND CROSSCOMPILE_ARGS -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM})
    list(APPEND CROSSCOMPILE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER})
    list(APPEND CROSSCOMPILE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})

    message(DEBUG "Cross compiling arguments are:")
    foreach (ARG ${CROSSCOMPILE_ARGS})
        message(DEBUG ${ARG})
    endforeach ()
endif ()

include(ExternalProject)
ExternalProject_Add(
        opencv
        URL https://github.com/opencv/opencv/archive/refs/tags/${OPENCV_VERSION}.zip
        CMAKE_ARGS  # https://docs.opencv.org/4.x/db/d05/tutorial_config_reference.html
        -DBUILD_TESTS=OFF
        -DBUILD_PERF_TESTS=OFF
        -DBUILD_EXAMPLES=OFF
        -DBUILD_ANDROID_EXAMPLES=OFF
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
        -DBUILD_LIST=calib3d,imgproc,imgcodecs,flann
        -DCMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
        ${CROSSCOMPILE_ARGS}
)

if (CMAKE_CROSSCOMPILING AND ANDROID)
    set(OPENCV_INCLUDE_DIR ${OPENCV_INSTALL_DIR}/sdk/native/jni/include)
    set(OPENCV_LINK_DIR ${OPENCV_INSTALL_DIR}/sdk/native/staticlibs/${ANDROID_ABI})
else ()
    set(OPENCV_INCLUDE_DIR ${OPENCV_INSTALL_DIR}/include/opencv4)
    set(OPENCV_LINK_DIR ${OPENCV_INSTALL_DIR}/lib)
endif ()

message(DEBUG "OpenCV include directory: ${OPENCV_INCLUDE_DIR}")
message(DEBUG "OpenCV link directory: ${OPENCV_LINK_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCV_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${OPENCV_LINK_DIR})
add_dependencies(${PROJECT_NAME} opencv)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENCV_PUBLIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENCV_PRIVATE_LIBRARIES})

message(STATUS "Configuring OpenCV - done")
