set(PROJECT_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

set(OPENCV_VERSION 4.5.5)
set(OPENCV_DOWNLOAD_DIR ${PROJECT_BUILD_DIR}/opencv-download)
set(OPENCV_BUILD_DIR ${PROJECT_BUILD_DIR}/opencv)
set(OPENCV_PUBLIC_LIBRARIES opencv_core)
set(OPENCV_PRIVATE_LIBRARIES opencv_calib3d opencv_imgproc opencv_imgcodecs opencv_flann)

# Stop the script if OpenCV is already built
# if (EXISTS ${OPENCV_BUILD_DIR}/OpenCVConfig.cmake)
#     message(STATUS "OpenCV already built: found OpenCVConfig.cmake in ${OPENCV_BUILD_DIR}")
#     return()
# endif ()

# Download and extract OpenCV sources
if (NOT EXISTS ${OPENCV_DOWNLOAD_DIR}/opencv-${OPENCV_VERSION}/CMakeLists.txt)
    message(STATUS "OpenCV not extracted into ${OPENCV_DOWNLOAD_DIR}")

    if (NOT EXISTS ${OPENCV_DOWNLOAD_DIR}/${OPENCV_VERSION}.zip)
        message(STATUS "OpenCV not downloaded into ${OPENCV_DOWNLOAD_DIR}")

        message(STATUS "Downloading OpenCV")
        file(
                DOWNLOAD
                https://github.com/opencv/opencv/archive/refs/tags/${OPENCV_VERSION}.zip
                ${OPENCV_DOWNLOAD_DIR}/${OPENCV_VERSION}.zip
                SHOW_PROGRESS
        )
        message(STATUS "Downloading OpenCV - done")
    endif ()

    message(STATUS "Extracting OpenCV")
    file(
            ARCHIVE_EXTRACT
            INPUT ${OPENCV_DOWNLOAD_DIR}/${OPENCV_VERSION}.zip
            DESTINATION ${OPENCV_DOWNLOAD_DIR}
    )
    message(STATUS "Extracting OpenCV - done")
endif ()

# Create a string of comma separated OpenCV modules to build
foreach (OPENCV_LIB ${OPENCV_PUBLIC_LIBRARIES} ${OPENCV_PRIVATE_LIBRARIES})
    string(REPLACE "opencv_" "" OPENCV_MODULE ${OPENCV_LIB})
    list(APPEND OPENCV_MODULES ${OPENCV_MODULE})
endforeach ()
string(REPLACE ";" "," OPENCV_MODULES "${OPENCV_MODULES}")
message(DEBUG "OpenCV modules to be build: ${OPENCV_MODULES}")

# Set toolchain-related flags
if (CMAKE_GENERATOR)
    list(APPEND TOOLCHAIN_ARGS -DCMAKE_GENERATOR=${CMAKE_GENERATOR})
endif ()
if (CMAKE_TOOLCHAIN_FILE)
    list(APPEND TOOLCHAIN_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
endif ()
if (CMAKE_C_COMPILER)
    list(APPEND TOOLCHAIN_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER})
endif ()
if (CMAKE_CXX_COMPILER)
    list(APPEND TOOLCHAIN_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
endif ()

# Configure OpenCV build
set(
        OPENCV_CMAKE_ARGS  # https://docs.opencv.org/4.5.5/db/d05/tutorial_config_reference.html
        # General
        -DBUILD_LIST=${OPENCV_MODULES}
        # Bundled components
        -DBUILD_TESTS=OFF
        -DBUILD_PERF_TESTS=OFF
        -DBUILD_ANDROID_PROJECTS=OFF
        -DBUILD_ANDROID_EXAMPLES=OFF
        -DBUILD_opencv_apps=OFF
        -DBUILD_JAVA=OFF
        -DBUILD_FAT_JAVA_LIB=OFF
        -DBUILD_KOTLIN_EXTENSIONS=OFF
        -DBUILD_OBJC=OFF
        -DBUILD_opencv_python2=OFF
        -DBUILD_opencv_python3=OFF
        # Disabled dependencies
        -DWITH_1394=OFF
        -DWITH_AVFOUNDATION=OFF
        -DWITH_CAP_IOS=OFF
        -DWITH_VTK=OFF
        -DWITH_GSTREAMER=OFF
        -DWITH_GTK=OFF
        -DWITH_WIN32UI=OFF
        -DWITH_FFMPEG=OFF
        -DWITH_V4L=OFF
        -DWITH_DSHOW=OFF
        -DWITH_MSMF=OFF
        -DWITH_DIRECTX=OFF
        -DWITH_VA=OFF
        -DWITH_VA_INTEL=OFF
        -DWITH_LAPACK=OFF                 # TODO: make sure it's not used
        -DWITH_PROTOBUF=OFF               # TODO: make sure it's not used
        -DWITH_QUIRC=OFF
        -DWITH_ANDROID_MEDIANDK=OFF
        -DWITH_ANDROID_NATIVE_CAMERA=OFF
        # Enabled dependencies
        -DWITH_OPENCL=ON
        -DWITH_OPENVX=ON                  # TODO: install OpenVX and set OPENVX_ROOT
        -DWITH_EIGEN=ON                   # TODO: install Eigen (and probably use EIGEN_INCLUDE_PATH)
        -DWITH_IPP=ON
        # Cross-compilation handling
        -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        ${TOOLCHAIN_ARGS}
)
file(MAKE_DIRECTORY ${OPENCV_BUILD_DIR})  # TODO: check if this manual creation is necessary
execute_process(COMMAND ${CMAKE_COMMAND} ${OPENCV_CMAKE_ARGS} -S ${OPENCV_DOWNLOAD_DIR}/opencv-${OPENCV_VERSION} -B ${OPENCV_BUILD_DIR})
message(STATUS "Configuring OpenCV build - done")

# Start OpenCV build
message(STATUS "Building OpenCV")
execute_process(COMMAND ${CMAKE_COMMAND} --build ${OPENCV_BUILD_DIR})
message(STATUS "Building OpenCV - done")