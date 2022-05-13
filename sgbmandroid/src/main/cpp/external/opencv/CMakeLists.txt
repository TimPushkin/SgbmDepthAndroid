message(STATUS "Configuring OpenCV")

set(OPENCV_VERSION 4.5.5)
set(OPENCV_PUBLIC_LIBRARIES opencv_core)
set(OPENCV_PRIVATE_LIBRARIES opencv_calib3d opencv_imgproc opencv_imgcodecs opencv_flann)

get_filename_component(OPENCV_BUILD_DIR ../../build/opencv REALPATH)
list(APPEND OpenCV_ROOT ${OPENCV_BUILD_DIR})
message(DEBUG "OPENCV_BUILD_DIR is ${OPENCV_BUILD_DIR}")

find_package(OpenCV ${OPENCV_VERSION} REQUIRED ${OPENCV_PUBLIC_LIBRARIES} ${OPENCV_PRIVATE_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENCV_PUBLIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENCV_PRIVATE_LIBRARIES})

message(STATUS "Configuring OpenCV - done")
