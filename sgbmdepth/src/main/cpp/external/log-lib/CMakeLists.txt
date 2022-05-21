message(STATUS "Configuring log-lib")

find_library(log-lib log)
target_link_libraries(${PROJECT_NAME} PRIVATE ${log-lib})

message(STATUS "Configuring log-lib - done")
