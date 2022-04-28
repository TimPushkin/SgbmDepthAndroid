add_subdirectory(opencv)
add_subdirectory(scapix)

if (ANDROID)
    add_subdirectory(log-lib)
endif ()
