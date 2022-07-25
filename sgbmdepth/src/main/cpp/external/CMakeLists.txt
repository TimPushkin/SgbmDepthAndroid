add_subdirectory(opencv)

if (ANDROID)
    add_subdirectory(log-lib)
endif ()

if (DEFINED SCAPIX_BRIDGE)
    add_subdirectory(scapix)
endif ()
