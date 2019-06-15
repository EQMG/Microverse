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

add_executable(Microverse ${MICROVERSE_SOURCES})

target_compile_features(Microverse PUBLIC cxx_std_17)
target_include_directories(Microverse PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Microverse PRIVATE Acid::Acid)

set_target_properties(Microverse PROPERTIES
		FOLDER "Microverse"
		)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set_target_properties(Microverse PROPERTIES 
			LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
			)
elseif(UNIX AND APPLE)
	set_target_properties(Microverse PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Microverse"
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${MICROVERSE_VERSION}
		MACOSX_BUNDLE_LONG_VERSION_STRING ${MICROVERSE_VERSION}
		MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in"
		)
endif()

# Creates a symbolic link from Resources to Resources/Game in the binary folder
if(MICROVERSE_LINK_RESOURCES)
	get_filename_component(CURRENT_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH)
	add_custom_command(TARGET Microverse POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/Resources
			COMMAND ${CMAKE_COMMAND} -E create_symlink ${CURRENT_PARENT_DIR}/Resources ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/Resources/Game 
			)
endif()

install(TARGETS Microverse
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		)