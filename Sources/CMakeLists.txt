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

if(ACID_BUILD_RELEASE AND WIN32)
	add_executable(Microverse WIN32 ${MICROVERSE_SOURCES})
else()
	add_executable(Microverse ${MICROVERSE_SOURCES})
endif()

set_target_properties(Microverse PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	FOLDER "Microverse"
)

add_dependencies(Microverse Acid)

target_include_directories(Microverse PUBLIC ${ACID_INCLUDE_DIR} ${MICROVERSE_INCLUDE_DIR})
target_link_libraries(Microverse PUBLIC Acid)

if(MSVC)
	set_target_properties(Microverse PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif(UNIX AND APPLE)
	set_target_properties(Microverse PROPERTIES
		MACOSX_BUNDLE_BUNDLE_NAME "Microverse"
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${MICROVERSE_VERSION}
		MACOSX_BUNDLE_LONG_VERSION_STRING ${MICROVERSE_VERSION}
		MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/Libraries/Acid/Scripts/MacOSXBundleInfo.plist.in"
	)
endif()

# Install
if(MICROVERSE_INSTALL)
	install(DIRECTORY .
		DESTINATION include
		FILES_MATCHING PATTERN "*.h"
		PATTERN "Private" EXCLUDE
	)

	install(TARGETS Microverse
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endif()
