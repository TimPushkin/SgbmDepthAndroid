project("examples")

add_executable(${PROJECT_NAME} GetDepthExample.cpp)

target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
