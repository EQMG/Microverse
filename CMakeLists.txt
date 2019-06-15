# 3.11.0 required for cxx_std_17 and FetchContent
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
project(Microverse VERSION 0.0.1 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(MICROVERSE_LINK_RESOURCES "Links the Resources to the bin directory" ON)

# Sets the install directories defined by GNU
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Add property to allow making project folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Position-independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to ON
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Under some compilers CMAKE_DEBUG_POSTFIX is set to "d", removed to clean dll names
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Set Debug library postfix")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "Set RelWithDebInfo library postfix")
# Removes any dll prefix name on windows, unix will keep a prefix set as "lib"
if(WIN32)
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

# Used to include Acid cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

# Allows usage of configure time content
include(FetchContent)

find_package(Acid QUIET)
if(NOT Acid_FOUND)
	FetchContent_Declare(acid
			GIT_REPOSITORY https://github.com/EQMG/Acid.git
			GIT_TAG master
			)
	FetchContent_GetProperties(acid)
	if(NOT acid_POPULATED)
		foreach(_acid_option "BUILD_TESTS" "ACID_INSTALL_EXAMPLES")
			set(${_acid_option} OFF CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(acid)
		add_subdirectory(${acid_SOURCE_DIR} ${acid_BINARY_DIR})
	endif()
endif()

# Sources
add_subdirectory(Sources)
