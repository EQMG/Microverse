include(CMakeSources.cmake)
#project(Polyorbis)

set(POLYORBIS_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/Polyorbis/")

#if(FLOUNDER_CONFIG_RELEASE AND WIN32)
#	add_executable(Polyorbis WIN32 ${POLYORBIS_SOURCES})
#else()
add_executable(Polyorbis ${POLYORBIS_SOURCES})
#endif()

add_dependencies(Polyorbis FlounderEngine)

target_include_directories(Polyorbis PUBLIC ${LIBRARIES_INCLUDES} ${POLYORBIS_INCLUDES})
target_link_libraries(Polyorbis PRIVATE ${LIBRARIES_LINKS})

# Install
if (POLYORBIS_INSTALL)
    install(
            DIRECTORY .
            DESTINATION include
            FILES_MATCHING PATTERN "*.h"
            PATTERN "Private" EXCLUDE
    )

    install(
            TARGETS Polyorbis
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    )
endif()
