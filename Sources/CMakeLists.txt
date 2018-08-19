include(CMakeSources.cmake)
#project(Microverse)

set(MICROVERSE_INCLUDES "${PROJECT_SOURCE_DIR}/Sources/")

#if(ACID_CONFIG_RELEASE AND WIN32)
#add_executable(Microverse WIN32 ${MICROVERSE_SOURCES})
#else()
add_executable(Microverse ${MICROVERSE_SOURCES})
#endif()

set_target_properties(Microverse PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Microverse")

add_dependencies(Microverse Acid)

target_include_directories(Microverse PUBLIC ${ACID_INCLUDES} ${MICROVERSE_INCLUDES})
target_link_libraries(Microverse PRIVATE Acid)

# Install
if (MICROVERSE_INSTALL)
	install(
			DIRECTORY .
			DESTINATION include
			FILES_MATCHING PATTERN "*.h"
			PATTERN "Private" EXCLUDE
	)

	install(
			TARGETS Microverse
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
	)
endif()
