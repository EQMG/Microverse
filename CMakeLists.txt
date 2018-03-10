cmake_minimum_required (VERSION 3.1)
project (Polyorbis)

# Options
option(POLYORBIS_INSTALL "Generate installation target" ON)

# Compiler Options
message(STATUS "System: ${CMAKE_SYSTEM}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
    add_definitions(-DFLOUNDER_COMPILER_MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /Ox /Ob2 /Oi /Ot /GT /GF /GS- /fp:fast /fp:except- /MP /W3 /wd4251")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-DFLOUNDER_COMPILER_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall") # -Wextra -Werror -pedantic
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions(-DFLOUNDER_COMPILER_GNU)
	add_definitions(-D__USE_MINGW_ANSI_STDIO=0)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall") # -Wextra -Werror -pedantic
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s")
endif()

# Output Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE}/bin)

# Dependencies
include(Libraries/FindFlounder.cmake)

string(STRIP "${LIBRARIES_INCLUDES}" LIBRARIES_INCLUDES)
string(STRIP "${LIBRARIES_LINKS}" LIBRARIES_LINKS)

message(${LIBRARIES_INCLUDES})
message(${LIBRARIES_LINKS})

# Sources
add_subdirectory(Sources/Polyorbis)
