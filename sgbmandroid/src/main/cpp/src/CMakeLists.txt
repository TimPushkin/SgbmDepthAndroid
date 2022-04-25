# Globbing is used to detect generated files like JNI bindings
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
