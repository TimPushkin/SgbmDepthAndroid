project("examples")

add_executable(${PROJECT_NAME} get_depth_example.cpp)

target_link_libraries(${PROJECT_NAME} sgbm_android_core)
