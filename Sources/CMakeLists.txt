file(GLOB_RECURSE MICROVERSE_HEADER_FILES
		"*.h"
		"*.hpp"
		)
file(GLOB_RECURSE MICROVERSE_SOURCE_FILES
		"*.c"
		"*.cpp"
		"*.rc"
		)
set(MICROVERSE_SOURCES
		${MICROVERSE_HEADER_FILES}
		${MICROVERSE_SOURCE_FILES}
		)
set(MICROVERSE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Sources/")

add_executable(Microverse ${MICROVERSE_SOURCES})
# Defined in find_package(Acid)
if(NOT ACID_FOUND)
	add_dependencies(Microverse Acid)
endif()

target_compile_features(Microverse PUBLIC cxx_std_17)
set_target_properties(Microverse PROPERTIES
		POSITION_INDEPENDENT_CODE ON
		FOLDER "Microverse"
		)

target_include_directories(Microverse PRIVATE ${ACID_INCLUDE_DIR} ${MICROVERSE_INCLUDE_DIR})
target_link_libraries(Microverse PRIVATE Acid)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Microverse PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(Microverse PROPERTIES
			MACOSX_BUNDLE_BUNDLE_NAME "Microverse"
			MACOSX_BUNDLE_SHORT_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_LONG_VERSION_STRING ${ACID_VERSION}
			MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Scripts/MacOSXBundleInfo.plist.in"
			)
endif()

add_test(NAME "Physics" COMMAND "Microverse")

if(ACID_INSTALL_EXAMPLES)
	install(TARGETS Microverse
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			)
endif()
